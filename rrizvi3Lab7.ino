/*
   1- Author: Rimsha Rizvi - 659249234
   2- Lab: Lab 7 - Interrupts 
   3- Description: In this lab, we create a game with a Seven-segment display that cycles through numbers 0-9, a random number is displayed in the serial monitor. A push button with interrupts is used to detect when it's pressed. If pressed on the correct number, a "Congratulations!" message is printed and the game restarts. Otherwise, an "Oops!" message is printed, and the game continues
   4-Lab Questions:
  What is the purpose of a current-limiting resistor in a seven-segment display circuit? It reduces the level of current flow into the display. This helps keep the overall circuit and equipment safe.
  Can you display special characters? If yes, How? When using a seven-segment display, you can only create special characters that can be displayed using the segments available (a, b, c, d, e, f, g). For instance, characters like '[' can be displayed, but characters like '$' cannot be displayed on a standard seven-segment display.
  What are some common applications of seven-segment displays? Seven-segment displays are used in situations where numeric or alphanumeric information needs to be shown in a clear and easily identifiable manner. Commonly used in various applications such as digital clocks, digital meters, timers, scoreboards, appliances, etc
   5- References: Understanding the seven-segment display and implementation of it: https://lastminuteengineers.com/seven-segment-arduino-tutorial/
                  Understanding the concept of interrupts: https://www.youtube.com/watch?v=9VZUb5cMrV0
   6- Demo: Demonstration uploaded to VoiceThread: https://uic.voicethread.com/lti-student/2523331/?tok=16972950966446d115738013.34770169
*/

#include "SevSeg.h"
SevSeg sevseg;

int button = 2;

int numPressed;
int gameLoop = 1;
int paused = 0;
long unsigned int lastPress;
int debounceTime = 100;
int randNumber;

// void setup()
// {
// 	//Set to 1 for single digit display
// 	byte numDigits = 1;

// 	//defines common pins while using multi-digit display. Left empty as we have a single digit display
// 	byte digitPins[] = {};

// 	//Defines arduino pin connections in order: A, B, C, D, E, F, G, DP
// 	byte segmentPins[] = {3, 10, 8, 7, 6, 4, 5, 9};
// 	bool resistorsOnSegments = true;

// 	//Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
// 	//sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
// 	sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);

// 	sevseg.setBrightness(90);
// }

// void loop()
// { 
//    //Display numbers one by one with 2 seconds delay
//    for(int i = 0; i < 10; i++)
//    {
//      sevseg.setNumber(i);
//      sevseg.refreshDisplay(); 
//      delay(2000);
//    }
// }

void setup() 
{
  //Set to 1 for single digit display
  byte numDigits = 1;
  // defines common pins while using multi-digit display. Left empty as we have a single digit display
  byte digitPins[] = {};

  //Defines arduino pin connections in order: A, B, C, D, E, F, G, DP
  byte segmentPins[] = {3, 10, 8, 7, 6, 4, 5, 9};
  bool resistorsOnSegments = true;

  //Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
  //sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);

  sevseg.setBrightness(90);
  Serial.begin(9600);

  pinMode(button, INPUT);

  attachInterrupt(digitalPinToInterrupt(button), buttonISR, RISING);
} 

void loop()
{
  sevseg.refreshDisplay();
  randNumber = random(0, 9);
  gameLoop = 1;
  paused = 0;

  while (gameLoop == 1)
  {
    sevseg.refreshDisplay();
    Serial.print("The selected random Number is : ");
    Serial.println(randNumber);
    for (int currentNum = 0; currentNum < 10; currentNum++)
    {
      sevseg.refreshDisplay();
      sevseg.setNumber(currentNum);
      numPressed = currentNum;
      if (gameLoop == 0)
      {
        sevseg.refreshDisplay();
        break;
      }
      if (paused == 1)
      {
        sevseg.refreshDisplay();
        delay(500);
        sevseg.setNumber(0);
        delay(500);
        gameLoop = 0;
        paused = 0;
        break;
      }
      delay(500);
    }
    if (gameLoop == 0)
    {
      sevseg.refreshDisplay();
      break;
    }
    if (paused == 0)
    {
      sevseg.refreshDisplay();
      sevseg.setNumber(0);
    }
  }
  delay(5000);
}


void buttonISR(){
  if (millis() - lastPress >= debounceTime) {
    lastPress = millis();
    Serial.println("Button pressed");
    Serial.println(numPressed);
    sevseg.refreshDisplay(); 
    if(randNumber == numPressed){
      Serial.println("Congratulations! You pressed the correct number.");
      Serial.println("Pausing game...");
      sevseg.refreshDisplay(); 
      delay(500);
      gameLoop = 0;
      paused = 1;
    }
    else{
      Serial.println("Oops! You missed.");
      delay(500);
      sevseg.refreshDisplay();
    }
  }
}

void startNewGame(){
  sevseg.refreshDisplay();
  randNumber = random(0, 9);
  gameLoop = 1;
  paused = 0;
}