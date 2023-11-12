#include <Servo.h>// Arduino pin assignment// Add IR Sensor Definition Here !!!
#define PIN_IR 0
#define PIN_SERVO 10
#define PIN_LED 9
#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)
#define LOOP_INTERVAL 20  // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.1
#define _DIST_MIN 100.0
#define _DIST_MAX 250.0 
Servo myservo;
float dist_ema, dist_prev = _DIST_MAX; //unit: mm
unsigned long last_loop_time;   // unit: msec
void setup()
{
  pinMode(PIN_LED,OUTPUT);
  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(_DUTY_NEU);
  dist_prev = _DIST_MIN;  
  Serial.begin(2000000);
}void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float dist;  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;  
  // Remove Next line !!!
  //a_value = analogRead(PIN_POTENTIOMETER);
  // Read IR Sensor value !!!
  a_value = analogRead(PIN_IR);
  // Convert IR sensor value into distance !!!
  dist = (6762.0/(a_value-9)-4.0)*10.0-60.0;
  // we need distance range filter here !!!  
  if (dist <= _DIST_MIN) {
    digitalWrite(PIN_LED, 1);       // LED OFF
    dist = dist_prev;           // cut lower than minimum
  } else if (dist >= _DIST_MAX) {
    digitalWrite(PIN_LED, 1);       // LED OFF
    dist = dist_prev;           // Cut higher than maximum
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON
    dist_prev = dist;
  }  // we need EMA filter here !!!
  dist_ema = _EMA_ALPHA*dist + (1-_EMA_ALPHA)*dist_ema;  // map distance into duty
  // duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  duty = _DUTY_MIN + (dist_ema - _DIST_MIN)*((_DUTY_MAX - _DUTY_MIN)/(_DIST_MAX - _DIST_MIN));
  myservo.writeMicroseconds(duty);  
  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:");     Serial.print(_DIST_MIN);
  Serial.print(",IR:");     Serial.print(a_value);
  Serial.print(",dist:");   Serial.print(dist);
  Serial.print(",ema:");    Serial.print(dist_ema);
  Serial.print(",servo:");  Serial.print(duty);
  Serial.print(",MAX:");    Serial.print(_DIST_MAX);
  Serial.println("");
  }
