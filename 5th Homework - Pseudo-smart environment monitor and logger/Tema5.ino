#include<EEPROM.h>

/// NOTIFICATION : EACH FUNCTION IS EXPLAINED SO DON'T WORRY

// SENSOR_SAMPLING_INTERVAL = 1 suboption of 1 main menu option = 10 + 1= 11
// SENSOR_SETTINGS = 1 option in main Menu = 1* 10
// and so on...
// the 'parent' options will be always a multiple of 10 ( first digit being the number of the option in main menu)
// the 'child' options will be derived from parents options (parent option + number of suboption)
enum Menu {
  MAIN_MENU = 0
    //1. Sensor Settings
    , SENSOR_SETTINGS = 10, SENSORS_SAMPLING_INTERVAL, ULTRASONIC_ALERT_THRESHOLD, LDR_ALERT_THRESHOLD
    //2. Reset Logger Data
    , RESET_LOGGER_DATA = 20, RESET_LOGGER_DATA_YES, RESET_LOGGER_DATA_NO
    //3. System Status
    , SYSTEM_STATUS = 30, CURRENT_SENSOR_READINGS, CURRENT_SENSOR_SETTINGS, DISPLAY_LOGGED_DATA
    //4. RGB LED Control
    , RGB_LED_CONTROL = 40, MANUAL_COLOR_CONTROL, LED_TOGGLE_AUTOMATIC
}
state;

// locked helps us to determine the program's state
// locked=0 -> cycle through menu
// locked=1 -> reading input for a specific function
// locked=2 -> similar to locked=1, used for avoiding the blockers ( input from Serial Monitor).Used for sensor readings functions!
byte locked = 0;

// pins for ultrasonic sensor
const byte ultrasonicEchoPin = 6;
const byte ultrasonicTrigPin = 7;

// pin for LDR sensor
const byte LDRPin = A0;

//used for handling the sampling rate
unsigned long lastSensorsRead = 0;

//used for avoiding the int overflow , anyway we don't need a long variable bcs we won't store that much data
//this variable resets JUST the counter of loggers to 0 if it's exceeded. The stored data counter helps us to prind the LoggerData so we don't want it to be overflowed
const int maxAllowedDataStore = 5000;

// EEPROM addres and variable used to store the counter value
// Counters helps us to get the total number of individual loggers saved in EEPROM. 
// These also helps us in the following corner case : in case we do not have enough saved loggers , we will print a notification message in case the DISPLAY_LOGGED_DATA is called
int counterLDR;
const int counterLDRAddress = 190;

int counterUltrasonic;
const int counterUltrasonicAddress = 120;

//LED pins
const byte colorRedPin = 3;
const byte colorBluePin = 9;
const byte colorGreenPin = 5;

// these pins helps us to decide whether we have an alert or not in case LED is on Automatic mode ON.
byte alertUltrasonic = 0;
byte alertLDR = 0;

// this variables helps us to unblock Loggers printing  in Serial Monitor
byte showUltrasonic = 0;
byte showLDR = 0;

const int ledStateAddress = 350; // address of the LED state in EEPROM
byte ledState;

// EEPROM addres and variable used to store the Ultrasonic threshold value
const int ultrasonicThresholdAddress = 210;
int ultrasonicThreshold;

// EEPROM addres and variable used to store the LDR threshold value
const int LDRThresholdAddress = 214;
int LDRThreshold;

// // EEPROM addres and variable used to store the intensity of the LED value ( AUTOMATIC STATE OFF)
const int rgbColorRedAddress = 260;
int rgbColorRed;

const int rgbColorGreenAddress = 264;
int rgbColorGreen;

const int rgbColorBlueAddress = 270;
int rgbColorBlue;

// the maximum number of saved loggers in EEPROM
const int maxLoggerData = 6;

// EEPROM addres and variable used to store the LDR & Ultrasonic Loggers
const int storedUltrasonicDataAddress = 180;
int storedUltrasonicData;
const int storedLDRDataAddress = 185;
int storedLDRData;

const int ultrasonicLoggerIndex = 0; /// first ultrasonic logger index . This also mean the first EEPROM address in which Ultrasonic Loggers will be stored 
const int ultrasonicLoggerIndexAddress = 99;
int lastUltrasonicLoggerIndex; // this variable will be updated in case we store a logger in EEPROM

// same situation as above 
const int LDRLoggerIndex = 30;
const int LDRLoggerIndexAddress = 101;
int lastLDRLoggerIndex;

//BACK STATE (this will be controlled with the help of backState array)
// this array stores the Back option number of each submenu. 
//Ex: Back option in Sensor settings will be the 4-th one so backstates[1]=4
const int backStates[5] = {-1,4,2,4,3};

String spacing=String("\n\n\n\n\n\n\n\n\n\n\n");

//starting option
byte option = 0;
byte functionStep = 0; // this variable will help us to manage the printing inside functions & switch between locked=0 and locked=1/2.
byte step = 0; // this variable will help us to split functions in 2 subcomponents , step==1 => ask for input , step==2 => make the actions.

//values which will be updated in functions:
const int samplingRateAddress = 200;
int samplingRate;

void setup() {

  Serial.begin(9600);
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);

  pinMode(colorBluePin, OUTPUT);
  pinMode(colorGreenPin, OUTPUT);
  pinMode(colorRedPin, OUTPUT);
  while (!Serial) {

  }

  mainMenu();
  //clearEEPROM();
  getEEPROMVars();
}

void loop() {

  if (locked == 2) {

    readSensors();

    if (Serial.available() > 0) {
      int value = Serial.read() - '0';
      callFunction(option, value, step);
    }

  } else if (locked == 1) { // function reading state
    readSensors();
    int inputValue = readNumber();
    callFunction(option, inputValue, step);

  } else if (Serial.available() > 0) { // locked==0 , means that we can navigate thourgh menu
    readSensors();

    int inputValue = Serial.parseInt();

    if (inputValue < 10)
      option = generateOption(inputValue, option);

    if (option % 10 != 0 || inputValue != 0 && inputValue < 10) {
      switch (option) {

      case MAIN_MENU:
        option = 0;
        break;

      case SENSOR_SETTINGS:
        sensorSettings();
        break;

      case SENSORS_SAMPLING_INTERVAL:
        sensorsSamplingInterval(inputValue, step);
        break;

      case ULTRASONIC_ALERT_THRESHOLD:
        ultrasonicAlertThreshold(inputValue, step);
        break;

      case LDR_ALERT_THRESHOLD:
        LDRAlertThreshold(inputValue, step);
        break;

      case RESET_LOGGER_DATA:
        resetLoggerData();
        break;

      case RESET_LOGGER_DATA_YES:
        resetLoggerData_YES(inputValue, step);
        break;

      case SYSTEM_STATUS:
        systemStatus();
        break;

      case CURRENT_SENSOR_READINGS:
        currentSensorReadings(inputValue, step);
        break;

      case CURRENT_SENSOR_SETTINGS:
        currentSensorSettings(inputValue, step);
        break;

      case DISPLAY_LOGGED_DATA:
        displayLoggedData(inputValue, step);
        break;

      case RGB_LED_CONTROL:
        rgbLedControl();
        break;

      case LED_TOGGLE_AUTOMATIC: {
        ledState = !ledState;
        EEPROM.put(ledStateAddress, ledState);
        option = getParentState(option);
        printParentMenu(option);
      }
      break;

      case MANUAL_COLOR_CONTROL:
        manualColorControl(inputValue, step);
        break;

      default: {
        //simulate the BACK case
        if ((option % 10) == backStates[getStateIndex(option)]) {
          option = MAIN_MENU;
          mainMenu();
        } else
          // we don't update the option value if the input is wrong 
          option = getParentState(option);
      }
      break;

      }
    }

  }

  readSensors();

}

void ledAlertMode(byte alertUltrasonic, byte alertLDR) {

  // ledState==0 means we are NOT on Automatic mode
  // if ledState is on Automatic mode then the green/red color will be displayed only if one alert (exceeding a threshold value) is triggered

  if (ledState == 0) {
    analogWrite(colorRedPin, rgbColorRed);
    analogWrite(colorBluePin, rgbColorBlue);
    analogWrite(colorGreenPin, rgbColorGreen);
  } else {
    if (alertUltrasonic == 0 && alertLDR == 0) {
      analogWrite(colorRedPin, 0);
      analogWrite(colorBluePin, 0);
      analogWrite(colorGreenPin, 220);
    } else {
      analogWrite(colorRedPin, 220);
      analogWrite(colorBluePin, 0);
      analogWrite(colorGreenPin, 0);
    }
  }
}

void manualColorControl(int value, byte & step) {

  // this function sets the intensity of each LED color
  // step=0 -> printing the options & waiting for user's input
  // step=1 -> using the value argument , the desired function is called
  //-> 4 th option getting back to the parent's option and menu  
  if (step == 0) {
    locked = step = 1;
    Serial.println("Choose the color:");
    Serial.println("1.Red");
    Serial.println("2.Blue");
    Serial.println("3.Green");
    Serial.println("4.Back");
  } else if (step == 1) {
    switch (value) {
    case 1:
      setColorIntensity(rgbColorRedAddress, rgbColorRed);
      break;

    case 2:
      setColorIntensity(rgbColorBlueAddress, rgbColorBlue);
      break;

    case 3:
      setColorIntensity(rgbColorGreenAddress, rgbColorGreen);
      break;

    case 4: {
      locked = step = 0;
      option = getParentState(option);
      printParentMenu(option);
    }
    break;

    default: {
      manualColorControl(value, step = 1);
    }
    break;
    }
  }
}

void setColorIntensity(const int address, int & colorIntensity) {

  // this function set the color intensity using the EEPROM address

  Serial.println(F("Type a value between 0 (LOW) and 255 (HIGH) to set the intensity of the color. To exit this mode print a value bigger than 255 "));
  int inputValue = readNumber();

  if (inputValue >= 0 && inputValue <= 255) {
    colorIntensity = inputValue;
    Serial.print(">>>>>Succes intensity color updated to :");
    Serial.println(colorIntensity);
    EEPROM.put(address, colorIntensity);
    manualColorControl(0, step = 0);
  } else if (inputValue > 255) { /// exit mode, going to the parent menu
    manualColorControl(0, step = 0);
  } else { /// in case of wrong input the function is called again
    Serial.println(F(">>>>>Wrong Input!"));
    setColorIntensity(address, colorIntensity);
  }

}

void displayLoggedData(int value, byte & Step) {

  // this function displays the saved EEPROM data in EEPROM
  // step =0 -> printing & waiting for the user input
  // step=1 -> prints the desired data & gets back to the parent's option and menu

  if (step == 0) {
    printUltrasonicLoggers();
    printLDRLoggers();
    Serial.println("1. Back");
    locked = step = 1;
  } else if (step == 1) {
    if (value == 1) {
      locked = step = 0;
      option = getParentState(option);
      printParentMenu(option);
    }
  }

}

void currentSensorSettings(int value, byte & step) {

  // step=0 -> prints the sensor settings
  // step=1 -> get back to the paren't state and menu

  if (step == 0) {
    Serial.print(F("- Sampling rate : "));
    Serial.println(samplingRate);
    Serial.println(F("- Threshold : "));
    Serial.print(F("    - Ultrasonic threshold value: "));
    Serial.println(ultrasonicThreshold);
    Serial.print(F("    - LDR threshold value: "));
    Serial.println(LDRThreshold);
    Serial.println("1. Back ");
    locked = step = 1;
  } else if (step == 1) {
    if (value == 1) {
      step = locked = 0;
      option = getParentState(option);
      printParentMenu(option);
    }
  }
}

void currentSensorReadings(int value, byte & step) {

  // this function unblock the printing of the loggers in Serial Monitor.

  if (step == 0) {
    Serial.println(F("Type any number to continue. If you want to exit this mode TYPE 0 in Serial Monitor!"));
    locked = step = 1;
  } else if (step == 1) {
    locked = 2; // set locked=2 to be able to avoid input blocker

    if (value == 0) { // get back to the parent's option and menu
      locked = step = 0;
      option = getParentState(option);
      printParentMenu(option);
      showLDR = 0;
      showUltrasonic = 0;
    } else { // making these variables 1 means the readings will be printed in Serial Monitor
      showLDR = 1;
      showUltrasonic = 1;
    }
  }
}


int generateOption(int value, byte _option) {

  /// this function generates the option using the 'value' given by user
  /// first appearing option will be multiplied with 10 then the second value will just be added to the last option 

  if (_option > 9 && _option % 10 == 0) { ///if we are on a submenu option ( ex : 10/20/30/40). We want to match the right case ( ex: 11 /12/13 ...)
    _option += value;
  } else if (option < 10) { ///if we are in main menu
    _option = value * 10;
  }

  // option remains the same in case none of the above conditions are matched
  return _option;
}

void callFunction(byte _option, int _inputValue, byte & _step) {

  // used when we are not in the 'cycle through menu' state, helps us to manage function calling

  readSensors();
  switch (_option) {
  case SENSORS_SAMPLING_INTERVAL:
    sensorsSamplingInterval(_inputValue, _step);
    break;

  case ULTRASONIC_ALERT_THRESHOLD:
    ultrasonicAlertThreshold(_inputValue, _step);
    break;

  case LDR_ALERT_THRESHOLD:
    LDRAlertThreshold(_inputValue, _step);
    break;

  case RESET_LOGGER_DATA_YES:
    resetLoggerData_YES(_inputValue, _step);
    break;

  case CURRENT_SENSOR_READINGS:
    currentSensorReadings(_inputValue, _step);
    break;

  case CURRENT_SENSOR_SETTINGS:
    currentSensorSettings(_inputValue, _step);
    break;

  case DISPLAY_LOGGED_DATA:
    displayLoggedData(_inputValue, _step);
    break;

  case MANUAL_COLOR_CONTROL:
    manualColorControl(_inputValue, _step);
    break;

  default:
    Serial.println(F("Back to main menu!..."));
    break;

  }
}

int getParentState(byte _option) {

  // this function helps us to get the parent's option value
  if (_option % 10 == 0)
    _option /= 10;
  _option -= _option % 10;

  return _option;
}

void printParentMenu(byte parentState) {

  // this function prints the desired parent menu
  switch (parentState) {

  case SENSOR_SETTINGS:
    sensorSettings();
    break;

  case RESET_LOGGER_DATA:
    resetLoggerData();
    break;

  case SYSTEM_STATUS:
    systemStatus();
    break;

  case RGB_LED_CONTROL:
    rgbLedControl();
    break;

  default: {
    Serial.println(F("Back to Main Menu...."));
    mainMenu();
  }
  break;

  }
}

int readNumber() {

  // this function helps us to read an entire number from user
  while (Serial.available() == 0) {
    readSensors();
  }
  int value = Serial.parseInt();

  return value;
}
int getStateIndex(int state) {

  /// This function helps us to get the right index of the parent state (the MENU ones, not the Submenu) . That index will be used to get the wanted value from backStates array.
  // Ex: we are in ULTRASONIC_ALERT_THRESHOLDL ( value =12) , number of back option in SENSOR_SETTINGS  is the 4-th one so getStateIndex returns 1 ( the first digit of the state) => backStates[1]=4
  int digit = (state > 9) ? (state / 10) : state;
  return digit;

}

void mainMenu() {
  Serial.println(spacing);
  Serial.println(F("Main Menu:"));
  Serial.println(F("1. Sensor Settings"));
  Serial.println(F("2. Reset Logger Data"));
  Serial.println(F("3. System Status"));
  Serial.println(F("4. RGB LED Control"));
}

void sensorSettings() {
  Serial.println(spacing);
  Serial.println(F("-> Sensor Settings"));
  Serial.println(F("  1. Sensors Sampling Interval"));
  Serial.println(F("  2. Ultrasonic Alert Threshold"));
  Serial.println(F("  3. LDR Alert Threshold"));
  Serial.println(F("  4. Back"));

}

void resetLoggerData() {
  Serial.println(spacing);
  Serial.println(F("-> Reset Logger Data"));
  Serial.println(F("  Press [Yes] to continue."));
  Serial.println(F("  1. Yes"));
  Serial.println(F("  2. No/Back"));
}

void systemStatus() {
  Serial.println(spacing);
  Serial.println(F("-> System Status"));
  Serial.println(F("  1. Current Sensor Readings"));
  Serial.println(F("  2. Current Sensor Settings"));
  Serial.println(F("  3. Display Logged Data"));
  Serial.println(F("  4. Back"));

}

void rgbLedControl() {
  Serial.println(spacing);
  Serial.println(F("-> RGB LED Control"));
  Serial.println(F("  1. Manual Color Control"));
  Serial.print(F("  2. LED: Toggle Automatic ON/OFF"));
  Serial.print(F(" [ACTUAL STATE : "));

  if (ledState == 0)
    Serial.print(F("OFF"));
  else
    Serial.print(F("ON"));

  Serial.println(F("]"));

  Serial.println(F("  3. Back"));
}

void sensorsSamplingInterval(int value, byte & step) {

  /// step=0 , printing & waiting fot user's input
  /// step=1, checking the inputValue and setting the sampling rate

  if (step == 0) {
    locked = step = 1;
    Serial.println(F("Please enter the desired sampling interval for the sensors"));
    Serial.println(F("The value should be between 1 and 10 (seconds)"));

  } else if (step == 1) {
    if (value > 0 && value < 11) {
      Serial.print(F(">>>Success! Sampling rate has been updated. Actual value in seconds is : "));
      samplingRate = value * 1000;
      EEPROM.put(samplingRateAddress, samplingRate);

      Serial.println(samplingRate / 1000); // convert to ms
      step = locked = 0;
      option = getParentState(option);
      printParentMenu(option);
    } else { // function is called again with step=0 if the wrong is wrong

      Serial.println(F(">>>>Wrong Input!"));
      sensorsSamplingInterval(0, step = 0);
    }

  }

}

void ultrasonicSensorRead() {

  // this calculate the distance using the trigger and Echo pins
  // this function checks if the thrashold is exceeded , the alert is triggered in case of exceeding
  // if the currentSensorReadings() is called then the messages will be printed

  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);

  long duration = pulseIn(ultrasonicEchoPin, HIGH);
  // Sound wave reflects from the obstacle, so to calculate the distance we
  // consider half of the distance traveled.
  int distance = duration * 0.034 / 2;

  storeUltrasonicEEPROM(distance);

  if (distance < ultrasonicThreshold) {
    alertUltrasonic = 1;
    if (showUltrasonic)
      Serial.print(F("!!! The object is too close   "));

  } else {
    alertUltrasonic = 0;
    if (showUltrasonic) {
      Serial.print(F("Distance : "));
      Serial.print(distance);
    }
  }

  ledAlertMode(alertUltrasonic, alertLDR);

}

void LDRSensorRead() {

  /// this function calculates the light level using the LDRpin
  /// also checks if the threshold is exceeded or not, if it is the alert is triggered
  // if the currentSensorReadings() is called then the messages will be printed
  int LDRValue = analogRead(LDRPin);
  storeLDREEPROM(LDRValue);

  if (LDRValue < LDRThreshold) {
    alertLDR = 1;
    if (showLDR)
      Serial.println(F(" !!! There's not enough light"));
  } else {
    alertLDR = 0;

    if (showLDR) {
      Serial.print(F("  | Light level : "));
      Serial.println(LDRValue); // the raw analog reading
    }
  }

  ledAlertMode(alertUltrasonic, alertLDR);
}

void readSensors() {

  // this function reads the sensors using the sampling rate

  if (millis() - lastSensorsRead > samplingRate) {
    ultrasonicSensorRead();
    LDRSensorRead();
    lastSensorsRead = millis();

  }
  ledAlertMode(alertUltrasonic, alertLDR);

}

void ultrasonicAlertThreshold(int value, byte & step) {

  // this function sets the Ultrasonic Threshold
  // step=0 ,printing the messages & wait for user input  
  // step=1 , update the threshold value and get back to the parent's option and menu

  if (step == 0) {
    step = locked = 1;
    Serial.println(F("Please enter a threshold value for Ultrasonic Sensor"));
    Serial.println(F("Take in consideration that exceeding the threshold (distance is smaller) means triggering an alert!"));

  } else if (step == 1) {

    ultrasonicThreshold = value;
    EEPROM.put(ultrasonicThresholdAddress, ultrasonicThreshold);
    Serial.print(F(">>>Success! Ultrasonic Threshold has been updated. Actual value in cm is : "));
    Serial.println(ultrasonicThreshold);
    step = locked = 0;
    option = getParentState(option);
    printParentMenu(option);

  }

}

void LDRAlertThreshold(int value, byte & step) {

  // this function sets the LDR Threshold
  // step=0 ,printing the messages & wait for user input  
  // step=1 , update the threshold value and get back to the parent's option and menu
  if (step == 0) {
    step = locked = 1;
    Serial.println(F("Please enter a threshold value for LDR Sensor"));
    Serial.println(F("Take in consideration that exceeding the threshold (low light) means triggering an alert!"));

  } else if (step == 1) {
    LDRThreshold = value;
    EEPROM.put(LDRThresholdAddress, LDRThreshold);
    Serial.print(F(">>>Success! LDR Threshold has been updated. Actual value is : "));
    Serial.println(LDRThreshold);
    step = locked = 0;
    option = getParentState(option);
    printParentMenu(option);

  }
}

void resetLoggerData_YES(int value, byte & step) {

  // this function resets the logger data using the sensor's reset functions.
  // step=0, wait for user's input and print the available the options
  // step=1 , using the desired sensor, the dedicated function will be printed OR will get back to the parent state

  if (step == 0) {
    Serial.println(F("--> Choose the sensor you want to reset:"));
    Serial.println(F("    1.LDR"));
    Serial.println(F("    2.Ultrasonic"));
    Serial.println(F("    3.Both"));
    Serial.println(F("    4.CANCEL"));
    locked = step = 1;

  } else if (step == 1) {
    switch (value) {
    case 1: {
      resetLDRLoggerData();
      option = getParentState(option);
      step = locked = 0;
      printParentMenu(option);
    }
    break;

    case 2: {
      resetUltrasonicLoggerData();
      option = getParentState(option);
      step = locked = 0;
      printParentMenu(option);
    }
    break;

    case 3: {
      resetLDRLoggerData();
      resetUltrasonicLoggerData();
      option = getParentState(option);
      step = locked = 0;
      printParentMenu(option);
    }
    break;

    case 4: {
      option = MAIN_MENU;
      printParentMenu(option);
      step = locked = 0;
    }
    break;

    default: {
      Serial.println(F(">>>Please choose an option from the listed ones!"));
    }
    break;
    }
  }
}

void storeUltrasonicEEPROM(int value) {

  // this function store loggers in EEPROM
  // when this function is called :
  // -the value argument is stored in a calculated ( with the help of the counters) addres based on a permutations algorithm type ( just updating the values in ascending order ) , ex : 0 1 2 3 4 5 -> 6,1,2,3,4,5 -> 6,7,2,3,4,5
  // -storedUltrasonicData helps us to know it we are able to print the loggers ( if the user asks for it), also is updated if exceeds the maxAllowedStore for avoiding the overflow
  // -lastUltrasonicIndex is updated ( will help on printings loggers), last address in which a value has been saved

  EEPROM.put((ultrasonicLoggerIndex + (counterUltrasonic % maxLoggerData)) * 2, value);
  lastUltrasonicLoggerIndex = ultrasonicLoggerIndex + counterUltrasonic;
  EEPROM.put(ultrasonicLoggerIndexAddress, lastUltrasonicLoggerIndex);
  ++counterUltrasonic;
  if (counterUltrasonic >= maxLoggerData)
    counterUltrasonic = 0;
  EEPROM.put(counterUltrasonicAddress, counterUltrasonic);

  storedUltrasonicData++;
  if (storedUltrasonicDataAddress > maxAllowedDataStore)
    storedUltrasonicData = maxLoggerData;

  EEPROM.put(storedUltrasonicDataAddress, storedUltrasonicData);

}

void resetUltrasonicLoggerData() {

  //- lastUltrasonicLoggerIndex is updated with the first addres (just as value) in which the values will be stored
  //- the values from allocated addreses are updated to 0
  //-counter is set to 0

  counterUltrasonic = 0;
  EEPROM.put(counterUltrasonicAddress, counterUltrasonic);
  lastUltrasonicLoggerIndex = ultrasonicLoggerIndex;
  EEPROM.put(ultrasonicLoggerIndexAddress, lastUltrasonicLoggerIndex);
  for (int i = 0; i < ultrasonicLoggerIndex + maxLoggerData; i++)
    EEPROM.put(i * 2, 0);

  storedUltrasonicData = 0;
  EEPROM.put(storedUltrasonicDataAddress, storedUltrasonicData);
}

void resetLDRLoggerData() {

  //- lastLDRLoggerIndex is updated with the first addres (just as value) in which the values will be stored
  //- the values from allocated addreses are updated to 0
  //-counter is set to 0
  counterLDR = 0;
  EEPROM.put(counterLDRAddress, counterLDR);
  lastLDRLoggerIndex = LDRLoggerIndex;
  EEPROM.put(LDRLoggerIndexAddress, lastLDRLoggerIndex);
  for (int i = 0; i < LDRLoggerIndex + maxLoggerData; i++)
    EEPROM.put(i * 2, 0);

  storedLDRData = 0;
  EEPROM.put(storedLDRDataAddress, storedLDRData);

}

void storeLDREEPROM(int value) {

  // same as storeUltrasonicEEPROM 

  EEPROM.put((LDRLoggerIndex + (counterLDR % maxLoggerData)) * 2, value);
  lastLDRLoggerIndex = LDRLoggerIndex + counterLDR;
  EEPROM.put(LDRLoggerIndexAddress, lastLDRLoggerIndex);
  ++counterLDR;
  if (counterLDR >= maxLoggerData)
    counterLDR = 0;

  EEPROM.put(counterLDRAddress, counterLDR);
  storedLDRData++;

  if (storedLDRData > maxAllowedDataStore)
    storedLDRData = maxLoggerData;

  EEPROM.put(storedLDRDataAddress, storedLDRData);

}

void printUltrasonicLoggers() {

  // this function prints the values in an order based on the lastUltrasonicLoggerIndex 
  // we have 6 7 1 2 3 4 5, the first for loop prints: 1 2 3 4 5; the second for loop prints : 6 7

  int value;
  int index = lastUltrasonicLoggerIndex;

  if (storedUltrasonicData >= maxLoggerData) {
    Serial.print("Ultrasonic Loggs :{ start -> ");
    for (int i = index + 1; i < ultrasonicLoggerIndex + maxLoggerData; i++) {
      EEPROM.get(i * 2, value);
      Serial.print(value);
      Serial.print(" -> ");
    }

    for (int i = ultrasonicLoggerIndex; i < index + 1; i++) {
      EEPROM.get(i * 2, value);
      Serial.print(value);
      if (i != (ultrasonicLoggerIndex + maxLoggerData - 1))
        Serial.print(" -> ");
    }
    Serial.println(" -> end }");
  } else {
    Serial.println("Ultrasonic sensor has not collected enough data yet :(");

  }

}

void printLDRLoggers() {

  // this function prints the values in an order based on the lastLDRLoggerIndex 
  // we have 6 7 1 2 3 4 5, the first for loop prints: 1 2 3 4 5; the second for loop prints : 6 7
  int value;
  int index = lastLDRLoggerIndex;

  if (storedLDRData >= maxLoggerData) {
    Serial.print("LDR Loggs :{ start -> ");
    for (int i = index + 1; i < LDRLoggerIndex + maxLoggerData; i++) {
      EEPROM.get(i * 2, value);
      Serial.print(value);
      Serial.print(" -> ");

    }
    for (int i = LDRLoggerIndex; i < index + 1; i++) {
      EEPROM.get(i * 2, value);
      Serial.print(value);

      if (i != (LDRLoggerIndex + maxLoggerData - 1))
        Serial.print(" -> ");

    }
    Serial.println(" -> end }");
  } else {
    Serial.println("LDR sensor has not collected enough data yet :(");
  }

}

void clearEEPROM() {

  // this function updates all EEPROM values to 0

  for (int i = 0; i < EEPROM.length(); i++)
    EEPROM.put(i, 0);
}

void getEEPROMVars() {

  // get all EEPROM variables

  EEPROM.get(ultrasonicThresholdAddress, ultrasonicThreshold);
  EEPROM.get(LDRThresholdAddress, LDRThreshold);
  EEPROM.get(samplingRateAddress, samplingRate);

  EEPROM.get(storedUltrasonicDataAddress, storedUltrasonicData);
  EEPROM.get(counterUltrasonicAddress, counterUltrasonic);

  EEPROM.get(storedLDRDataAddress, storedLDRData);
  EEPROM.get(counterLDRAddress, counterLDR);

  EEPROM.get(ultrasonicLoggerIndexAddress, lastUltrasonicLoggerIndex);
  EEPROM.get(LDRLoggerIndexAddress, lastLDRLoggerIndex);

  EEPROM.get(rgbColorBlueAddress, rgbColorBlue);
  EEPROM.get(rgbColorGreenAddress, rgbColorGreen);
  EEPROM.get(rgbColorRedAddress, rgbColorRed);
  EEPROM.get(ledStateAddress, ledState);

}