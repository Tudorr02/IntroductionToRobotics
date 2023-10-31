
// red vars
const int redPin=6;
const int redPot=A0;

//blue vars
const int bluePin=3;
const int bluePot=A2;

//green vars
const int greenPin=5;
const int greenPot=A1;

const int minPotValue=0;
const int maxPotValue=1023;

const int minDigValue=0;
const int maxDigValue=255;

void setup() {
  
  pinMode(redPin, OUTPUT);
  pinMode(redPot, INPUT);

  pinMode(greenPin, OUTPUT);
  pinMode(greenPot, INPUT);

  pinMode(bluePin, OUTPUT);
  pinMode(bluePot, INPUT);

  Serial.begin(9600);
  
}

void PWM(int potColor, int pinColor ){

  int potValue=analogRead(potColor);
  int intensity=map(potValue,minPotValue,maxPotValue,minDigValue,maxDigValue);
  analogWrite(pinColor, intensity);
}

void loop() {
 
 PWM(redPot, redPin);
 PWM(greenPot, greenPin);
 PWM(bluePot, bluePin);
   
}
