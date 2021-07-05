#include <Servo.h>
//initialize the actuators as servos
Servo servo1;
Servo servo2;
//label what pins they are connected to
const int flexPin = A0;
const int servoPin1 = 4;
const int servoPin2 = 3;
//servo pin position
int servoPosition;
//int servo1Pos;
//int servo2Pos;
//values from flex sensor
int value, value1, value2, value3, value4, avgValue;
// Weights for values in the filter
int weight, weight1, weight2, weight3, weight4;
int threshold = 50;
int state = 0;
void setup()
{
  Serial.begin(9600);
  servo1.attach(servoPin1); //actuator to pin 4
  servo2.attach(servoPin2); //actuator to pin 3
  // Values for averaging filter
  value = 0; value1 = 0; value2 = 0; value3 = 0; value4 = 0;
  // If we want to do a weighted average filter
  weight = 1; weight1 = 1; weight2 = 1; weight3 = 1; weight4 = 1;
  // put your setup code here, to run once:
}
void loop()
{
  // 5-step averaging filter
  // The number of values and their weights are tunable parameters
  value4 = value3;
  value3 = value2;
  value2 = value1;
  value1 = value;
  value = analogRead(flexPin);
  avgValue = (value * weight + value1 * weight1 + value2 * weight2 + value3 * weight3 + value4 * weight4) / 5;
  value = map(value, 700, 900, 0, 100);

  // The averaging filter will not be accurate if there is significant delay in the overall loop.
  // In general you don't want to have significant delays in the loop if you can avoid it.
  // Since this is a single-threaded device (I think) and multithreading is hard (I know),
  // we can only update our sensor readings and command signals as fast as the loop function
  // cycles. No need for extra for loops in here.
  // if avgValue >= threshold, set state to 1
  // if state == 1, begin hand close procedure:
  //   if servoPosition > 1, servoPosition--; servo.write(servoPosition); // We could probably make this decrease faster than once per 10 ms
  //   else if avgValue < threshold, set state to 0;
  // else if state==0, begin hand open procedure:
  //   if servoPosition < 360, servo.write(servoPosition); servoPosition++; // We could probably make this increase faster than once per 10 ms
  // delay(10)
  // Doing the structure this way *should* make it so you can use the averaging filter.

  // Refactored code
  // To do:
  //  Split logic to move fingers independently if we can hook up more sensors
  if (avgValue >= threshold)
  {
    state = 1;
  }
  if (state == 1)
  {
    if (servoPosition > 1)
    {
      servoPosition--;
      servo1.write(servoPosition);
      servo2.write(servoPosition);
    }
    else if (avgValue < threshold)
    {
      state = 0;
    }
  }
  else if (state ==0)
  {
    if (servoPosition < 360)
    {
      servoPosition++;
      servo1.write(servoPosition);
      servo2.write(servoPosition);
    }
  }
  Serial.print("Servo Position: ");
  Serial.println(servoPosition);
  Serial.print("Sensor Values [value value1 value2 value3 value4]: [");
  Serial.print(value);
  Serial.print(" ");
  Serial.print(value1);
  Serial.print(" ");
  Serial.print(value2);
  Serial.print(" ");
  Serial.print(value3);
  Serial.print(" ");
  Serial.print(value4);
  Serial.println("]"); // I forget if println puts the newline at the beginning or end, so might need to reformat
  delay(10); // The only delay in the loop
}
