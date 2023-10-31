# Introduction To Robotics 2023-2024 🤖
  This GitHub repository is dedicated to my laboratory homeworks.<br />

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

## Homework 2
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


