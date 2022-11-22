/* 
THIS CODE WAS ORIGINALLY CREATED BY INCOGNITO OF T05-C 
MEMBERS: 
LANDERO, ELLA MAE F.
PABUAYA, JOJIE ANN B.
PALENCIA, MERILY R.
VILLAVERDE, KARL M.
*/


#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "pitches.h"

#define NOTE_D3 147
#define NOTE_D5 587
#define NOTE_D_black_5 622
#define NOTE_A_black_5 830
#define NOTE_A5 880
#define rest 10

Servo p;

long timestamp;
int numRand, hardClue1 = 0, hardClue2 = 0, seconds = 0, minutes = 0, prevsec=0, prevmin=0;
String category = "";
unsigned long timecount;

byte arrowkeys[] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000
};
byte upkeys[] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte downkeys[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000
};

// LIST OF  WORDS OF EASY LEVEL
String EasyWords[60] = {
  "DIODE", "CAPACITOR", "RESISTOR", "SWITCHES", "FUSE", "TRANSISTOR", "BATTERY", "SENSOR", "DIAC", "TRIAC",  // COMPONENTS
  "SETUP", "LOOP", "ARRAY", "PINMODE", "DIGITALREAD", "DELAY", "BYTE", "LONG", "DIGITALWRITE", "VARIABLES",  // CODES
  "COMPUTER", "RADIO", "IRON", "AMPLIFIER", "LAPTOP", "SCANNER", "CAMERA", "SPEAKER", "TIMER", "PRINTER",    //DEVICES
  "OHMS", "VOLTS", "WATTS", "AMPERE", "HERTZ", "FARAD", "MHO", "PPM", "JOULE", "LUMEN",                      // UNITS
};

// LIST OF  WORDS OF HARD LEVEL
String HardWords[60] = {
  "SERVO MOTOR", "THYRISTOR", "TRANSFORMER", "ZENER DIODE", "TOGGLE SWITCH", "POTENTIOMETER", "DC MOTORS", "SEMICONDUCTOR", "REGULATOR", "SOLENOIDS",  // COMPONENTS
  "FUNCTION", "STRING", "CHAR", "MILLIS", "ANALOGPIN", "STRUCTURE", "ANALOGREAD", "CURLY BRACES", "DATATYPES", "COMMENTS",                             // CODES
  "STOPWATCH", "TELEVISION", "SMARTPHONE", "MICROPHONE", "PROJECTOR", "MICROWAVE", "REFRIGERATOR", "WEBCAM", "DASHCAM", "CCTV",                        //DEVICES
  "DECIBEL", "PASCAL", "COULOMB", "HENRY", "TESLA", "CANDELA", "GAUSS", "NEWTON", "ECQUEREL", "WEBER"                                                  //UNITS
};

int notes[] = {
  NOTE_D3,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  NOTE_D5,
  0,
  NOTE_D_black_5,
  0,
  NOTE_A5,
  0,
  NOTE_D5,
  0,
  NOTE_D_black_5,
  0,
  NOTE_A5,
  0,
};

int durations[] = {
  200,
  70,
  100,
  80,
  100,
  80,
  400,
  rest,
  200,
  100,
  80,
  100,
  80,
  400,
  70,
  200,
  rest,
  125,
  rest,
  175,
  rest,
  175,
  rest,
  175,
  rest,
  175,
  rest,
  175,
  rest,
  50,
  5,
};

String Hashwords = "";  // memory of encrypted random word
String words = "";      //memory of the random word

int life = 3;              //life of the player
int light = 24;            // for the light pin
const int up = 2;          // for the up button
const int down = 3;        // for the down button
const int enter = 4;       // for the enter button
const int servoPin = 5;    // for the servo pin
const int speakerPin = 6;  // for the speaker pin

LiquidCrystal_I2C lcd(0x27, 0, 4);

// Setup and configuration of the inputs as well as the output pins
void setup() {
  // Setup Servo
  Trap.attach(servoPin);
  Trap.write(90);


  // Clue setup
  hardClue1 = random(words.length());
  hardClue2 = random(words.length());
  // serial setup for troubleshooting
  Serial.begin(9600);

  // pin set-up
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  for (int x = 22; x <= light; x++) {
    pinMode(x, OUTPUT);
    digitalWrite(x, HIGH);
  }

  // Lcd set-up
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // lcd display of custom character
  lcd.createChar(0, arrowkeys);
  lcd.createChar(1, upkeys);
  lcd.createChar(2, downkeys);
  // begin of the display of our group name
  lcd.setCursor(5, 1);
  lcd.print("INCOGNITO");
  delay(1000);
  lcd.clear();

  lcd.setCursor(6, 1);
  lcd.print("HANGMAN");
  delay(1000);
  lcd.clear();
  play();
}


// function that will generates a sounds every time user will select a button
void clickEffects() {
  digitalWrite(speakerPin, HIGH);
  delay(100);
  digitalWrite(speakerPin, LOW);
}

//function that would let user choice a level of difficulty
void intro() {
  int select = 0;
  String line1 = "<", line2 = "";
  bool parLevel = false;
  int numSelect = 0;

  do {
    if (digitalRead(enter) == HIGH && numSelect == 0) {
      clickEffects();
      words = EasyWords[numRand];
      parLevel = true;
    }
    if (digitalRead(enter) == HIGH && numSelect == 1) {
      clickEffects();
      words = HardWords[numRand];
      parLevel = true;
    }

    if (digitalRead(up) == HIGH) {
      numSelect++;
      clickEffects();
      if (numSelect > 1) {
        numSelect = 0;
      }
      if (numSelect == 0) {
        line1 = "<";
        line2 = " ";
      }
      if (numSelect == 1) {
        line1 = " ";
        line2 = "<";
      }
    }
    if (digitalRead(down) == HIGH) {
      numSelect--;
      clickEffects();
      if (numSelect < 0) {
        numSelect = 1;
      }
      if (numSelect == 0) {
        line1 = "<";
        line2 = " ";
      }
      if (numSelect == 1) {
        line1 = " ";
        line2 = "<";
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("SELECT A LEVEL:");
    lcd.setCursor(0, 1);
    lcd.print("EASY " + line1);
    lcd.setCursor(0, 2);
    lcd.print("HARD " + line2);
    delay(50);
  } while (parLevel == false);
  encrypt(numSelect);
}

// main program of our game HANGMAN
void start() {
  int count = 0;
  int numLetter = 65;
  char Letter = numLetter;  //char letter 65-90
  int error;
  const int totalNotes = sizeof(notes) / sizeof(int);
  float wait = 1;
  int keys = 0;
  delay(300);
  for (int i = 5; i != 0; i--) {
    lcd.setCursor(2, 0);
    lcd.print("GUESS THE WORDS");
    lcd.setCursor(3, 1);
    lcd.print(" IN ");
    lcd.print(i);
    lcd.print(" SECONDS");
    clickEffects();
    delay(1000);
  }
  digitalWrite(speakerPin, HIGH);
  delay(300);
  digitalWrite(speakerPin, LOW);
  delay(1000);
  lcd.clear();
  do {
    for (int i = 0; i < totalNotes; i++) {
      const int currentNote = notes[i];
      if (i <= 20) {
        wait = durations[i] / 1.5;
      } else {
        wait = durations[i] / 0.8;
      }
      if (currentNote != 0) {
        tone(speakerPin, notes[i] - 25, wait);
      } else {
        noTone(speakerPin);
      }

      timecount = millis();
      if (timecount - timestamp > 1000) {
        timestamp = timecount;
        seconds += 1;
        if (seconds >= 60) {
          seconds = 0;
          minutes += 1;
        }
      }

      lcd.setCursor(0, 0);
      lcd.print("SELECT");
      lcd.write(keys);
      lcd.print(Letter);
      lcd.setCursor(13, 0);
      lcd.print("LIFE: ");
      lcd.print(life);
      lcd.setCursor(5, 1);
      lcd.print("TIME: ");
      lcd.print(minutes);
      lcd.print(":");
      lcd.print(seconds);
      lcd.setCursor(0, 2);
      lcd.print(Hashwords);
      lcd.setCursor(0, 3);
      lcd.print("CATEGORY: ");
      lcd.print(category);
      if (numRand >= -1 && numRand <= 9) {
        category = "COMPONENTS";
      } else if (numRand >= 10 && numRand <= 19) {
        category = "CODES";
      } else if (numRand >= 20 && numRand <= 29) {
        category = "DEVICES";
      } else if (numRand >= 30 && numRand <= 39) {
        category = "UNITS";
      }

      if (digitalRead(up) == HIGH && numLetter < 91) {
        numLetter += 1;
        keys = 1;
        clickEffects();
        if (numLetter == 91) {
          numLetter = 65;
        }
        Letter = numLetter;
      }

      if (digitalRead(down) == HIGH && numLetter > 64) {
        numLetter -= 1;
        keys = 2;
        clickEffects();
        if (numLetter == 64) {
          numLetter = 90;
        }
        Letter = numLetter;
      }

      if (digitalRead(enter) == HIGH) {
        for (int i = 0; i < words.length(); i++) {
          if (Letter == words[i]) {
            Hashwords[i] = Letter;
          } else {
            error++;
          }
        }

        if (words == Hashwords) {
          lcd.setCursor(0, 2);
          lcd.print(words);
          if (minutes <= prevmin && seconds <= prevsec || prevmin == 0 &&  prevsec==0 || minutes == prevmin && seconds <= prevsec) {
            prevsec = seconds;
            prevmin = minutes;
            lcd.setCursor(0, 3);
            lcd.print("YOU ARE THE FASTEST!");
          }
          delay(3000);
          winner();
        }

        if (error == words.length()) {
          life -= 1;
          if (life == 2) {
            digitalWrite(light, LOW);
          }
          if (life == 1) {
            digitalWrite(light - 1, LOW);
          }
          if (life == 0) {
            digitalWrite(light - 2, LOW);
            lcd.setCursor(0, 2);
            lcd.print(words);
            delay(500);
            lcd.setCursor(0, 2);
            lcd.print(Hashwords);
            delay(500);
            lcd.setCursor(0, 2);
            lcd.print(words);
            delay(250);
            lcd.setCursor(0, 2);
            lcd.print(Hashwords);
            delay(250);
            lcd.setCursor(0, 2);
            lcd.print(words);
            break;
          }
          clickEffects();
          clickEffects();
        }
        count = 0;
        error = 0;
      }
    }
  } while (life > 0);
}

// encryption of the random chosen words
int encrypt(int numSelect) {
  int easyClue = random(words.length());
  int hardClue = random(words.length());
  while (hardClue == easyClue) {
    hardClue = random(words.length());
  }
  for (int i = 0; i < words.length(); i++) {
    Hashwords += "_";
    if (words[i] == ' ') {
      Hashwords[i] = ' ';
    }
    if (i == easyClue) {
      Hashwords[easyClue] = words[easyClue];
    }

    if (i == hardClue && numSelect == 1 || words.length() > 5) {
      Hashwords[hardClue] = words[hardClue];
    }
  }
}

// this function will run if player will win the game
void winner() {
  int x = 0;
  do {
    lcd.clear();
    for (int y = 0; y != 2; y++) {
      lcd.clear();
      lcd.setCursor(x + 3, y);
      lcd.print("YOU WIN!");
      lcd.setCursor(x, y + 1);
      lcd.print("ENJOY YOUR LIFE");
      delay(150);
    }
    for (int y = 2; y != 0; y--) {
      lcd.clear();
      lcd.setCursor(x + 3, y);
      lcd.print("YOU WIN!");
      lcd.setCursor(x, y + 1);
      lcd.print("ENJOY YOUR LIFE");
      delay(150);
    }
    x++;
    if (x == 6) {
      lcd.clear();
      menu();
    }
  } while (true);
}

//this function will run if player will loose the game
void fail() {
  bool con = true;
  bool servoCheck = false;
  int x = 0;
  do {
    if (servoCheck == false) {
      Trap.write(180);
      servoCheck = true;
    }
    lcd.clear();
    for (int y = 0; y != 2; y++) {
      lcd.clear();
      lcd.setCursor(x + 2, y);
      lcd.print("GAME OVER!");
      lcd.setCursor(x, y + 1);
      lcd.print("ANY LAST WORD?");
      delay(150);
    }
    for (int y = 2; y != 0; y--) {
      lcd.clear();
      lcd.setCursor(x + 2, y);
      lcd.print("GAME OVER!");
      lcd.setCursor(x, y + 1);
      lcd.print("ANY LAST WORD?");
      delay(150);
    }
    x++;
    if (x == 7) {
      lcd.clear();
      menu();
    }
  } while (true);
}
void menu() {
  int melodyend[] = {
    NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5,
    NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5,
    NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
  };

  // The note duration, 8 = 8th note, 4 = quarter note, etc.
  int durationsend[] = {
    8, 8, 8, 4, 4, 4,
    4, 8, 8, 8, 8, 8,
    8, 8, 8, 4, 4, 4,
    4, 8, 8, 8, 8, 8
  };
  int songLength = sizeof(melodyend) / sizeof(melodyend[0]);
  int select = 0;
  String line1 = "<", line2 = "";
  bool parLevel = false;
  int numSelect = 0;
  bool con = true;
  do {
    for (int thisNote = 0; thisNote < songLength; thisNote++) {
      int durationend = 1000 / durationsend[thisNote];
      tone(6, melodyend[thisNote], durationend);
      int pause = durationend * 1.3;
      delay(pause);
      noTone(8);
      if (digitalRead(enter) == HIGH && numSelect == 0) {
        con = false;
        numRand;
        hardClue1 = 0;
        hardClue2 = 0;
        category = "";
        Hashwords = "";
        words = "";
        life = 3;
        setup();
        loop();
      }
      if (digitalRead(enter) == HIGH && numSelect == 1) {
        lcd.clear();
        lcd.setCursor(1, 1);
        lcd.print("THANKS FOR PLAYING");
        lcd.setCursor(6, 2);
        lcd.print("HANGMAN");
        delay(3000);
        end();
      }

      if (digitalRead(up) == HIGH) {
        numSelect++;
        clickEffects();
        if (numSelect > 1) {
          numSelect = 0;
        }
        if (numSelect == 0) {
          line1 = "<";
          line2 = " ";
        }
        if (numSelect == 1) {
          line1 = " ";
          line2 = "<";
        }
      }
      if (digitalRead(down) == HIGH) {
        numSelect--;
        clickEffects();
        if (numSelect < 0) {
          numSelect = 1;
        }
        if (numSelect == 0) {
          line1 = "<";
          line2 = " ";
        }
        if (numSelect == 1) {
          line1 = " ";
          line2 = "<";
        }
      }
      lcd.setCursor(7, 0);
      lcd.print("MENU:");
      lcd.setCursor(7, 1);
      lcd.print("RETRY " + line1);
      lcd.setCursor(7, 2);
      lcd.print("EXIT  " + line2);
      lcd.setCursor(0, 3);
      lcd.print("TIME TO BEAT: ");
      lcd.print(prevmin);
      lcd.print(":");
      lcd.print(prevsec);
    }
  } while (con == true);
}

// ENDING OF THE GAME WILL FLASH THE MEMBERS OF THE GROUP
void end() {
  int melodyend[] = {
    NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5,
    NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5,
    NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
  };

  // The note duration, 8 = 8th note, 4 = quarter note, etc.
  int durationsend[] = {
    8, 8, 8, 4, 4, 4,
    4, 8, 8, 8, 8, 8,
    8, 8, 8, 4, 4, 4,
    4, 8, 8, 8, 8, 8
  };
  int songLength = sizeof(melodyend) / sizeof(melodyend[0]);
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MEMBERS: ");
  lcd.setCursor(3, 1);
  lcd.print("TO5-C  HANGMAN");
  lcd.setCursor(1, 2);
  lcd.print("ELLA MAE  LANDERO");
  lcd.setCursor(1, 3);
  lcd.print("JOJIE ANN PABUAYA");
  delay(3000);
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MEMBERS: ");
  lcd.setCursor(3, 1);
  lcd.print("TO5-C  HANGMAN");
  lcd.setCursor(2, 2);
  lcd.print("MERILY  PALENCIA");
  lcd.setCursor(2, 3);
  lcd.print("KARL  VILLAVERDE");
  delay(3000);
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("TO5-C  HANGMAN");
  do {
    for (int thisNote = 0; thisNote < songLength; thisNote++) {
      int durationend = 1000 / durationsend[thisNote];
      tone(6, melodyend[thisNote], durationend);
      int pause = durationend * 1.3;
      delay(pause);
      noTone(8);
    }
  } while (true);
}
// function that will generate a random numbers with background music
void play() {
  lcd.clear();
  lcd.setCursor(8, 1);
  lcd.print("PLAY");
  lcd.setCursor(3, 2);
  lcd.print("(select enter)");
  delay(100);
  bool sound = true;
  const int totalNotes = sizeof(notes) / sizeof(int);
  float wait;
  do {
    for (int i = 0; i < totalNotes; i++) {
      numRand = random(40);
      const int currentNote = notes[i];
      if (i <= 20) {
        wait = durations[i] / 1.5;
      } else {
        wait = durations[i] / 0.8;
      }
      if (currentNote != 0) {
        tone(speakerPin, notes[i], wait);
      } else {
        noTone(speakerPin);
      }
      if (digitalRead(enter) == HIGH) {
        sound = false;
        delay(750);
        lcd.clear();
        break;
      }
      delay(wait);
    }
  } while (sound == true);
}
// main function of arduino
void loop() {
  if (life > 0) {
    int level = 0;
    if (level == 0) {
      intro();
      lcd.clear();
      delay(500);
    }
    level = 1;
    start();
  }
  fail();
  delay(300);
}