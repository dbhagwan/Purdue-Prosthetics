#include <Servo.h>

// Initialization
Servo servo1;
Servo servo2;
Servo servo3;

// Variables
const int flexpin = A0;          // The analog input pin to measure flex sensor position:
const int resistorPin = A1;      // Current spike measured through shunt resistor
const int vibrationpin = 4;      // Make sure to pick a PWM pin
const int buttonPin = 7;         // Digital pin connected to pushbutton
const int ledCount = 4;          // the number of LEDs in the bar graph
int flexposition;                // Input value from the analog pin
int servoposition;               // Output value to the servo
int ledPins[] = { 0, 1, 2, 3 };  // Array of pin numbers to which LEDs are attached
int vibrationIntensity;          // Vibration intensity maps to hand closure
int currentValue;                 // Analog value read from the voltage drop across resistor
int threshold = 120;             // Threshold value to determine when to stop actuators
int buttonState = 0;             // State of the button


void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  pinMode(buttonPin, INPUT);
  pinMode(flexpin, INPUT);
  pinMode(vibrationpin, OUTPUT);
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);  // Reads the state of the push button

  while (buttonState == 1) {
    digitalWrite(ledPins[0], HIGH);      // Turn on LED 1
    flexposition = analogRead(flexpin);  // Read the position of the flex sensor (0 to 1023)

    // ** Servo **
    // Because the voltage divider circuit only returns a portion
    // of the 0-1023 range of analogRead(), we'll map() that range
    // to the servo's range of 0 to 180 degrees. The flex sensors
    // we use are usually in the 600-900 range:
    servoposition = map(flexposition, 600, 900, 0, 180);
    servoposition = constrain(servoposition, 0, 180);

    servo1.write(servoposition);  // Command the servo to move to that position
    servo2.write(servoposition);  // Command the servo to move to that position
    servo3.write(servoposition);  // Command the servo to move to that position

    // ** Stop on contact **
    currentValue = analogRead(resistorPin);
    if (currentValue >= threshold) {
      bool x = true;

      while (x == true) {
        delay(50);
        if (currentValue < threshold)
          x = false;
      }
    }

    else
      delay(20);  // wait 20ms between servo updates

    // ** LED Bar **
    if (servoposition > 85) {
      digitalWrite(ledPins[1], HIGH);  // Turn on LED 2-4
    }
    if (servoposition < 85) {
      digitalWrite(ledPins[1], LOW);  // Turn off LED 2-4
    }
    if (servoposition > 120) {
      digitalWrite(ledPins[2], HIGH);  // Turn on LED 5-7
    }
    if (servoposition < 120) {
      digitalWrite(ledPins[2], LOW);  // Turn off LED 5-7
    }
    if (servoposition > 150) {
      digitalWrite(ledPins[3], HIGH);  // Turn on LED 8-10
    }
    if (servoposition < 150) {
      digitalWrite(ledPins[3], LOW);  // Turn off LED 8-10
    }

    // ** Vibration Motor **
    // We should probably not map the lowest values the sensor can read
    // to a vibration intensity, since then the motors would vibrate all the time
    vibrationIntensity = map(flexposition, 700, 900, 0, 255);
    vibrationIntensity = constrain(flexposition, 0, 255);
    analogWrite(vibrationpin, vibrationIntensity);


    buttonState = digitalRead(buttonPin);  // Reads button state once again

    // ** Debugging **
    Serial.print(" sensor: ");
    Serial.print(flexposition);
    Serial.print("  servo: ");
    Serial.println(servoposition);
    Serial.print("  vibration:  ");
    Serial.println(vibrationIntensity);
    delay(20);
  }
}
