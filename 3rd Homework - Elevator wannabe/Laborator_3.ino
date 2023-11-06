const int floorButtonPin[]= {4,5,6};
const int floorLedPin[] = {7,8,9}; 
const int soundPin=11;
const int greenLedPin=12;

int state=0;
int lastButtonState[] = {LOW,LOW,LOW};
int floorButtonState[3];
int greenLedState=0; 
int ledState=1;

unsigned long lastDebounceTime[] = {0,0,0};
unsigned long lastFloorTime=0;
unsigned long lastSoundTime=0;
unsigned long lastGreenLedToggle=0;

unsigned long debounceDelay = 50;  
const int firstTriggerDelay=1000;// this variable is used to avoid the first trigger which is made automatically
const int soundDelay=2000;
const int floorDelay=2000;
const int greenLedDelay=100;

byte lastFloor=0;
byte wantedFloor=5; //should be between 3 and 255

void setup() {
 
 for(int i=0;i<3;i++){
    pinMode(floorButtonPin[i], INPUT_PULLUP);
    pinMode(floorLedPin[i], OUTPUT);
  }
  
  pinMode(greenLedPin, OUTPUT);
  digitalWrite(floorLedPin[0],HIGH);
  digitalWrite(greenLedPin,HIGH);
  Serial.begin(9600);
}

void FloorTrigger(const int buttonPin, int & lastButtonState, unsigned long & lastDebounceTime, int & buttonState, byte & wantedFloor){

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {                
        if(millis()>firstTriggerDelay){

          lastButtonState=reading;
          lastSoundTime=millis();

          if(buttonPin == floorButtonPin[0])
            wantedFloor=0;

          if(buttonPin == floorButtonPin[1])
            wantedFloor=1;

          if(buttonPin == floorButtonPin[2])
            wantedFloor=2;
          
        }
        
      }
    }
  }
  
  lastButtonState = reading;
}

void goToFloor(byte & fromFloor, byte toFloor){

    if(millis()-lastFloorTime>floorDelay){
      if(fromFloor== toFloor){
        digitalWrite(floorLedPin[toFloor],HIGH);
        digitalWrite(greenLedPin,HIGH);

        lastFloor=toFloor;
        state=2;
        lastSoundTime=millis();
      }
      else{ 
        int step;
        if(fromFloor<toFloor)
          step=1;
        else
          step=-1;
          digitalWrite(floorLedPin[fromFloor],ledState);

          if(ledState==LOW)
            fromFloor+=step;
         
          ledState=!ledState;
          lastFloorTime=millis();
      }
    }
}

void greenLedToggle(){

   if (millis() - lastGreenLedToggle >= greenLedDelay) {
        greenLedState = !greenLedState; 
        digitalWrite(greenLedPin, greenLedState);
        lastGreenLedToggle = millis(); 
    }
}

void moveSound(){
  tone(soundPin,440,3);
  Serial.print("2");
}

void finishSound(){
  tone(soundPin,1670,3); 
  Serial.print("3");
}

void closeSound(){
  tone(soundPin,880,3);
  Serial.print("1");
}
void loop() {

  if(state==1){
    if(millis()-lastSoundTime>soundDelay){
      moveSound();
      goToFloor(lastFloor, wantedFloor);
      greenLedToggle();
    }else
    closeSound();
   
  }else if(state==2){
    finishSound();
    digitalWrite(greenLedPin,HIGH);

    if(millis()-lastSoundTime>soundDelay){
      wantedFloor=5;
      state=0;
      ledState=1;
    }
  }

  if(state==0){
    FloorTrigger(floorButtonPin[0], lastButtonState[0] , lastDebounceTime[0], floorButtonState[0],wantedFloor);
    FloorTrigger(floorButtonPin[1], lastButtonState[1] , lastDebounceTime[1], floorButtonState[1],wantedFloor);
    FloorTrigger(floorButtonPin[2], lastButtonState[2] , lastDebounceTime[2], floorButtonState[2],wantedFloor);

    if(wantedFloor>=0 && wantedFloor<3 && wantedFloor!=lastFloor)
      state=1;
  }
}
