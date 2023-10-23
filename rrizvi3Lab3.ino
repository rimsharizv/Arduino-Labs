/*
   1- Author: Rimsha Rizvi - UIN: 659249234

   2- Lab: Lab 3 - Analog Signals

   3- Description: In your own words, what is this code supposed to do?

   4- Lab Questions:
        What contrast number did you pick for your LCD? 40
        What was the smallest potentiometer value you were able to beat the game on? 0
        What is the difference between an analog and PWM signal? 
          The difference between an analog signal and a pulse width modulation (PWM) signal is that an analog signal varies continuously and smoothly over time,
          whereas a PWM signal alternates between discrete voltage levels at a fixed frequency, with the ratio of on to off time dictating the overall signal value.

   5- References: Setting up breadboard: https://docs.arduino.cc/built-in-examples/basics/AnalogReadSerial, Mapping the potentiometer value in levels for speed: https://www.instructables.com/Motor-LCD-Arduino-Motor-Speed-Controller-With-Spee/, Lab 0 code for using mills()

   6- Demo: Demonstration uploaded to VoiceThread: https://uic.voicethread.com/lti-student/2321149/?tok=29857190563fd3291164dd0.27867098
*/



#include <LiquidCrystal.h>

// If you use a different pin for your wiring, make sure it is a PWM pin (~)
// and replace 6 with the pin you used.
#define CONTRAST_PIN 6

// MAKE SURE THESE PINS MATCH YOUR WIRING!!!
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char player = 0xFC;
int player1_pos = 0;
unsigned long previousMillis = 0;
int speed = 500; // initial time interval in milliseconds
bool direction = true; // true for forward, false for backward
unsigned long interval = 500;

const int BUTTON_1_PIN = 8;
const int ledPinG = 9;  // the number of the LED pin
const int ledPinR = 7;  // the number of the LED pin


void setup() {
  pinMode(CONTRAST_PIN, OUTPUT);
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinR, OUTPUT);

  lcd.begin(16, 2);

  // TODO: When you have a contrast value you want to try, put this line of code
  // into your setup function and replace <VALUE> with the contrast value that you picked (between 0 and 255).
  //
  analogWrite(CONTRAST_PIN, 40);
  Serial.begin(9600);
  

}


// This code will loop through every contrast value on an LCD screen
// while displaying a message, so that you can figure out what value
// to set the contrast on your display to.
void loop() {
  int sensorValue = analogRead(A0);
  // print out the value you read:


  // Calculate new speed based on potentiometer reading
  int speed = map(sensorValue, 0, 1023, 100, 1000);  // used a reference for this

  int button1_state = digitalRead(BUTTON_1_PIN);
  if (button1_state == HIGH) {
    delay(2000);  // pause once pressed, before showing the the state of the win
    lcd.clear();
    if (player1_pos == 6 || player1_pos == 7 || player1_pos == 8 || player1_pos == 9) { // corresponding block values in the line underneath
      lcd.print("You Won!");
      digitalWrite(ledPinG, HIGH);
      delay(5000);  // delay to show the state of the win before continue playing
    } else {
      lcd.print("You Lose!");
      digitalWrite(ledPinR, HIGH);
      delay(5000);
    }
  }
  digitalWrite(ledPinG, LOW);
  digitalWrite(ledPinR, LOW);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= speed) {
    previousMillis = currentMillis;

    // Clear screen and set cursor to player position
    lcd.clear();
    lcd.setCursor(player1_pos, 0);

    // Draw player character
    lcd.write(player);

    // Move player character forward or backward
    if (direction) {
      player1_pos++;
    } else {
      player1_pos--;
    }

    // Reverse direction when player reaches edge of screen
    if (player1_pos == 15) {
      direction = false;
    } else if (player1_pos == 0) {
      direction = true;
    }
  }

    // printing blocks in the middle bottom row of the LCD
  lcd.setCursor(6,1);
  for (int i = 0; i < 4; i++) {
    lcd.write(255);
  }
  
  Serial.println(sensorValue);
  delay(1);  // delay in between reads for stability
  
  // // Loop through all values (0 - 255)
  // for (int i = 0; i < 256; ++i) {
    
  //   // Set contrast
  //   analogWrite(CONTRAST_PIN, i);

  //   // Clear screen and print contrast value
  //   lcd.clear();
  //   lcd.setCursor(0, 0);
  //   lcd.print("Contrast ");
  //   lcd.print(i);

  //   // Delay so value is legible
  //   delay(500);
  // }

}
