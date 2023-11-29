#include "LedControl.h" // need the library

const byte dinPin = 12; // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11; // pin 11 is connected to the CLK pin 13
const byte loadPin = 10; // pin 10 is connected to LOAD pin 12
const byte matrixSize = 8; // 1 as we are only using 1 MAX7219
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 2;

// joystick pins
const int xPin = A0;
const int yPin = A1;
const int swPin=2;

// the coordinates of the dot
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;


// used to trigger the bomb
bool explosion=false;

// last time when the bomb has been triggered ( the joystick button has been pressed)
unsigned long lastTrigger=0;

// the number of destroyable dots per view : front- back - right -left
const byte destroyArea=3;

// the state of an wall ( dot representing a part of an wall) . used as a parameter for blinking
byte dotState=1;

// used in plantBomb function.
// planting=0 => count the score for before explosion ( turning the wall off) AND make turn all the destroyable area on ( blinking reasons)
// planting=1 => blocking reasons
byte planting=0;

// the number of seconds until the end of the game/level
int timer=20;
const int timerDelay=1000;// check if one second has been reached
unsigned long lastTimer=0;// used to simulate an entire second

// Thresholds for joystick movement detection
const int minThreshold = 200;
const int maxThreshold = 600;

// Timing variables for movement updates
const byte moveInterval = 350;
unsigned long lastMoved = 0;

// used to determine the complexity of the level
const int occupancyRate = 30;

// used as a delay ,used to recognise the player
const int playerBlink = 400;
// last time when the pleyer dot has changed his state (on-off)
unsigned long lastPlayerBlink = 0;

// used as a delay, used to recognise the bomb
const int destroyBlink=100;
//last time when the dots ( which creates a wall) have changed their led state (on-off)
unsigned long lastDestroyedBlink=0;

//initialized before blinking the wall, it helps us to check if the blinking animation has been ended
unsigned long destroyTime=0;
// the bomb animation time
const int plantBombDelay=1000;

// 2D array representing the state (on/off) of each LED in the matrix
byte matrix[matrixSize][matrixSize] = {
  /* Initial state with all LEDs off */ };


byte score=0;

void(* resetFunc) (void) = 0; 
 

void setup() {
  Serial.begin(9600);

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0); // clear screen

  randomSeed(analogRead(A5));//generate the seed for random map and player position

  pinMode(swPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(swPin), bombTrigger ,FALLING);

  generateMap(occupancyRate);
  generatePlayerPos();
  updateMap();
}

void loop(){

  endGame();
  if(explosion)
    plantBomb(destroyArea);

  if ((millis() - lastMoved > moveInterval ) && explosion==false) { 
    updatePositions(); // Update the position of the LED based on joystick input
    lastMoved = millis(); // Update the time of the last move
  }

  blinkPlayer();
  updateMap();

  

}

void generateMap(byte occupancyRate) {
  // generate a map using an occupanyRate as parameter

  byte numberOfWalls = (occupancyRate * (matrixSize * matrixSize)) / 100;
  byte xWall;
  byte yWall;

  while (numberOfWalls > 0) {

    xWall = random(8);
    yWall = random(8);

    if (matrix[xWall][yWall] == 0) {
      numberOfWalls--;
      matrix[xWall][yWall] = 1;
    }

  }

}

void generatePlayerPos() {

  // generate player's first position
  // we should spawn him on an empty place 
  xPos = random(8);
  yPos = random(8);

  while (matrix[xPos][yPos] != 0) {

    xPos = random(8);
    yPos = random(8);

  }

  matrix[xPos][yPos] = 1;

}

void updateMap() {

  //handles the moves in game
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]); 
     
    }
  }
}

void endGame(){

 if(millis()-lastTimer>timerDelay){

    Serial.println(timer);
    timer--;
    lastTimer=millis();

    if(timer==-1 || score>=((occupancyRate * (matrixSize * matrixSize)) / 100)  ){
      byte numberOfWalls = (occupancyRate * (matrixSize * matrixSize)) / 100;
      if(score>= numberOfWalls)
        timer=-1;
      Serial.println("TIME IS OVER");
      Serial.print("YOUR SCORE IS :");
      Serial.println(score);
      Serial.print("DESTRUCTION RATE: ");
      
      int result=((double)score/(numberOfWalls) *100)+1;
      Serial.print(result);
      while(timer=1){

      }

    }
  }
    
  
  
}

void updatePositions() {

  int xValue = analogRead(xPin); // Read the X-axis value
  
  int yValue = analogRead(yPin); // Read the Y-axis value

  // use temporary coordinates to not affect the players position in case of a failure (intersection with a wall )
  int tempY=yPos;
  int tempX=xPos;


  // Store the last positions
  xLastPos = xPos;
  yLastPos = yPos;

  // Update tempY based on joystick movement
  if (xValue < minThreshold) {
    tempY = (yPos > 0) ? yPos - 1 : matrixSize - 1;
  } else if (xValue > maxThreshold) {
    tempY = (yPos < matrixSize - 1) ? yPos + 1 : 0;
  }
  // Update tempX based on joystick movement
  if (yValue < minThreshold) {
    tempX = (xPos > 0) ? xPos - 1 : matrixSize - 1;
  } else if (yValue > maxThreshold) {
    tempX = (xPos + 1) % matrixSize;
  }

  if(matrix[tempX][tempY]==0 ){
     
    // in case of succes ( empty space) the xPos and yPos are updated
    xPos=tempX;
    yPos=tempY;
    // Check if the position has changed and update the matrix accordingly
    if (xPos != xLastPos || yPos != yLastPos ) {
     // matrixChanged = true;
      matrix[xLastPos][yLastPos] = 0; // Turn off the LED at the last position
      matrix[xPos][yPos] = 1; // Turn on the LED at the new position

    }
  }

}

void blinkPlayer() {
  
  if (millis() - lastPlayerBlink > playerBlink) {
    matrix[xPos][yPos] = !matrix[xPos][yPos];
    lastPlayerBlink = millis();
  }
}



void countScore( byte destroyArea){

  // get the coordinates of the player
  int currentRow=xPos;
  int currentCol=yPos;
  destroyArea=destroyArea%matrixSize; // avoid bugs with %, destroyArea=2 -> destroys 2 blocks on each view (left-right-front-back)

  //determine the limits
  byte upLimit= (currentRow - destroyArea)>=0 ? currentRow - destroyArea : 0;
  byte downLimit= (currentRow + destroyArea)<=(matrixSize-1) ? currentRow + destroyArea : matrixSize-1;
  byte leftLimit= (currentCol - destroyArea)>=0 ? currentCol - destroyArea : 0;
  byte rightLimit= (currentCol + destroyArea)<=(matrixSize-1) ? currentCol + destroyArea :  matrixSize-1;


  //count the score for each view
   for(int row=currentRow-1; row>=upLimit  ; row--) //back
    if(matrix[row][currentCol]==1)
      score++;
  
  for(int row=currentRow+1; row<=downLimit;row++)  // front
    if(matrix[row][currentCol]==1)
      score++;

  for(int col=currentCol-1;col>=leftLimit;col--) //left
    if(matrix[currentRow][col]==1)
      score++;

  for(int col=currentCol+1;col<=rightLimit;col++) //right
    if(matrix[currentRow][col]==1)
      score++;



}

void destroyAreaState( byte state, byte destroyArea){

  // State =0 => the destroyable leds turns OFF
  // State =1 => the destroyable leds turn on
  // used for blinking and for removing the wall parts
  int currentRow=xPos;
  int currentCol=yPos;
  destroyArea=destroyArea%matrixSize;


  byte upLimit= (currentRow - destroyArea)>=0 ? currentRow - destroyArea : 0;
  byte downLimit= (currentRow + destroyArea)<=(matrixSize-1) ? currentRow + destroyArea : matrixSize-1;
  byte leftLimit= (currentCol - destroyArea)>=0 ? currentCol - destroyArea : 0;
  byte rightLimit= (currentCol + destroyArea)<=(matrixSize-1) ? currentCol + destroyArea :  matrixSize-1;

  for(int row=currentRow-1; row>=upLimit  ;row--)
    matrix[row][currentCol]=state;

  for(int row=currentRow+1; row<=downLimit;row++)
    matrix[row][currentCol]=state;

  for(int col=currentCol-1;col>=leftLimit;col--)
    matrix[currentRow][col]=state;

  for(int col=currentCol+1;col<=rightLimit;col++)
    matrix[currentRow][col]=state;
  

}


void plantBomb(byte destroyArea){

  // if planting just started, we:
  // count the score ( number of wall parts which will be destroyed)
  // set the leds of destroyable parts ON ( blinking reasons) 
  // save the destroy Time to time the animation
  if (planting==0){
    countScore(destroyArea);
    destroyAreaState(dotState=1, destroyArea);
    destroyTime=millis();
    planting=1;
  }

  // blinking animation
  if(millis()-lastDestroyedBlink>destroyBlink){

    dotState=!dotState;
    destroyAreaState(dotState,destroyArea);

    lastDestroyedBlink=millis();
  }

  // end of the explosion 
  // turn off the destroyed dots
  // unlock the planting
  if(millis()-destroyTime>plantBombDelay){
    explosion=false;
    planting=0;
    destroyAreaState(dotState=0,destroyArea);
  }
}

void bombTrigger(){
  // explosion will call the plantBomb function
  if(micros()-lastTrigger>moveInterval*1000){
    explosion=true;
    lastTrigger=micros();
    if(timer==-1)
    resetFunc();
  }
  
}






