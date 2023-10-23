/*
   1- Author: Rimsha Rizvi - UIN: 659249234
   2- Lab: Lab 2 - The LCD 
   3- Description: Implementing a simple two-player racing game using toggle-buttons and LCD. The player's have to finish two laps using the toggle buttons and the first to finish wins the race. This is presented on an LCD.
   4- LCD Pins: List what each of the following LCD pins are for, i.e. “D4: Data pin 4”. (Read the prelab)
     VSS(GND): 10k potentiometer to Arduino UNO GND 
     VDD(+5V): 10k potentiometer to Arduino UNO 5V
     V0: Connected to 10k potentiometer output to control display contrast pin on LCD 
     RS: Pin 12
     RW: Arduino UNO GND
     E(Enable): Pin 11
     D4: Data pin 5
     D5: Data pin 4
     D6: Data pin 3
     D7: Data pin 2
     A(BKlt+): Connected to the 220 ohm resistor which is connected to 5V power
     K(BKlt-): Connected to Arduino UNO GND
   5- References: Figuring out the LCD on Arduino using 'Hello World' example from https://docs.arduino.cc/learn/electronics/lcd-displays
   6- Demo: Demonstration uploaded to VoiceThread: https://uic.voicethread.com/lti-student/2208837/?tok=7929377263f3f71ee5ba88.82734768
*/

#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD pin
const int RS_PIN = 12, EN_PIN = 11, D4_PIN = 5, D5_PIN = 4, D6_PIN = 3, D7_PIN = 2;

// Initialize button pins
const int BUTTON_1_PIN = 6;
const int BUTTON_2_PIN = 7;

const int max_overall = 31;
const int max_lap = 16;

// Initalize player characters and initial positions
const char P1_DISP_CHAR = '1';
const char P2_DISP_CHAR = '2';
int player1_pos = 0;
int player2_pos = 0;

bool running = false;
bool game_start = false;

bool p1_button_on = false;
bool p2_button_on = false;

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Assign buttons as inputs
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
}

bool decide_winner(int p1_pos, int p2_pos) {
  if (p1_pos >= max_overall) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player 1 Wins!");
    return true;
  } else if (p2_pos >= max_overall) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Player 2 Wins!");
    return true;
  } else {
    return false;
  }
}

void update_position(int& pos, const int button_pin, bool& button_on, const char disp_char) {
  int button_state = digitalRead(button_pin);

  // If button is not pressed, make button_on false. Used to stop users from holding the button.
  if (!button_state) {
    button_on = false;
  }

  // If the button is pressed, and the button was off before.
  if (button_state && !button_on) {
    // If player did not win, increase position by one and make button_on true again.
    pos++;
    button_on = true;
  }

  // Checking to see if player has made one lap
  if (pos >= max_lap) {
    lcd.setCursor(15, (disp_char == P1_DISP_CHAR) ? 0 : 1);
    lcd.print("#");
  }

  // new pos
  lcd.setCursor(pos % max_lap, (disp_char == P1_DISP_CHAR) ? 0 : 1);
  lcd.print(disp_char);
}

void loop() {
  // Check if main menu screen should be displayed.
  // If the game has not started yet, display main menu.
  if (!game_start) {
    lcd.setCursor(0, 0);
    lcd.print("Player One (1)");

    lcd.setCursor(0, 1);
    lcd.print("Player Two (2)");
  }

  int button1_state = digitalRead(BUTTON_1_PIN);
  int button2_state = digitalRead(BUTTON_2_PIN);

  // If both buttons are pressed, start the game.
  if (button1_state && button2_state) {
    game_start = true;
    running = true;
  }

  while (running) {
    // Clear the screen
    lcd.clear();

    update_position(player1_pos, BUTTON_1_PIN, p1_button_on, P1_DISP_CHAR);
    update_position(player2_pos, BUTTON_2_PIN, p2_button_on, P2_DISP_CHAR);
    if (decide_winner(player1_pos, player2_pos)) {
      running = false;
      game_start = false;
      player1_pos = 0;
      player2_pos = 0;
      delay(2000);
    }

    // Add delay to prevent the game from moving too fast.
    delay(100);
  }
}
