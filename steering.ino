#include <Encoder.h>
#include <Joystick.h>

// Encoder setup
Encoder myEnc(2, 3);  // Pins for the encoder

// Joystick setup
Joystick_ Joystick;

// Potentiometer pins
const int potentiometerPin1 = A0;
const int potentiometerPin2 = A1;

// Variables for encoder
long oldPosition = 0;
long accumulatedChange = 0; // Variable to track accumulated position change
const long changeThreshold = 2; // Number of units to trigger an action

// Joystick axis limits
int joystickX = 1024; // Start with the middle value (0-2048 range)
int joystickY = 512; // Start in the middle for the Y-axis

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder and Potentiometer to Joystick Test:");

  // Initialize the joystick
  Joystick.begin();
  
  // Set the axis ranges
  Joystick.setXAxisRange(0, 2048);
  Joystick.setYAxisRange(70, 970);  // Updated Y-axis to 70-970
  Joystick.setZAxisRange(0, 1024);

  // Center all axes initially
  Joystick.setXAxis(joystickX);
  Joystick.setYAxis(joystickY);
  
  // Send initial joystick state
  Joystick.sendState();
}

void loop() {
  // Handle encoder
  long newPosition = myEnc.read();
  long change = newPosition - oldPosition;
  
  // Accumulate the change
  accumulatedChange += change;
  oldPosition = newPosition;

  // Check if accumulated change reaches or exceeds the threshold
  if (abs(accumulatedChange) >= changeThreshold) {
    // Update joystick X-axis based on encoder movement
    if (accumulatedChange > 0) {
      joystickX += 15;   // Increase X-axis value
    } else {
      joystickX -= 15;   // Decrease X-axis value
    }

    // Keep joystickX within bounds (0-2048)
    joystickX = constrain(joystickX, 0, 2048);

    // Update the joystick X-axis position
    Joystick.setXAxis(joystickX);
    
    // Send joystick state to the computer
    Joystick.sendState();

    // Reset accumulated change after each update
    accumulatedChange = 0;
  }

  // --- Potentiometer 1: Map to Joystick Y-axis ---
  int potentiometerValue1 = analogRead(potentiometerPin1);
  
  // Map potentiometer (70-970) to the Y-axis (70-970)
  joystickY = map(potentiometerValue1, 70, 970, 970, 70);

  // Update joystick Y-axis
  Joystick.setYAxis(joystickY);

  // --- Potentiometer 2: Act as 7 buttons ---
  int potentiometerValue2 = analogRead(potentiometerPin2);
  
  // Divide the potentiometer range (70 to 970) into 7 zones
  int zone1 = 70 + (970 - 70) / 7;
  int zone2 = 70 + 2 * (970 - 70) / 7;
  int zone3 = 70 + 3 * (970 - 70) / 7;
  int zone4 = 70 + 4 * (970 - 70) / 7;
  int zone5 = 70 + 5 * (970 - 70) / 7;
  int zone6 = 70 + 6 * (970 - 70) / 7;

  // Determine which zone the potentiometer is in and press a corresponding button (B5 to B11)
  if (potentiometerValue2 <= zone1) {
    Joystick.pressButton(5); // Press Button B5
    Joystick.releaseButton(6);
    Joystick.releaseButton(7);
    Joystick.releaseButton(8);
    Joystick.releaseButton(9);
    Joystick.releaseButton(10);
    Joystick.releaseButton(11);
  } 
  else if (potentiometerValue2 > zone1 && potentiometerValue2 <= zone2) {
    Joystick.releaseButton(5);
    Joystick.pressButton(6); // Press Button B6
    Joystick.releaseButton(7);
    Joystick.releaseButton(8);
    Joystick.releaseButton(9);
    Joystick.releaseButton(10);
    Joystick.releaseButton(11);
  } 
  else if (potentiometerValue2 > zone2 && potentiometerValue2 <= zone3) {
    Joystick.releaseButton(5);
    Joystick.releaseButton(6);
    Joystick.pressButton(7); // Press Button B7
    Joystick.releaseButton(8);
    Joystick.releaseButton(9);
    Joystick.releaseButton(10);
    Joystick.releaseButton(11);
  }
  else if (potentiometerValue2 > zone3 && potentiometerValue2 <= zone4) {
    Joystick.releaseButton(5);
    Joystick.releaseButton(6);
    Joystick.releaseButton(7);
    Joystick.pressButton(8); // Press Button B8
    Joystick.releaseButton(9);
    Joystick.releaseButton(10);
    Joystick.releaseButton(11);
  }
  else if (potentiometerValue2 > zone4 && potentiometerValue2 <= zone5) {
    Joystick.releaseButton(5);
    Joystick.releaseButton(6);
    Joystick.releaseButton(7);
    Joystick.releaseButton(8);
    Joystick.pressButton(9); // Press Button B9
    Joystick.releaseButton(10);
    Joystick.releaseButton(11);
  }
  else if (potentiometerValue2 > zone5 && potentiometerValue2 <= zone6) {
    Joystick.releaseButton(5);
    Joystick.releaseButton(6);
    Joystick.releaseButton(7);
    Joystick.releaseButton(8);
    Joystick.releaseButton(9);
    Joystick.pressButton(10); // Press Button B10
    Joystick.releaseButton(11);
  }
  else {
    Joystick.releaseButton(5);
    Joystick.releaseButton(6);
    Joystick.releaseButton(7);
    Joystick.releaseButton(8);
    Joystick.releaseButton(9);
    Joystick.releaseButton(10);
    Joystick.pressButton(11); // Press Button B11
  }

  // Send the updated joystick state
  Joystick.sendState();
  delay(5);
}
