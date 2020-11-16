#include <Servo.h>

// Initialize servo objects to control the linear actuators
Servo servo1; // Linear Actuator 1
Servo servo2; // Linear Actuator 2
Servo servo3; // Linear Actuator 3

// Variables
const int buttonPin = 7; // Digital pin connected to pushbutton 
const int potPin = A0; // Analog pin connected to potentiometer 
const int myoPin = A5; // Analog pin connected to Myoelectic sensor 
const int analogOutPin = 3; // Analog output pin connected to output
const int servoPin1 = 9; // Input pin of linear actuator 1
const int servoPin2 = 10; // Input pin of linear actuator 2
const int servoPin3 = 11; // Input pin of linear actuator 3
int potValue = 0; // Value read from the potentiometer
int myoValue = 0; // Value read from the Myoelectic sensor
int buttonState = 0; // State of the button
int threshold = 700; // Threshold value of the Myoelectric sesnor


void reset()
{
  
  // Sets actuators to a default relaxed position
  servo1.write(myoValue);
  servo2.write(myoValue);
  servo3.write(myoValue);;
}

void setup()
{
  pinMode(buttonPin, INPUT); // Sets the digital pin 7 as input

  // Set each actuator to a pin
  servo1.attach(servoPin1); //  Actuator 1 to pin 9
  servo2.attach(servoPin2); // Actuator 2 to pin 10
  servo3.attach(servoPin3);  // Actuator 3 to pin 11


  Serial.begin(9600);
}

void loop()
{
 // buttonState = digitalRead(buttonPin); // Reads the state of the push button
 
    potValue = analogRead(potPin); // Reads the value of the potentiometer
    //Serial.println(potValue);

    if (potValue >= 50)
    {
        myoValue = analogRead(myoPin); // Reads the value of the potentiometer (value between 0 and 1023)
        
      if (myoValue >= threshold)
      {
        for (int i = 0; i < 360; i++)
        {
          servo1.write(i);
          servo2.write(i);
          servo3.write(i);
          delay(10);
        }

        delay(1000);
      }

      //Serial.println(val);

      if (myoValue < threshold)
      {
        for (int i = 360; i > 1; i--)
        {
          servo1.write(i);
          servo2.write(i);
          servo3.write(i);
          delay(10);
        }

        delay(1000);
      }
    }

    else
    {
      reset();
    }

}
