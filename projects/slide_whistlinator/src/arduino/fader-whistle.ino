#include <Audio.h>
#include <CapacitiveSensor.h>
#include "Whistle.h"

#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <TimeLib.h>
#include <Wire.h>

#define mtof(x) 440 * pow(2, (x - 69.0) / 12)

const int FADER_POS_THRESHOLD = 20; // threshold for fader position
const int CAP_THRESHOLD = 10000; // threshold for fader position

#define C_MIDI 60
const int majorScale[] = {0, 2, 4, 5, 7, 9, 11, 12, 12, 14, 16, 17, 19, 21, 23, 24};

#define MIN_BREATH 90
#define MAX_BREATH 130

#define pThresh 400
#define pThresh2 880
static bool pressed = false;
//static int targetPosition;

const int pwmPinA = 13;  // PWM pin connected to L9110H INA
const int pwmPinB = 19;  // PWM pin connected to L9110H INB
const int faderPin = A8;

Whistle whistle;
AudioOutputI2S out;
AudioOutputUSB outUSB;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(whistle, 0, out, 0);
AudioConnection patchCord1(whistle, 1, out, 1);
AudioConnection patchCord2(whistle, 0, outUSB, 0);
AudioConnection patchCord3(whistle, 0, outUSB, 1);

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

void setup()
{
  AudioMemory(8);
  audioShield.enable();
  audioShield.volume(.6);

  //Serial.begin(9600);  // initializing serial port
  //cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  pinMode(A8, INPUT);

  analogWrite(pwmPinA, 0);
  analogWrite(pwmPinB, 0);
}

void loop()
{
  // Serial.print("Breath: "); 
  // Serial.println(analogRead(A0));
  // Serial.print("p0: ");
  // Serial.println(analogRead(A1));
  // Serial.print("p1: ");
  // Serial.println(analogRead(A2));
  // Serial.print("p2: ");
  // Serial.println(analogRead(A3));
  // Serial.print("p3: ");
  // Serial.println(analogRead(A4));
  // Serial.print("fader: ");
  // Serial.println(analogRead(faderPin));

  bool p0 = analogRead(A1) > pThresh;
  bool p1 = analogRead(A2) > pThresh;
  bool p2 = analogRead(A3) > pThresh;
  bool p3 = analogRead(A4) > pThresh2; 
  pressed = p0 || p1 || p2 || p3;

  int faderPos = analogRead(faderPin);
  int newPos = getNewPos(p0, p1, p2, p3);
  if (pressed) {
    if (abs(faderPos - newPos) > FADER_POS_THRESHOLD) {
      //Serial.println("UPDATE");
      //Serial.println(faderPos);
      //Serial.println(newPos);
      updatePosition(faderPos, newPos);
    }
  } else {
    motor(0);
  }

  // long total1 =  cs_4_2.capacitiveSensor(30);
  // if (total1 > 10000) {
  //   Serial.print("CAPACITIVE ");
  //   Serial.println(total1);
  // }

  // for (int i = 100; i < 1000; i+=100) {
  //   goToPosition(i);
  //   delay(250);
  // }
  
  float breath = mapToFloat(analogRead(A0), MIN_BREATH, MAX_BREATH);
  whistle.setParamValue("pressure", breath);
  // Serial.print("pressure: ");
  // Serial.println(breath);
  float midi = C_MIDI + (24*mapToFloat(analogRead(faderPin), 0, 1023));
  whistle.setParamValue("freq", mtof(midi));
  //Serial.print("midi: ");
  //Serial.println(midi);
  
  //delay(30);
}

int getNewPos(bool p0, bool p1, bool p2, bool p3) 
{
  int output = 0;
  // binary mapping
  if (p0) {
    output += 1;
  }
  if (p1) {
    output += 2;
  }
  if (p2) {
    output += 4;
  }
  if (p3) {
    output += 8;
  }

  return 1023 * majorScale[output] / 24;
}

void updatePosition(int faderPos, int newPosition) {
  if (faderPos > newPosition) {
    motor(smoothSpeed(faderPos - newPosition));
    //motor(230);
  }
  if (faderPos < newPosition) {
    motor(-smoothSpeed(newPosition - faderPos));
    //motor(-230);
  }

  // stop motor
  // motor(0);
  // Serial.print("curr: ");
  // Serial.println(faderPos);
}

void motor(int speed) {
  // Touch sensor (capacitance > threshold)
  long cap =  cs_4_2.capacitiveSensor(30);
  if (cap > CAP_THRESHOLD) {
    //Serial.print("Touch: "); 
    //Serial.println(total1);
    speed = 0;
  } 

  // Adjust motor speed/direction
  if (speed == 0) {
    analogWrite(pwmPinA, 0);
    analogWrite(pwmPinB, 0);
  } else if (speed > 0) {
    analogWrite(pwmPinA, speed);
    analogWrite(pwmPinB, 0);
  } else {
    analogWrite(pwmPinA, 0);
    analogWrite(pwmPinB, -speed);
  }
}

// if distance greater than 36, 255
// else logarithmically map to 210-255
int smoothSpeed(int input) {
  int smoothedValue = 255;
  if (input > 50) {
    smoothedValue = 255;
  } else {
    smoothedValue = 211 + 10 * mapToFloat(input, 0, 50);
  }
  Serial.print("smoothed: ");
  Serial.println(smoothedValue);
  return smoothedValue;
}

// map min and max and current to a 0-1 float
float mapToFloat(int current, int min, int max) {
  float output = (float)(current - min) / (max - min);
  output = constrain(output, 0.0, 1.0);
  return output;
}
