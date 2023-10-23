/*
   1- Author: Rimsha Rizvi - UIN: 659249234
   2- Lab: Lab 5 - Communication 
   3- Description: In your own words, what is this code supposed to do? 
   This code is running a game of Odd or Even using servo motor and LED to help us visualise the game better. The user will select a number and then select either odd or even, then a random number will be generated while the servo motor is moving. The sum of the user's number and the random number will be checked to see if it is odd or even. If the user guessed correct, then they won and the green LED will turn on, otherwise they lose and the red LED turns on.
   4-Lab Questions:
What function did you use to change the servo position? servo.write();
What are the angles of the servo motor for the three directions? My initial position (neutral) is 115, the position pointing to green LED (win) is 150, the position pointing to red LED (lose) is 105
What functions/methods did you use to take input? Serial.parseInt() and Serial.readStringUntil('\n')
   5- References: How to connect and run a servo motor: https://docs.arduino.cc/learn/electronics/servo-motors, how the functions of serial monitor works: https://www.programmingelectronics.com/serial-read/, reading an input value properly in serial: https://www.arduino.cc/reference/en/language/functions/communication/serial/readstringuntil/, checking and clearing out buffer: https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
   6- Demo: Demonstration uploaded to VoiceThread: https://uic.voicethread.com/lti-student/2386178/?tok=160988070064220d0a42c7e9.34275316
*/

#include <Servo.h>

// Create servo object to control a servo
Servo myServo;

// Define pins for the LEDs
const int greenLEDpin = 6;
const int redLEDpin = 4;

int inputVal;
bool isOdd;
String inputVal2;

void setup() {
  myServo.attach(9);

  // pin modes for the LEDs
  pinMode(greenLEDpin, OUTPUT);
  pinMode(redLEDpin, OUTPUT);

  Serial.begin(9600);

  // a welcome message
  Serial.println("Welcome to the even odd game!!");

  // move servo to initial position
  myServo.write(115);
}

void loop() {
  // turn off both LEDs at the beginning of each loop
  digitalWrite(greenLEDpin, LOW);
  digitalWrite(redLEDpin, LOW);

  // prompt user to enter a number between 1 and 5
  Serial.println("Enter a number between 1 and 5");
  delay(100); // delay for the user to read the comment

  // Wait for valid user input
  bool validInput = false;
  while (!validInput) {
    while (Serial.available() == 0);
    inputVal = Serial.parseInt();
    if (inputVal >= 1 && inputVal <= 5) {
      Serial.print("I received: ");
      Serial.println(inputVal);
      delay(30);
      validInput = true;
    } else{
      Serial.println("Wrong input!! Enter a number between 1 and 5");
      while (Serial.available() > 0) {
        Serial.read(); // discard any other input
      }
      delay(30);
    }
  }

  // Prompt user to choose odd or even
  Serial.println("Now choose odd or even! Type even or odd");
  delay(100); // Add a delay of 100ms to give time for the user to read the comment

  // Wait for valid user input
  bool validOption = false;
  while (!validOption) {
    while (Serial.available() == 0);

    inputVal2 = Serial.readStringUntil('\n');
    inputVal2.toLowerCase();
    if (inputVal2 == "even") {
      Serial.println("I received: even");
      isOdd = false;
      validOption = true;
    } else if (inputVal2 == "odd") {
      Serial.println("I received: odd");
      isOdd = true;
      validOption = true;
    } if(Serial.available() == 0) {
        if(inputVal2 != "even" && inputVal2 != "odd"){      
          Serial.println("Wrong input!! Enter 'even' or 'odd'");
          while (Serial.available() > 0) {
            Serial.read(); // discard any other input
          }
          delay(30);
        }
     }
  }
  // computer guessing a number now
  Serial.println("Generating...");

  for (int pos = 0; pos <= 180; pos += 1) {
    myServo.write(pos);
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);
    delay(15);
  }

  int rando = random(1, 6);
  Serial.print("My number: ");
  Serial.println(rando);

  // calculating the sum to determine even or odd and if the user's guess was correct
  int sum = rando + inputVal;
  Serial.print("Sum: ");
  Serial.println(sum);

  if (sum % 2 == isOdd) {
    Serial.println("You Won!");
    myServo.write(150);
    digitalWrite(greenLEDpin, HIGH);
  } else {
    Serial.println("You Lost!");
    myServo.write(105);
    digitalWrite(redLEDpin, HIGH);
  }
  delay(5000);
}