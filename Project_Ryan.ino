#include <Servo.h>

// Initialize servo objects to control the linear actuators
Servo servo1; // Linear Actuator 1
Servo servo2; // Linear Actuator 2
Servo servo3; // Linear Actuator 3

// Initialize Pins
const int buttonPin = 7; // Pushbutton connected to digital pin 7
const int potPin = A0; // Potentiometer connected to analog pin A0
const int myoPin = 5; // Myoelectic sensor connectected to digital pin 5
const int analogOutPin = 3; // Analog output pin that the LED is attached to
const int servoPin1 = 9; // Input pin of linear actuator 1
const int servoPin2 = 10; // Input pin of linear actuator 2
const int servoPin3 = 11; // Input pin of linear actuator 3


// Initialize Variables
int potValue; // Value read from the potentiometer
int myoValue; // Value read from the Myoelectic sensor
int outputValue; // Value output to the PWM (analog out)
int buttonState; // State of the button
int state;

void reset()
{
  // Sets actuators to a default relaxed position
  myoValue = 1023;
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
  buttonState = digitalRead(buttonPin); // Reads the state of the push button

  if (buttonState)
  {
    potValue = analogRead(potPin); // Reads the value of the potentiometer
    //Serial.println(potValue);

    if (potValue >= 0)
    {
      myoValue = analogRead(myoPin); // Reads the value of the potentiometer (value between 0 and 1023)

      if (myoValue >= 700 && state == 0)
      {
        for (int i = 0; i < 360; i++)
        {
          servo1.write(i);
          servo2.write(i);
          servo3.write(i);
          //servo1.write(360-i);
          //servo2.write(360-i);
          //servo3.write(360-i);
          delay(10);
        }

        delay(1000);
        state = 1;
      }

      //Serial.println(val);

      if (myoValue >= 700 && state == 1)
      {
        outputValue = map(myoValue, 0, 1023, 0, 255);
        analogWrite(analogOutPin, outputValue);

        for (int i = 360; i > 1; i--)
        {
          servo1.write(i);
          servo2.write(i);
          servo3.write(i);
          //servo1.write(1+i);
          //servo2.write(1+i);
          //servo3.write(1+i);
          delay(10);
        }

        delay(1000);
        state = 0;
        //if (analogRead(myo) >= 800)
        //{
        //  state = 0;
        // }  //}
        // state = 0;

      }
      Serial.println(state);

    }

    else
    {
      reset();
    }

    //Serial.println(myoValue);
  }
}
