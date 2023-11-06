const int pinA = 5;
const int pinB = 6;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 3;
const int pinG = 4;
const int pinDP =10;

const int segSize = 8;
const int indexDpPin=7;

const unsigned long resetDelay=2000000;


unsigned int standardReadX=500;
unsigned int standardReadY=500;

unsigned long lastReadJoystick=0;

int positionLine=indexDpPin;
int positionColumn=0;

const int thresHold=100;
const int joystickDelay=300;

const int blinkDelay=200;
unsigned long lastBlinkTime=0;
byte blinkState=HIGH;


const int segmentMatrix[][5]={
        //UP DOWN LEFT RIGHT
   {pinA, -1, 6, 5, 1}, 
  {pinB, 0, 6, 5, -1},
  {pinC, 6, 3, 4, 7},
  {pinD, 6, -1, 4,2},
  {pinE, 6, 3, -1, 2},
  {pinF, 0, 6, -1, 1},
  {pinG, 0, 3, -1,-1},
  {pinDP, -1,-1,2,-1}
};

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
byte swState;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1000;

int xValue = 0;
int yValue = 0;

const byte upIndex=1;
const byte downIndex=2;
const byte leftIndex=3;
const byte rightIndex=4;

///for interrupt
volatile bool swPressed=false;

int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int segmentState[segSize] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup()
{
  for (int i = 0; i < segSize; i++) {
	pinMode(segments[i], OUTPUT);
	}
  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW),swDebounce , FALLING);
  Serial.begin(9600);
  
}

void blink(const int segmentPin){
  
  digitalWrite(segmentPin, blinkState);
  
  if(millis()-lastBlinkTime>blinkDelay){
    blinkState=!blinkState;
    lastBlinkTime=millis();
  }
}

void readPosition(){

  joystickRead();
    
  if((xValue > standardReadX+thresHold || xValue <standardReadX-thresHold || yValue > standardReadY+thresHold || yValue <standardReadY-thresHold) && millis()-lastReadJoystick>joystickDelay){
   
    if(xValue > (standardReadX+thresHold)){ 
        positionColumn=rightIndex;
    }else if (xValue < (standardReadX-thresHold)) {
        positionColumn=leftIndex;
    }else if (yValue < (standardReadY-thresHold)) {
        positionColumn=upIndex;
    }else if (yValue > (standardReadY+thresHold)) {
        positionColumn=downIndex;
    }

    int indexSegment=segmentMatrix[positionLine][positionColumn];

    if(indexSegment!=-1){
      digitalWrite(segmentMatrix[positionLine][0], LOW);
      positionLine=indexSegment;
    }
    
    lastReadJoystick=millis();
  }
}

  void joystickRead(){

    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
     
    if(swPressed){
    segmentState[positionLine]=!segmentState[positionLine];
    swPressed=false;
    }


    if((micros()-lastDebounceTime>resetDelay) && digitalRead(pinSW)==LOW){
      resetSegments();
    }
  

    Serial.print("X-axis: ");
    Serial.print(xValue);

    Serial.print(" Y-axis: ");
    Serial.print(yValue);
    Serial.println(" | ");
  }

void swDebounce(){

    if ((micros() - lastDebounceTime) > (debounceDelay*1000)) {
      swPressed=true;
        lastDebounceTime=micros();
    }  
    
}

void resetSegments(){
  for(int i=0;i<segSize;i++){
    segmentState[i]=LOW;
  }
  positionLine=indexDpPin;
  
}

void loop()
{
    readPosition();
    for(int i=0;i<segSize;i++){
      digitalWrite(segments[i], segmentState[i]);
      
    }

    blink(segmentMatrix[positionLine][0]);

}