// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register
 
// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

volatile unsigned long pausedTime=0;
volatile int startNumber=0;
 
// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display
byte exceptionBlocker=0;// do not count the first debounce which is made automatically

// buttonPin[0]= Start/Pause
//buttonPin[1]=Reset
//buttonPin[2]=LAP
byte buttonPin[]={3,8,2};

int laps[4]={0,0,0,0};
int lapCount=4;
int lapIndex=0;

unsigned long lastDebounceTime[]={0,0,0};
const unsigned long debounceDelay=150;
const unsigned long resetDelayBtn=50;
int lastButtonState[]={0,0,0};
int buttonState[3];

volatile enum State {INITIAL_STATE, COUNTING_STATE, PAUSE_STATE, LAP_STATE_ADD, LAP_STATE_CYCLE} state;

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};
// Variables for controlling the display update timing

const byte DPEncoding=B00000001;
unsigned long lastIncrement = 0;
unsigned long delayCount = 100; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed

 
void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 
  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);

    if(i<3){
      pinMode(buttonPin[i], INPUT_PULLUP);
    }
  }  

  attachInterrupt(digitalPinToInterrupt(buttonPin[0]),btnDebounce,FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin[2]),btnDebounce ,FALLING);
  // Begin serial communication for debugging purposes
  Serial.begin(9600);
}
 
void loop() {

  switch (state) {
  
    case INITIAL_STATE: initialState();
    break;

    case COUNTING_STATE:{
      if(millis()-lastIncrement>delayCount){
        startNumber++;
        startNumber%=10000;
        lastIncrement=millis();
      }
      countingState(startNumber);
      
    } 
    break; 

    case PAUSE_STATE: {
      writeNumber(pausedTime);
      resetDebounce();
    }
    break;

    case LAP_STATE_ADD: {
      addLap(startNumber);
      state=COUNTING_STATE;
    }
    break;

    case LAP_STATE_CYCLE:{
      if(lapIndex==4){
        lapIndex=0;
      }else
        seeLap(lapIndex);
      resetDebounce();
    } 
    break;
  }


}

void seeLap(int _lapIndex){

  unsigned int currentLap= laps[_lapIndex];
  if(currentLap==0){
    
    for (int i = 0; i < displayCount; i++) {
      digitalWrite(displayDigits[i], LOW);
    }
    writeReg( B00010000);
     
  }else{
    writeNumber(currentLap);
  }
  
}

void addLap(int time){
  Serial.print("ADDED LAP: ");
  
  if(laps[lapCount-1]!=0){
    for(int i=0;i<lapCount-1;i++){
      laps[i]=laps[i+1];
    }
    laps[lapCount-1]=time;
    Serial.println(laps[lapCount-1]);
  }else{

    for(int i=0;i<lapCount;i++){
      if(laps[i]==0){
        laps[i]=time;
        Serial.println(laps[i]);
        break;
      }

    }
  }
 
}
 
void writeReg(byte digit) {
    // Prepare to shift data by setting the latch pin low
    digitalWrite(latchPin, LOW);
    // Shift out the byte representing the current digit to the shift registe

    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // Latch the data onto the output pins by setting the latch pin high
    digitalWrite(latchPin, HIGH);

    
}
 
 
void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    if(i!=displayNumber)
    digitalWrite(displayDigits[i], HIGH);
  }
  
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}
 
void writeNumber(int number) {
  // DONE: Initialize necessary variables for tracking the current number and digit position
  int currentNumber = number;
  int displayDigit = 3; // Start with the least significant digit
  int lastDigit = 0;

  
 
  // DONE: Loop through each digit of the current number
  while (currentNumber != 0) {
    // DONE: Extract the last digit of the current number
    lastDigit = currentNumber % 10;
    // DONE: Activate the current digit on the display
    printDigit(displayDigit,byteEncodings[lastDigit]);
    displayDigit--;
    // DONE: Update 'currentNumber' by removing the last digit
    currentNumber /= 10;
  }

  while(displayDigit>=0){
    printDigit(displayDigit,byteEncodings[0]); //to print the rest of the 0's
    displayDigit--;
  }
}

void initialState(){

  for(int i=0; i< displayCount ; i++){
    printDigit(i,byteEncodings[0]);
  }
  startNumber=0;
  pausedTime=0;
  
}

void printDigit(int displayIndex, byte currentDigit){
  
  activateDisplay(displayIndex);
  currentDigit=(displayIndex==2 && currentDigit!=0)?(currentDigit| DPEncoding):currentDigit; //decimal point check
  writeReg(currentDigit);
  writeReg(B00000000);
  
}



void countingState(int startNr){
  
  writeNumber(startNr);

}

void resetDebounce(){


   int reading = digitalRead(buttonPin[1]);
  
    
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[1]) {
    // reset the debouncing timer
    lastDebounceTime[1]= millis();
  }

  if ((millis() - lastDebounceTime[1]) > resetDelayBtn) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState[1]) {
      buttonState[1] = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState[1] == HIGH ) {
          if(exceptionBlocker!=0){
            if(state== LAP_STATE_CYCLE){
              for(int i=0;i<lapCount;i++)
                laps[i]=0;
            }
            if(state == PAUSE_STATE || state== LAP_STATE_CYCLE)
              state=INITIAL_STATE;
            
          }
        exceptionBlocker=1;
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState[1] = reading;

}

void btnDebounce(){

  if ((digitalRead(buttonPin[0])==LOW) && (micros() - lastDebounceTime[0]) > (debounceDelay*1000)) {
    if(state==INITIAL_STATE || state==PAUSE_STATE || state==LAP_STATE_CYCLE){
      state=COUNTING_STATE;
      startNumber=pausedTime;
    }
    else if(state==COUNTING_STATE){
      state=PAUSE_STATE;
      pausedTime=startNumber;
    }
    lastDebounceTime[0]=micros();
  }else if((digitalRead(buttonPin[2])==LOW) &&(micros() - lastDebounceTime[2]) > (debounceDelay*1000)){
    if(state==COUNTING_STATE)
      state=LAP_STATE_ADD;
    else if(state==INITIAL_STATE){ 
      lapIndex=0;
      state=LAP_STATE_CYCLE;
    }else if(state==LAP_STATE_CYCLE){
      lapIndex++;
    }
    lastDebounceTime[2]=micros();
    
  }

}