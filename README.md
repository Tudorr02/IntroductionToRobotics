# Introduction To Robotics 2023-2024 🤖
  This GitHub repository is dedicated to my laboratory homeworks.<br />
  

  |Homework |Name|
  |:---:|:---:|
  |[1st Homework ](#homework-1)| PWM RGB |
  |[2nd Homework ](#homework-2)| Elevator Wannabe|
  |[3rd Homework ](#homework-3)| 7 segment display drawing|
  |[4th Homework ](#homework-4)| Stopwatch timer|  
  |[5th Homework ](#homework-5)| Pseudo-smart environment monitor and logger|  
  
  
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
 ### Workflow / Requirements:
  
  ### Implementation details
      
  #### *Components Used*

#### *State Machine*

The code utilizes a state machine with different states:


### Bonus:

### Code 
💻 https://github.com/Tudorr02/IntroductionToRobotics/blob/main/5th%20Homework%20-%20Pseudo-smart%20environment%20monitor%20and%20logger/Tema5.ino
### Photo
<img src="https://github.com/Tudorr02/IntroductionToRobotics/assets/92024989/c156625d-031f-41aa-8a10-ea9a1db340df" width="500" height="500">

### Video


### Sources: 

##### [back to top](#introduction-to-robotics-2023-2024-)
