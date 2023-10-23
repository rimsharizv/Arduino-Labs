/*
   1- Author: Rimsha Rizvi - UIN : 659249234
   2- Lab: Lab 1 - Toggle Button
   3- Description: A loop function used to explain the basic concept of the AND Logic Gate using three LED's and two push-buttons that act like a toggle button.
   4- PIN numbers: Pin 5 should be used to connect to the Red LED which is an output component
                   Pin 6 should be used to connect to one of the Green LED which is an output component
                   Pin 7 should be used to connect to one of the Green LED which is an output component
                   Pin 12 should be used to connect the one of the buttons (right) which is an input component
                   Pin 13 should be used to connect the one of the buttons (left) which is an input component

   5- References: Used the Lab 1 Guideline PDF: https://docs.google.com/document/d/1tXWhQ_uEueWgSO5-5Im3AKF-ko3Cv3zeNsiXLWLmbd4/edit, https://www.youtube.com/watch?v=VPGRqML_v0w&ab_channel=educ8s.tv.
   6- Demo: Demonstration uploaded to VoiceThread: https://uic.voicethread.com/lti-student/2054467/?tok=198170905163e17a814ebc31.04202072
*/

// connection of breadboard to the Arduino PINs
const int leftButtonPin = 13;
const int rightButtonPin = 12;

const int GreenLED1Pin = 7;
const int GreenLED2Pin = 6;
const int RedLEDPin = 5;

// initialising the states of the button
int leftState = LOW;
int rightState = LOW;

void setup() {  // code set up
  // the buttons are INPUT
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);

  // the LED's are OUTPUT
  pinMode(GreenLED1Pin, OUTPUT);
  pinMode(GreenLED2Pin, OUTPUT);
  pinMode(RedLEDPin, OUTPUT);
}

void loop() {
  // read the state of the buttons
  leftState = digitalRead(leftButtonPin);
  rightState = digitalRead(rightButtonPin);

  // To ensure the push button acts like a toggle button -

  if(leftState == HIGH){  // first check if button is pushed
    if(digitalRead(GreenLED1Pin) == HIGH) {  // then check if LED is ON
      digitalWrite(GreenLED1Pin, LOW);  // if button is pushed, and LED is ON, leave LED ON
    }
    else{
      digitalWrite(GreenLED1Pin, HIGH);
    }
  }
  if(rightState == HIGH) {  // first check if button is pushed
    if(digitalRead(GreenLED2Pin) == HIGH) {  // then check if LED is ON
      digitalWrite(GreenLED2Pin, LOW);  // if button is pushed, and LED is ON, leave LED ON
    }
    else{
      digitalWrite(GreenLED2Pin, HIGH);
    }
  }
  if((digitalRead(GreenLED1Pin) == HIGH) && (digitalRead(GreenLED2Pin) == HIGH)) {  // if both LED is ON, only then turn the Red LED ON (AND Gate)
    digitalWrite(RedLEDPin, HIGH);
  } else {
    digitalWrite(RedLEDPin, LOW);
  }

}
