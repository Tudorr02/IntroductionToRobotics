# Introduction To Robotics 2023-2024 🤖
  This GitHub repository is dedicated to my laboratory homeworks.<br />
  

  |Homework |Name|
  |:---:|:---:|
  |[1st Homework ](#homework-1)| PWM RGB |
  |[2nd Homework ](#homework-2)| Elevator Wannabe|
  |[3rd Homework ](#homework-3)| 7 segment display drawing|
  |[4th Homework ](#homework-4)| Stopwatch timer|  
  |[5th Homework ](#homework-5)| Pseudo-smart environment monitor and logger|  
  |[6th Homework ](#homework-6)| Mini Matrix Game | 
  |[7th Homework ](#homework-7)| Matrix Project Checkpoint |
  
  ![myimage](https://media.giphy.com/media/tczJoRU7XwBS8/giphy.gif)


  ## Each homework contains :
   - requirements
   - implementation details
   - code
   - picture of the setup
   - video

## Homework 1  

  ### Requirements
  Use a separate potentiometer for controlling each color of the RGB LED : Red, Green and Blue. This control must leverage digital electronics. Specifically, you need to read the potentiometer’s value with Arduino and then write a mapped value to the LED pins.

  ### Implementation details
   - The first ( left side ) potentiometer controls the intensity of the Red color 🔴
   - The second ( the middle one ) potentiometer controls the intensity of the Green color 🟢
   - The third ( right side ) potentiometer controls the intensity of the Blue color 🔵

### Code 
💻 https://github.com/Tudorr02/IntroductionToRobotics/blob/main/2nd%20Laboratory%20-%20PWM/Laborator_2.ino
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/702ed900-0de1-42eb-ae39-41b40ba1db6a" width="500" height="500">

### Video
https://youtube.com/shorts/UV9fsi2b-7M

##### [back to top](#introduction-to-robotics-2023-2024-)

## Homework 2
  > This  assignment  involves  simulating  a  3-floor  elevator  control  system  usingLEDs, buttons, and a buzzer with Arduino.
  ### Requirements
  Design  a  control  system  that  simulates  a  3-floor  elevator  using  the  Arduinoplatform. 
  - LED Indicators: Each of the 3 LEDs should represent one of the 3 floors.The LED corresponding to the current floor should light up.  Additionally,another LED should represent the elevator’s operational state.  It shouldblink when the elevator is moving and remain static when stationary.
  - Buttons:Implement 3 buttons that represent the call buttons from the3 floors.  When pressed, the elevator should simulate movement towardsthe floor after a short interval (2-3 seconds).
  - State Change & Timers:If the elevator is already at the desired floor,pressing the button for that floor should have no effect.  Otherwise, aftera button press, the elevator should ”wait for the doors to close” and then”move”  to  the  corresponding  floor.   If  the  elevator  is  in  movement,  itshould either do nothing or it should stack its decision (get to the firstprogrammed floor,  open the doors,  wait,  close them and then go to thenext desired floor).
  - Debounce:Remember to implement debounce for the buttons to avoidunintentional repeated button presses.
  ### Implementation details
- `state` keeps track of the elevator's current state (0 for idle, 1 for moving, 2 for finishing).
- `lastButtonState` stores the last recorded state of the floor buttons.
- `floorButtonState` keeps track of the current state of the floor buttons.
- `greenLedStat` controls the state of a green LED.
- `ledState` is used to toggle the state of the floor indicator LEDs.
- Timing variables (`lastDebounceTime, lastFloorTime, lastSoundTime, and lastGreenLedToggle`) are used to manage debounce times and delays.
- `debounceDelay` is set to 50 milliseconds to debounce the floor buttons to prevent false triggering.
- `firstTriggerDelay` introduces a delay of 1000 milliseconds to avoid the first trigger, which is automatically detected.
- `soundDelay` and floorDelay control the timing of elevator movement and floor change.
- `greenLedDelay` manages the green LED toggle timing.
- `lastFloor` and `wantedFloor` are used to keep track of the elevator's current and desired floor.
- **FloorTrigger()** Function: This function handles button presses for individual floor buttons.
It debounces the button presses and identifies the desired floor when a button is pressed.
- **goToFloor()** Function: Responsible for controlling the elevator's movement to a desired floor.
It manages the floor LED indicators and the movement of the elevator between floors.
- **greenLedToggle() Function**: Toggles the state of a green LED at regular intervals to indicate elevator operation.
- **moveSound()** **finishSound()** and **closeSound()** Functions: These functions generate different tones on a sound output pin to indicate different states of the elevator operation.
### Code 
💻 https://github.com/Tudorr02/IntroductionToRobotics/blob/main/2nd%20Homework%20-%20Elevator%20wannabe/Laborator_3.ino
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/b2527be9-74b6-4ffa-bf62-c35f4db4b913" width="500" height="500">

### Video
[https://youtube.com/shorts/UV9fsi2b-7M](https://youtu.be/b93LjvrGLxs)https://youtu.be/b93LjvrGLxs
##### [back to top](#introduction-to-robotics-2023-2024-)

# Homework 3
  > This project controls the position of the segment and ”draw” it on the display.  The movement between segments should be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
  ### Requirements
   The  initial  position  should  be  on  the  DP.  The  currentposition always blinks (irrespective of the fact that the segment is on oroff).  Use the joystick to move from one position to neighbors (see table forcorresponding movement).  Short pressing the button toggles the           segmentstate  from  ON  to  OFF  or  from  OFF  to  ON.  Long  pressing  the  buttonresets the entire display by turning all the segments OFF and moving thecurrent position to the decimal point. Use `interrupts`!
   #### Be careful :
   - Joystick movements should be done with toggle, as in the lab (joy-Moved, etc)
   - For CTI, it is part of requirement to use interrupts.
   - The  code  in  the  lab  for  joystick  movement  is  not  perfect  and  can be  improved.   The  code  should  be  wrapped  in  a  function  and  the constraint between 0 and 9 can be simplified.

  ### Implementation details
      
   - The `segmentMatrix` array is an implementation detail used to map the segments of a 7-segment LED display to joystick inputs. It enables dynamic selection and activation of specific segments based on the joystick's directional input.
    <img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/d366bc22-7d76-45cf-aeac-b7ae9e4dd6d9" width="500" height="250">
   - The **blink()** function is responsible for creating a blinking effect on a particular segment of the 7-segment display. This detail is implemented through toggling the state of the specified segment at regular intervals, enhancing the visual feedback for users.
   - The **readPosition()** and **joystickRead()** functions handle the processing of analog joystick inputs. The implementation detail here involves monitoring the joystick's analog readings, comparing them to a standard threshold, and determining the joystick's direction (up, down, left, or right). These details are vital for responsive interaction with the joystick.
   - The **swDebounce()** function is an interrupt service routine designed to debounce the switch input. This implementation detail addresses switch bounce, ensuring that the switch's state change is detected accurately and reliably.
   - The **resetSegments()** function is an implementation detail used to reset all segments of the 7-segment display to their OFF state. It is triggered after a prolonged switch press, ensuring that the system can return to a known state.


### Code 
💻 [https://github.com/Tudorr02/IntroductionToRobotics/blob/main/2nd%20Laboratory%20-%20PWM/Laborator_2.ino](https://github.com/Tudorr02/IntroductionToRobotics/blob/main/3rd%20Homework%20-%207%20segment%20display%20drawing/sketch_nov4a.ino)
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/25e0318d-6c96-4d4b-9ddb-22a584f79acd" width="500" height="500">
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/9b5521e8-d78a-4a5f-a419-890089b6badf" width="500" height="500">

### Video
[https://youtube.com/shorts/UV9fsi2b-7M](https://youtube.com/shorts/Bmy6azuQxgY?si=GB1e-MOokvwtQPSW)https://youtube.com/shorts/Bmy6azuQxgY?si=GB1e-MOokvwtQPSW
##### [back to top](#introduction-to-robotics-2023-2024-)
# Homework 4
> Using the 4 digit 7 segment display and 3 buttons, you should implement a stopwatch timer that counts in 10ths of a secondand has a save lap functionality (similar to most basic stopwatch functionson most phones).
  ### Requirements
   The starting value of the 4 digit 7 segment display shouldbe ”000.0”.  Your buttons should have the following functionalities:
   - Button 1:  Start / pause.
   - Button  2:   Reset  (if  in  pause  mode).   Reset  saved  laps  (if  in  lapviewing mode).
  - Button 3:  Save lap (if in counting mode), cycle through last savedlaps (up to 4 laps).
   #### Workflow:
   1.  Display shows ”000.0”.  When pressing theStartbutton, the timershould start.
   2.  During timer counter, each time you press the lap button, you shouldsave  that  timer’s  value  in  memory  (not  persistent,  it  is  OK  to  bedeleted upon reset), up to 4 laps (or more if you want); pressing the5th  time  should  override  the  1st  saved  one.   If  you  press  the  resetbutton while timer works, nothing happens.  If you press the pausebutton, the timer stops.
   3. InPauseMode, the lap flag button doesn’t work anymore.  Pressingthe reset button resets you to 000.0.
   4. After reset, you can now press the flag buttons to cycle through thelap times.  Each time you press the flag button, it takes you to thenext saved lap.  Pressing it continuously should cycle you through itcontinuously.  Pressing the reset button while in this state resets allyour flags and takes the timer back to ”000.0”.
   #### Be careful :
   -  Make sure you put the dot ”.” on the display to separate the secondsfrom the 10ths of a second
  - You should display all the digits.  For example,  if you have ”20.4”seconds, you should display ”020.4”.
   -  For  CTI,  interrupts  are  mandatory  for  thesave  lap  and  pausebutton.   This  is  to  increase  precision  of  counting.   For  ComputerScience, these are bonus.

  ### Implementation details
      
  #### *Components Used*

- Shift Register: Connections to the shift register are defined using `latchPin`, `clockPin`, and `dataPin` for efficient control of the 4-digit 7-segment display.
- Digit Control Pins: Pins `segD1`, `segD2`, `segD3`, and `segD4` are used to control which digit is currently active for displaying.
- Button Pins: Buttons for Start/Pause (`buttonPin[0]`), Reset (`buttonPin[1]`), and LAP (`buttonPin[2]`).

#### *State Machine*

The code utilizes a state machine with different states:

- `INITIAL_STATE`: Initial state for setup.
- `COUNTING_STATE`: Stopwatch is actively counting.
- `PAUSE_STATE`: Stopwatch is paused.
- `LAP_STATE_ADD`: Lap is added during counting state.
- `LAP_STATE_CYCLE`: Cycling through lap display.

#### *Display and Encoding*
- Hexadecimal encodings (0-F) for the 7-segment display are defined in the `byteEncodings` array.
- The `writeNumber` function converts a given number to its corresponding 7-segment display encoding.
#### *Button Handling and Debouncing*
Interrupts are used for button debouncing (`btnDebounce` function). The `attachInterrupt` function is utilized to trigger specific actions on button presses.
#### *Lap Functionality*
Laps are stored in an array (`laps`) with a specified lap count (`lapCount`). Laps are added, and the lap display is cycled through during the LAP state.
#### *Display Update and Shift Register*
The `writeReg` function updates the shift register with the current digit's encoding. The `activateDisplay` function manages which digit is currently active.



### Bonus:
- Empty LAP is displayed as `_ _ _ _`
### Code 
💻 https://github.com/Tudorr02/IntroductionToRobotics/blob/main/4th%20Homework%20-%20Stopwatch%20timer/Laborator_5.ino
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/e4aad90a-bc49-402a-8487-0e69bbd861e5" width="500" height="500">

### Video
https://youtube.com/shorts/RqltP6ljy88?feature=share

### Sources: 
- *Debounce on a Pushbutton template* : https://docs.arduino.cc/built-in-examples/digital/Debounce
- *Debounce using interrupts template* : https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

##### [back to top](#introduction-to-robotics-2023-2024-)

# Homework 5
> Develop a ”Smart Environment Monitor and Logger” using Arduino.  This system will utilize various sensors to gather environmental data, log this data intoEEPROM, and provide both visual feedback via an RGB LED and user interac-tion through a Serial Menu.  The project focuses on integrating sensor readings,memory management, Serial Communication and the general objective of build-ing a menu.
 ### Menu Structure (Workflow / Requirements) :
 
  1. **Sensor Settings**<br>

       - **1.1** ***Sensors Sampling Interval:*** <br> Here you should be prompted fora value between 1 and 10 seconds.  Use this value as a sampling ratefor the sensors.  You can read a separate value for each or have thesame for both.<br>
       - **1.2** ***Ultrasonic Alert Threshold:*** <br>Here you should be promptedfor a threshold value for the ultrasonic sensor.  You can decide if thatis the min or max value (you can signal that something is too close).When sensor value exceeds the threshold value,  an alert should begiven.  This can be in the form of a message.  If the LED is set toAutomatic Mode (see section 4.2), it should also turn red if any ofthe sensors are outside the value.<br>
       - **1.3** ***LDR Alert Threshold:*** <br> Here you should be prompted for athreshold  value  for  the  LDR  sensor.   You  can  decide  if  that  is  themin or max value (for example, it could signal that night is coming).When sensor value exceeds the threshold value,  an alert should begiven.  This can be in the form of a message.  If the LED is set toAutomatic Mode (see section 4.2), it should also turn red if any ofthe sensors are outside the value.<br>
       - **1.4** ***Back:*** <br> Return to the main menu.<br>

2. **Reset Logger Data**.Should  print  a  message,  promting  if  you  toconfirm to delete all  data.  Something like ”are you sure?”,  followed bythe submenu with YES or NO. You can reset both sensor data at the sametime, or you can do it individually.  Your choice.  Individually makes moresense, but I’m trying to simplify the homework.<br>

     - **2.1** ***Yes.***<br>
     - **2.2**  ***No.***

3. **System Status**<br>

   - **3.1** ***Current Sensor Readings:*** <br> Continuously print sensor readingsat the set sampling rate, from all sensors.  Make sure you have a wayto exit this (such as pressing a specific key) and inform the user ofthis method through a message.<br>
   - **3.2** ***Current Sensor Settings:*** <br> Displays  the  sampling  rate  andthreshold value for all sensors.<br>
   - **3.3** ***Display Logged Data:*** <br> Displays last 10 sensor readings for allsensors.  (or be creative and do it another way).<br>
   - **3.4** ***Back:*** <br> Return to the main menu.<br>

4. **RGB LED Control**<br>

   - **4.1** ***Manual Color Control:*** <br> Set the RGB colors manually.  Youdecide how to input them, either by making an option for each chan-nel, or by putting a string etc.  If you expect a specific format, makesure to inform the user.<br>
   - **4.2** ***LED: Toggle Automatic ON/OFF:*** <br>  If  automatic  mode  isON, then the led color should be GREEN when all sensors value donot exceed threshold values (aka no alert) and RED when there is analert (aka ANY sensor value exceeds the threshold). When automaticmode is OFF, then the LED should use the last saved RGB values.<br>
   - **4.3** ***Back:*** <br> Return to the main menu.<br>
  
  ### Main Components:
- **Menu Navigation**: Utilizes an enumeration (`Menu`) for menu options, supporting easy navigation through main and sub-menus.
- **Sensor Control**: Interfaces with ultrasonic and LDR (Light Dependent Resistor) sensors, including setting thresholds and sampling intervals.
- **LED Control**: Manages an RGB LED, allowing manual color control and an automatic alert mode based on sensor readings.
- **Data Logging**: Stores sensor data in EEPROM and provides functionality to display and reset this logged data.
- **EEPROM Management**: Handles reading and writing of various settings and logged data to EEPROM.

### Key Functionalities:
- **Menu System**: Facilitates cycling through main and sub-menus, executing functions based on user input.
- **Sensor Reading and Alerting**: Regularly samples sensors, comparing readings to set thresholds to trigger alerts.
- **LED Management**: Offers manual control over LED colors and an automatic mode that changes colors based on sensor alerts.
- **Data Storage and Retrieval**: Logs sensor readings to EEPROM and retrieves them on command.
- **Settings Persistence**: Saves and retrieves settings like thresholds, sampling rates, and LED states from EEPROM, ensuring persistence across power cycles.

### Implementation Details:
- **Locked States**: Uses a `locked` variable to manage program flow, distinguishing between menu navigation, function execution, and sensor reading.
- **Pin Configuration**: Defines and initializes GPIO pins for sensors and LED.
- **Function Modularization**: Each menu option is associated with a specific function, streamlining the process of adding or modifying features.
- **EEPROM Data Handling**: Employs addresses and counters to efficiently store and manage sensor data and settings in EEPROM.
- **Sensor Functions**: Includes specialized functions for reading and processing data from ultrasonic and LDR sensors.

###  Menu States (Enum 'Menu'):
Enumerates various states of the menu system, including both main menu options and their sub-menus. Examples:
- `MAIN_MENU`
- `SENSOR_SETTINGS`
- `RESET_LOGGER_DATA`
- `SYSTEM_STATUS`
- `RGB_LED_CONTROL`
- Sub-options like `SENSORS_SAMPLING_INTERVAL`, `ULTRASONIC_ALERT_THRESHOLD`, etc.

### Program Flow State (`locked` Variable):
Controls the flow of the program with three distinct states:
- `0`: Menu navigation mode, allowing the user to cycle through different menu options.
- `1`: Reading input for a specific function. The program expects user input for the selected menu option.
- `2`: Similar to state `1`, but specifically for reading sensor data without blocking other operations.


### Variables Description:
- **Menu state**: Enumerates different menu states for navigation.
- **byte locked**: Manages program flow; `0` for menu navigation, `1` for input reading, `2` for sensor reading.
- **byte ultrasonicEchoPin**, **byte ultrasonicTrigPin**: Pins for the ultrasonic sensor.
- **byte LDRPin**: Pin for the LDR sensor.
- **unsigned long lastSensorsRead**: Tracks the last time sensors were read.
- **int counterLDR**, **int counterUltrasonic**: Counters for LDR and ultrasonic sensor data stored in EEPROM.
- **byte colorRedPin**, **byte colorBluePin**, **byte colorGreenPin**: Pins for RGB LED.
- **byte alertUltrasonic**, **byte alertLDR**: Flags for ultrasonic and LDR alerts.
- **byte showUltrasonic**, **byte showLDR**: Flags to control sensor data display.
- **byte ledState**: State of the LED (manual or automatic mode).
- **int ultrasonicThreshold**, **int LDRThreshold**: Threshold values for sensors.
- **int rgbColorRed**, **int rgbColorGreen**, **int rgbColorBlue**: Intensity values for LED colors.
- **int lastUltrasonicLoggerIndex**, **int lastLDRLoggerIndex**: Last indices where sensor data was logged.
- **const int backStates[5]**: Array containing back option numbers for each submenu.
- **byte option**, **byte step**: Variables for managing menu navigation and function execution.
- **int samplingRate**: Variable for sensor sampling rate.

### Functions Description:
- `setup()`: Initializes the Arduino setup, including serial communication and pin configurations.
- `loop()`: Main program loop handling menu navigation, sensor reading, and function calling.
- `ledAlertMode(byte, byte)`: Controls the LED based on sensor alerts and mode.  
- `manualColorControl(int, byte&)`: Allows manual control of the LED's color intensity.
- `setColorIntensity(const int, int&)`: Sets the intensity of a specific LED color.
- `displayLoggedData(int, byte&)`: Displays logged sensor data.
- `currentSensorSettings(int, byte&)`: Displays current sensor settings.
- `currentSensorReadings(int, byte&)`: Manages the display of current sensor readings.
- `generateOption(int, byte)`: Generates the option value for menu navigation.
- `callFunction(byte, int, byte&)`: Calls a specific function based on the current menu option.
- `getParentState(byte)`: Returns the parent menu state of a given option.
- `printParentMenu(byte)`: Prints the menu corresponding to a parent state.
- `readNumber()`: Reads a complete number from the Serial input.
- `getStateIndex(int)`: Returns the index of a state for the `backStates` array.
- `mainMenu()`, `sensorSettings()`, `resetLoggerData()`, `systemStatus()`, `rgbLedControl()`: Functions to print various menus.
- `sensorsSamplingInterval(int, byte&)`, `ultrasonicSensorRead()`, `LDRSensorRead()`, `readSensors()`, `ultrasonicAlertThreshold(int, byte&)`, `LDRAlertThreshold(int, byte&)`: Functions for managing sensor settings and readings.
- `resetLoggerData_YES(int, byte&)`: Manages the reset of logged sensor data.
- `storeUltrasonicEEPROM(int)`, `resetUltrasonicLoggerData()`, `storeLDREEPROM(int)`, `resetLDRLoggerData()`: Functions for storing and resetting sensor data in EEPROM.
- `printUltrasonicLoggers()`, `printLDRLoggers()`: Functions to print logged data from sensors.
- `clearEEPROM()`: Clears all data in EEPROM.
- `getEEPROMVars()`: Retrieves stored variables from EEPROM.



### Bonus:
- live printing the actual state of the LED near `Toggle Automatic Mode ON/OFF` submenu option
- option to reset loggers individually


### Code 
💻 https://github.com/Tudorr02/IntroductionToRobotics/blob/main/5th%20Homework%20-%20Pseudo-smart%20environment%20monitor%20and%20logger/Tema5.ino
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/c156625d-031f-41aa-8a10-ea9a1db340df" width="500" height="500">

### Video
https://youtu.be/9sSg39AyvPg

### Sources: 
- _minimizing_ the percentage of dynamic memory usage :  https://www.baldengineer.com/arduino-f-macro.html

##### [back to top](#introduction-to-robotics-2023-2024-)

# Homework 6
> Develop a small game on the 8x8 matrix. The purpose of this is to get you started with your matrix project.  
  ### Requirements
   The game must have at least 3 typesof elements:  player (blinks slowly),  bombs/bullets (blinks fast),  wall (doesn’tblink).Thebasic idea is that you generate walls on the map (50% - 75% of the map) and thenyou move around with the player and destroy them.  You can do it Bombermanstyle or terminator-tanks style (or another way, be creative). 
  
   #### Be careful :
  - LED Differentiating:The player and the bombs/bullets need to blinkat  different  rates.   The  player  should  blink  slowly  and  the  bomb/bulletshould blink fast.  The wall should not blink at all.
  - Control:Ensure that control is smooth.  You can implement any type ofcontrol you want on the joystick, but make sure it is ”pleasant” to playwith.
  - Control:Ensure that you do not generate walls on top of the player when the game starts.
  ### Implementation details
      
  #### *Components Used*

- Arduino Uno Board
- Joystick
- 8x8 LED Matrix
- MAX7219
- Resistors and capacitors as needed
- Breadboard and connecting wires

## Key Variables and Constants

- **Pins:**
  - `dinPin`, `clockPin`, `loadPin`: Pins connecting the Arduino to the MAX7219 LED matrix.
  - `xPin`, `yPin`, `swPin`: Pins for joystick inputs.

- **Game Mechanics:**
  - `matrixSize`: Size of the LED matrix (8x8).
  - `xPos`, `yPos`: Current position of the 'player' on the LED matrix.
  - `explosion`, `lastTrigger`, `destroyArea`: Variables controlling the 'bomb' feature in the game.
  - `timer`, `timerDelay`, `lastTimer`: Variables for managing a countdown timer.
  - `occupancyRate`: Determines complexity by setting the density of 'walls' on the LED matrix.
  - `score`: Tracks the player's score.

## Functions

- **setup()**: Initializes serial communication, sets up the LED matrix, joystick inputs, and initializes the game state (map, player position).
- **loop()**: The main game loop handling game timing, player movement, bomb planting, and blinking effects.
- **generateMap(byte occupancyRate)**: Generates a random map on the LED matrix based on the occupancy rate.
- **generatePlayerPos()**: Randomly positions the player on the matrix at game start.
- **updateMap()**: Updates the LED matrix to reflect the current game state.
- **endGame()**: Handles the termination of the game, displaying the score and the destruction rate.
- **updatePositions()**: Reads joystick input and updates the player's position.
- **blinkPlayer()**: Blinks the player's LED to make it distinguishable.
- **countScore(byte destroyArea)**: Calculates the score based on the number of walls destroyed.
- **destroyAreaState(byte state, byte destroyArea)**: Updates the state of LEDs in the bomb's area of effect.
- **plantBomb(byte destroyArea)**: Handles the bomb planting and explosion animation.
- **bombTrigger()**: Interrupt service routine for the joystick button, triggering the bomb.

## Gameplay Mechanics

- The player moves a dot (representing themselves) on the LED matrix using the joystick.
- The matrix contains randomly placed 'walls' (LEDs turned on) based on the occupancy rate.
- Pressing the joystick button plants a bomb, destroying adjacent walls and increasing the player's score.
- A countdown timer dictates the duration of each game session.

## Special Features

- **Bomb Mechanism**: An important game feature where the player can plant a bomb to destroy nearby walls.
- **Timer-Based Game End**: The game ends when the timer reaches zero, at which point the score and destruction rate are displayed.
- **Reset Function**: A function pointer `resetFunc` is used to reset the game if needed.


### Code 
💻 https://github.com/Tudorr02/IntroductionToRobotics/blob/main/6-th%20Homework%20-%20Mini%20Matrix%20Game/Tema6.ino
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/77a9e149-4f1f-4704-9f2a-7fcbd102ae3a" width="500" height="500">
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/7614df36-bf1e-4267-8430-38547f32f53a" width="500" height="500">

### Video
[https://youtube.com/shorts/RqltP6ljy88?feature=share](https://youtu.be/n3jlpPuWBoU)

### Sources: 
- *Debounce using interrupts template* : https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

##### [back to top](#introduction-to-robotics-2023-2024-)







# Homework 7
>  Matrix Project Checkpoint
  ### Requirements
   Develop a menu which will be displayed on an LCD screen and navigated using a joystick. This interface is crucial for enhancing the gaming experience by allowing players to easily navigate through various game options. 
  
  Add the following functionality for this checkpoint:
  
 ### 1. Intro Message
   -  When  powering  up  the  project,  a  greeting  message should be shown for a few moments.
 ### 2. Menu:
  - Start game, starts the initial level of the game
  - Settings.
    - LCD  brightness  control.   Save  value  to  EEPROM  and  load  itupon system start.
    - Matrix brightness control (see function setIntesnity from the led-Control library). Make sure to display something on the matrixwhen  selecting  it.   Save  value  to  EEPROM  and  load  it  upon system start.
    - About:should include details about the creator(s) of the game.  Atleast game name, author and github link or user (use scrolling text?).
### 3. End Message
  - When the game ends, add a message.  Wait for a promtfrom the user (a button push, for example), before returning to main menu again.
### 4. During gameplay:
  - Show  relevant  details,  that  are  dynamic  (change  with  gameplay):time,  level,  lives etc.  Doesn’t matter,  what can be used.  You canimplement the number of LEDs that are still switched on.
  - Implement an end game / level functionality.  You current gameshould end.  For example,  if you implemented the game where you”destroy”  all  the  LEDs,  it  should  end  when  you  finished  them  all.Upon ending, it should display the aforementioned message, wait foruser prompt and then return to the main menu again.

## *Components Used*
  
1. **Arduino Board**
2. **Liquid Crystal Display (LCD)**
3. **LED Matrix**
4. **Analog Joystick**
5. **EEPROM**
7. **Custom Characters**

   
## Implementation details
      
- **`setup()`**: Initializes the Arduino setup, including serial communication and pin configurations.
- **`loop()`**: Main program loop handling menu navigation, sensor reading, and function calling.
- **`introMessage()`**: Displays the game title "Impossible Road" and animates custom characters on the LCD to simulate a loading bar.
- **`joystickControl()`**: Reads joystick inputs and returns the direction of movement.
- **`buttonPressed()`**: Interrupt service routine for joystick button press, handling debouncing and setting the 'pressed' flag.
- **`menuPrint(byte SORow, char * options[])`**: Function to print menu options on the LCD.
- **`generateOption(byte value)`**: Generates menu option based on joystick input.
- **`setParentOption()`**: Adjusts selectedOption to return to the parent menu.
- **`updateMenuPrint(byte & SORow, byte length)`**: Updates the menu display based on joystick movement.
- **`settingsMenu()`**: Handles the display and navigation of the settings menu.
- **`aboutMenu()`**: Displays the about message with scrolling effect.
- **`lcdBrightness()`**: Adjusts the brightness of the LCD display.
- **`matrixBrightness()`**: Adjusts the brightness of the LED matrix display.
- **`Menu()`**: General menu handling function.
- **`setCustomCharacters()`**: Defines custom characters for LCD display.
- **`getEEPROMVals()`**: Retrieves stored values from EEPROM.
- **`setLCDBrightness(byte lcdBrightnessVal)`**: Sets the brightness of the LCD.
- **`matrixConfiguration()`**: Configures the LED matrix settings.


### Code 
💻 [https://github.com/Tudorr02/IntroductionToRobotics/blob/main/6-th%20Homework%20-%20Mini%20Matrix%20Game/Tema6.ino](https://github.com/Tudorr02/IntroductionToRobotics/blob/main/7-th%20Homework%20-%20Matrix%20Project%20Checkpoint%20v1.1/tema7.ino)
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/cd681967-0377-4d7f-a9b2-efd854a75b05" width="700" height="500">

##### [back to top](#introduction-to-robotics-2023-2024-)
