#include <LiquidCrystal.h>

#include <LedControl.h>

#include <EEPROM.h>

// Pin configuration for the LCD display
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte dinPin = 12; // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11; // pin 11 is connected to the CLK pin 13
const byte loadPin = 10; // pin 10 is connected to LOAD pin 12

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const byte lcdBPin = 3; //brightness pin for LCD


// Analog joystick pins
const byte xPin = A0;
const byte yPin = A1;
const byte swPin = 2;

// EEPROM addresses for LCD brightness settings
const byte lcdBrightnessAddr = 50;
byte lcdBrightnessVal = 0;
byte brightnessSteps = 7;

// EEPROM addresses for MATRIX brightness settings
const byte matrixBrightnessAddr = 60;
byte matrixBrightnessVal = 0;


const char introText[16] = "Impossible Road";

// Custom LCD character for the game
const byte skeletonIndex = 0;
const byte skeleton[] = { // used to simulate the loading bar
  B01110,
  B11111,
  B10101,
  B10101,
  B11111,
  B01010,
  B01110,
  B00000
};


byte selectedOptionIndex = 1;
byte selectedOptionCharacter[] = {
  B00000,
  B00000,
  B01110,
  B10101,
  B11111,
  B10101,
  B01110,
  B00000
};

const byte lcdRowSize = 2;
const byte lcdColSize = 16;


// Joystick movement thresholds
const int minThreshold = 300;
const int maxThreshold = 700;

// Enum for joystick directions
enum joystickDirection {
  UP,
  DOWN,
  RIGHT,
  LEFT,
  PRESS
};

// Delays for joystick movement and debounce
const int moveJoystickDelay = 150;
unsigned long lastMove = 0;
const int debounceDelay = 200;
unsigned long lastDebounce = 0;
bool pressed = false;

// Menu text 
const char * parentOptionsMessage[] = {
  "  Start Game ",
  "  Settings ",
  "  About ",
  " "
};

const char * settingsOptionsMessage[] = {
  "  LCD Brightness",
  "  Matrix Brightness ",
  "  Exit ",
  " "
};

const char aboutMessage[] = "Impossible Road | Author: Ilie Octavian Tudor | Github: Tudorr02";


byte selectedOption = 0; //the option that is selected by user on menu
byte arrayComponents = 0; // the number of menu options
byte pointerRow = 0; // the user cursor
byte submenuPointerRow = 0; // the user cursor, used in submenus
int state = 0; 
// state=0 => cycle though Main menu
// state=-1 => block infinite main menu printing
// state=2 => call menu option's functions



enum menuOptions {
  START = 10,
  SETTINGS = 20, LCD_BRIGHTNESS, MATRIX_BRIGHTNESS, EXIT,
  ABOUT = 30
};

// Flag to prevent redundant menu prints
bool printLock = false;

void setup() {

  matrixConfiguration();
  getEEPROMVals();
  setLCDBrightness(lcdBrightnessVal);
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin(lcdColSize, lcdRowSize);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(lcdBPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(swPin), buttonPressed, FALLING);

  setCustomCharacters();

  introMessage();
  menuPrint(pointerRow = 0, parentOptionsMessage);

}
void loop() {

  Menu();

}

void introMessage() {
  // Displaying intro text and a loading-like animation on LCD
  lcd.home();
  lcd.print(introText);

  for (int i = 0; i < lcdColSize; i++) {
    lcd.setCursor(i, 1); //set the cursor to the next row/line;
    lcd.write(skeletonIndex);
    delay(200);
  }
}

joystickDirection joystickControl() {
  // Reads joystick inputs and returns the direction of movement
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  if (pressed) {
    pressed = false;
    return PRESS;
  }

  if (xValue > maxThreshold)
    return RIGHT;

  if (xValue < minThreshold)
    return LEFT;

  if (yValue > maxThreshold)
    return DOWN;

  if (yValue < minThreshold)
    return UP;

  return -1;

}

void buttonPressed() {
  // Interrupt service routine for joystick button press
  // Debouncing and setting the 'pressed' flag for joystick button
  if (millis() - lastDebounce > debounceDelay) {
    pressed = true;
    lastDebounce = millis();
  }

}

void menuPrint(byte SORow, char * options[]) {
  // Function to print menu options on the LCD
  lcd.clear();
  byte col = 0;
  byte row = 0;
  byte option = (SORow % 2 == 0) ? SORow : (SORow - 1);

  byte position = SORow % 2;

  lcd.home();
  lcd.print(options[option]);

  lcd.setCursor(col, row + 1);
  lcd.print(options[option + 1]);

  lcd.setCursor(col, position);
  lcd.write(selectedOptionIndex);

}

void generateOption(byte value) {
  // Function to generate menu option based on joystick input
  // Modifying the selectedOption based on the menu item selected
  if (selectedOption < 10)
    selectedOption = (value + 1) * 10;
  else if (selectedOption % 10 == 0)
    selectedOption += (value + 1);

}

void setParentOption() {
  // Adjusting selectedOption to return to the parent menu
  selectedOption -= selectedOption % 10;
}

void updateMenuPrint(byte & SORow, byte length) {
  // Updating the menu display based on joystick movement
  joystickDirection direction = joystickControl();

  if (direction == PRESS) {
    if (state == 2) {
      printLock = false;
    }
    generateOption(SORow);
    state = 2;
    return;
  }

  if (SORow < length - 2 && direction == DOWN) {
    if (state == 2) {
      printLock = false;
    } else
      state = 0;
    SORow++;
  }

  if (SORow > 0 && direction == UP) {
    if (state == 2) {
      printLock = false;
    } else
      state = 0;
    SORow--;
  }

}

void settingsMenu() {
  // Settings menu logic
  // Handling the display and navigation of the settings menu
  byte options = sizeof(settingsOptionsMessage) / sizeof(settingsOptionsMessage[0]);

  if (!printLock) {

    menuPrint(submenuPointerRow, settingsOptionsMessage);
    printLock = true;
  }

  if (millis() - lastMove > moveJoystickDelay) {
    updateMenuPrint(submenuPointerRow, options);
    lastMove = millis();
  }

}

void aboutMenu() {
  // Displaying the about message with scrolling effect
  lcd.setCursor(15, 1);
  lcd.print(aboutMessage);

  const int scrollDelay = 800;
  unsigned long lastScroll = 0;
  while (true) {

    if (millis() - lastScroll > scrollDelay) {
      lcd.scrollDisplayLeft();
      lastScroll = millis();
    }

    if (millis() - lastMove > moveJoystickDelay) {
      joystickDirection direction = joystickControl();

      if (direction == PRESS) {
        selectedOption = EXIT;
        return;
      }

      lastMove = millis();
    }

  }

}

void lcdBrightness() {
  // Adjusting the brightness of the LCD display
  if (!printLock) {
    lcd.clear();
    lcd.home();
    lcd.print("LCD Light Level");
    lcd.setCursor(0, 1);
    lcd.print("Value: ");
    lcd.print("- ");
    printLock = true;
    lcd.setCursor(9, 1);
    lcd.print(lcdBrightnessVal);
    lcd.setCursor(10, 1);
    lcd.print(" +");
  }

  int xValue = analogRead(xPin);

  if (millis() - lastMove > moveJoystickDelay) {

    lastMove = millis();

    if (xValue > maxThreshold || xValue < minThreshold) {
      if (xValue > maxThreshold && lcdBrightnessVal < brightnessSteps)
        lcdBrightnessVal += 1;

      if (xValue < minThreshold && lcdBrightnessVal > 0)
        lcdBrightnessVal -= 1;

      setLCDBrightness(lcdBrightnessVal);
      EEPROM.put(lcdBrightnessAddr, lcdBrightnessVal);
      lcd.setCursor(9, 1);
      lcd.print(lcdBrightnessVal);

    }

    joystickDirection direction = joystickControl();

    if (direction == PRESS) {
      selectedOption = SETTINGS;
      printLock = false;
      return;
    }

    lastMove = millis();
  }

}

void matrixBrightness() {
  // Adjusting the brightness of the LED matrix display
  lc.setIntensity(0, map(matrixBrightnessVal, 0, brightnessSteps, 0, 15));

  byte matrix[][8]={
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}};

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }

  if (!printLock) {
    lcd.clear();
    lcd.home();
    lcd.print("Matrix Light Lvl");
    lcd.setCursor(0, 1);
    lcd.print("Value: ");
    lcd.print("- ");
    printLock = true;
    lcd.setCursor(9, 1);
    lcd.print(matrixBrightnessVal);
    lcd.setCursor(10, 1);
    lcd.print(" +");
  }

  int xValue = analogRead(xPin);

  if (millis() - lastMove > moveJoystickDelay) {

    lastMove = millis();

    if (xValue > maxThreshold || xValue < minThreshold) {
      if (xValue > maxThreshold && matrixBrightnessVal < brightnessSteps)
        matrixBrightnessVal += 1;

      if (xValue < minThreshold && matrixBrightnessVal > 0)
        matrixBrightnessVal -= 1;

      lc.setIntensity(0, map(matrixBrightnessVal, 0, brightnessSteps, 0, 15));
      EEPROM.put(matrixBrightnessAddr, matrixBrightnessVal);
      lcd.setCursor(9, 1);
      lcd.print(matrixBrightnessVal);

    }

    joystickDirection direction = joystickControl();

    if (direction == PRESS) {
      selectedOption = SETTINGS;
      printLock = false;
      return;
    }

    lastMove = millis();
  }

}

void Menu() {
  // General menu handling function
  // Main menu handling logic, calling relevant functions based on state

  if (state == 2) {

    switch (selectedOption) {

      case SETTINGS:
        settingsMenu();
        break;

      case ABOUT:
        aboutMenu();
        break;

      case LCD_BRIGHTNESS:
        lcdBrightness();
        break;

      case MATRIX_BRIGHTNESS:
        matrixBrightness();
        break;

      case EXIT: {
        setParentOption();
        printLock = false;
        submenuPointerRow = 0;
        state = 0;
      }
        break;

      default:{
        lcd.clear();
        lcd.home();
        lcd.print("ERROR MENU");
        lcd.setCursor(0, 1);
        lcd.print("INVALID OPTION");
      }

    }

  } else {

    if (state == 0) {
      selectedOption = 0;
      menuPrint(pointerRow, parentOptionsMessage);
      state = -1;
      arrayComponents = sizeof(parentOptionsMessage) / sizeof(parentOptionsMessage[0]);
    }

    if (millis() - lastMove > moveJoystickDelay) {
      updateMenuPrint(pointerRow, arrayComponents);
      lastMove = millis();
    }

  }

}

void setCustomCharacters() {
  lcd.createChar(skeletonIndex, skeleton);
  lcd.createChar(selectedOptionIndex, selectedOptionCharacter);
}

void getEEPROMVals() {
  EEPROM.get(lcdBrightnessAddr, lcdBrightnessVal);
  EEPROM.get(matrixBrightnessAddr, matrixBrightnessVal);
  Serial.println(matrixBrightnessVal);
}

void setLCDBrightness(byte lcdBrightnessVal) {
  analogWrite(lcdBPin, map(lcdBrightnessVal, 0, brightnessSteps, 0, 255));
}

void matrixConfiguration() {
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, map(matrixBrightnessVal, 0, brightnessSteps, 0, 15)); // sets brightness (0~15 possible values)
  lc.clearDisplay(0); // clear screen
}