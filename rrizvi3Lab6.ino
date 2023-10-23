/*
   1- Author1: Shehriar Burney - UIN: 679754208
    - Author2: Rimsha Rizvi - UIN: 657249234
   2- Lab: Lab 6 - Serial Communication 
   3- Description: 
   a) Play a 2-player game where the goal is to win a race against the other. There are two buttons, with one LCD and one button on each arduino. The goal is to press the button faster than your opponent.
      The arduinos are connected using UART communication.
   b) Both Shehriar and Rimsha worked on the code and circuit building for the project. We use Shehriar's Lab 2 code as a reference to initially start the lab and began to build off from there. 
      Shehriar used the references and his previous lab to build the circuit initially using the components required. From there Rimsha focused on the game logic and the serial communication. Overall, we both tried to figure out the lab together and helped each other whenever the other person did not understand.
      Most of the lab also consisted of debugging our code/circuit building which we both did together.

   4- Serial Communication: We used UART communication in our lab. In UART, the TX (transmit) pin of arduino 1 is connected to the RX (receive) pin of arduino 2. And the TX pin of arduino 2 is connected to the RX pin of arduino 1.
          We then used the built-in Serial functions to communicate between arduinos by reading and writing bits between them. We also used Serial.begin() to start the serial communication and Serial.available() to check for inputs.


   5- References: To understand serial communication: https://iot-guider.com/arduino/serial-communication-between-two-arduino-boards/
                  To write data to LCD from serial port: https://forum.arduino.cc/t/print-data-from-serial-port-to-lcd/42534/4
                  Starter code: UIC CS 362 Lab 2 Shehriar Burney
                  To understand serial availability: UIC CS 362 Lab 5
   6- Demo: in-person demonstration on 04/10 at 3pm. TA: Amir
*/

// include the library code:
#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;

// Initialize button pins
const int button = 3;
const int buzzer = 6;
const int redLED = 4;
const int greenLED = 5;
int button1State;
int button2State;

// Initalize player characters and initial positions
const char player1Char = 'S';
int player1Pos = 0;

bool running = false;
bool gameStart = false;

bool oneOn = false;
bool twoOn = false;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  Serial.begin(9600);
  
  // Assign buttons as inputs
  pinMode(button, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  
  lcd.clear();
  player1Pos = 0;
  // Check if main menu screen should be displayed.
  // If the game has not started yet, display main menu.
  if(!gameStart){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Player One (S)");

    // lcd.setCursor(0, 1);
    // lcd.print("Player Two (R)");
  }

  button1State = digitalRead(button);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);

  if (Serial.available() > 0) {
    int value = Serial.read();
    if (value == 1) {
      button2State = 1;
      twoOn = true;
    } else {
      // lcd.setCursor(0, 0);
      // lcd.print("Ready, Waiting");
      // lcd.setCursor(0, 1);
      // lcd.print("for Player 2");
      button2State = 0;
    }
  }
  if (button1State) {
    oneOn = true;    
  }
  if (oneOn) {
    Serial.write(1);
    delay(100); // add a small delay to avoid flooding the UART connection

    if (twoOn == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ready, Waiting");
      lcd.setCursor(0, 1);
      lcd.print("for Player 2");
      delay(1000); // add a small delay to avoid flooding the UART connection
    }
  }
  
  // If both buttons are pressed, start the game.
  if(oneOn && twoOn){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("3");
    tone(buzzer, 261);
    delay(200);
    noTone(buzzer);
    delay(800);
    
    lcd.setCursor(0, 0);
    lcd.print("2");

    tone(buzzer, 261);
    delay(200);
    noTone(buzzer);
    delay(800);

    lcd.setCursor(0, 0);
    lcd.print("1");

    tone(buzzer, 261);
    delay(200);
    noTone(buzzer);
    delay(800);

    lcd.setCursor(0, 0);
    lcd.print("Go");
    tone(buzzer, 261);
    delay(1000);
    noTone(buzzer);
    
    lcd.clear();
    gameStart = true;
    running = true;
  }

  while(running){
    // Clear the screen
    lcd.clear();

    int button1State = digitalRead(button);
    
    // If button is not pressed, make oneOn and twoOn false. Used to stop users from holding the button.
    if(!button1State){
      oneOn = false;
    }

    // If the button is pressed, and the button was off before.
    if(button1State && !oneOn){
      // Checking to see if player won the game.
      if(player1Pos == 31){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 1 Wins!");
        digitalWrite(greenLED, HIGH);
        Serial.write(2);
        delay(5000);
        running = false;
        gameStart = false;
        oneOn = false;
        twoOn = false;
        while (Serial.available()) Serial.read();
        break;
      }
      // If player did not win, increase position by one and make oneOn true again.
      player1Pos ++;
      oneOn = true;
    }
    // Checking to see if player has made one lap, print #
    if(player1Pos >= 16){
        lcd.setCursor(15, 0);
        lcd.print("#");
    }

    // Print new position of player on LCD.
    lcd.setCursor(player1Pos%16, 0);
    lcd.print(player1Char);

    if (Serial.available() > 0) {
      Serial.write(1);
      int num =  Serial.read();
      Serial.write(player1Pos);
      Serial.write(player1Char);

      if (num == 2) {
        lcd.clear();
        lcd.print("Player 2 Wins!");
        digitalWrite(redLED, HIGH);
        delay(5000);
        running = false;
        gameStart = false;
        oneOn = false;
        twoOn = false;
        while (Serial.available()) Serial.read();
        break;
      } else if (num == 1) {
          int player2Pos = Serial.read();
          char message = Serial.read();
          if (player2Pos >= 16) {
            lcd.setCursor(15, 1);
            lcd.print("#");
          }
          lcd.setCursor(player2Pos % 16, 1);
          lcd.print(message);
      }
    }
    delay(100);
  }
  // print the number of seconds since reset:
}
