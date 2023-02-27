/*

*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include <Keypad.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3D
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMOFCOMM 0
#define MOTOR 1
#define DIRECTION 2
#define VALUE 3
#define VERIFY 4
#define EXECUTE 5
#define EDIT 6

#define JOYSTICK 10
#define KEYPAD 11

#define HUNDREDS 15
#define TENS 16
#define ONES 17

int controltype = 10;
int kpControl;
int digitPlace = 15;

int g;

int s;

int pVal;

int dVal;

int sMessage = 0;

int mShiver;
int dShiver;
int hShiver;
int tShiver;
int oShiver;
int vShiver;
int eShiver;

int motorvar1;
int motorvar2;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[COLS][ROWS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'0', 'F', 'E', 'D'}
};

byte colPins[COLS] = {37, 36, 35, 34};
byte rowPins[ROWS] = {33, 32, 31, 30};

byte mValArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte dValArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte hValArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte tValArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte oValArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int BaseA = 14;
int BaseB = 15;
int LowerA = 4;
int LowerB = 5;
int UpperA = 6;
int UpperB = 7;
int Wrist1a = 22;
int Wrist1b = 23;
int Wrist2a = 24;
int Wrist2b = 25;
int HandA = 26;
int HandB = 27;

int ControlJoystick = 16;
int ControlKeypad = 17;

int changeMessage = 0;

int previousCount;
int currentCount;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  pinMode(BaseA, OUTPUT);
  pinMode(BaseB, OUTPUT);
  pinMode(LowerA, OUTPUT);
  pinMode(LowerB, OUTPUT);
  pinMode(UpperA, OUTPUT);
  pinMode(UpperB, OUTPUT);
  pinMode(Wrist1a, OUTPUT);
  pinMode(Wrist1b, OUTPUT);
  pinMode(Wrist2a, OUTPUT);
  pinMode(Wrist2b, OUTPUT);
  pinMode(HandA, OUTPUT);
  pinMode(HandB, OUTPUT);

  pinMode(ControlJoystick, INPUT);
  pinMode(ControlKeypad, INPUT);

  Serial.begin(9600);
  pinMode(9, INPUT);
}

void loop() {
  int y = analogRead(2);
  int x = digitalRead(9);
  char customKey = customKeypad.getKey();
  g = customKey;
  //Key Code, 0-9 Correspond to # on Keypad
  //17-22 Correspond to A-F on Keypad.
  int key = map(g, 48, 70, 0, 22);
  int jsR = digitalRead(ControlJoystick);
  int kpR = digitalRead(ControlKeypad);
  if (jsR == HIGH) {
    controltype = 10;
  }
  if (kpR == HIGH) {
    controltype = 11;
  }

  switch (controltype) {
      {
      case JOYSTICK:
        if (key == 17) {
          motorvar1 = 14;
          motorvar2 = 15;
        } if (key == 18) {
          motorvar1 = 4;
          motorvar2 = 5;
        } if (key == 19) {
          motorvar1 = 6;
          motorvar2 = 7;
        } if (key == 20) {
          motorvar1 = 22;
          motorvar2 = 23;
        } if (key == 21) {
          motorvar1 = 24;
          motorvar2 = 25;
        } if (key == 22) {
          motorvar1 = 26;
          motorvar2 = 27;
        }
        if (0 <= y && y <= 262) {
          digitalWrite(motorvar1, HIGH);
          digitalWrite(motorvar2, LOW);
        } else if (762 <= y && y <= 1023 ) {
          digitalWrite(motorvar1, LOW);
          digitalWrite(motorvar2, HIGH);

        } else if (262 <= y && y <= 762) {
          digitalWrite(motorvar1, LOW);
          digitalWrite(motorvar2, LOW);
        }

      }

    case KEYPAD:
      switch (kpControl) {
        case NUMOFCOMM:
          if (changeMessage == 0) {
            Serial.println("# of Commands");
            changeMessage = 1;
          }
          if (key >= 0 && key <= 9) {
            s = key;
            if (s == 0) {
              s = 10;
            }
            changeMessage = 0;
            mShiver = 0;
            dShiver = 0;
            hShiver = 0;
            tShiver = 0;
            oShiver = 0;
            vShiver = 0;
            eShiver = 0;
            kpControl = 1;
          }
          break;

        case MOTOR: {
            if (changeMessage == 0) {
              Serial.println("Changed to Motor");
              changeMessage = 1;
            }
            if (mShiver < s) {
              if (key >= 17 && key <= 22) {
                mValArray[mShiver] = key;
                mShiver = mShiver + 1;
              }
            } if (mShiver == s) {
              changeMessage = 0;
              kpControl = 2;
            }
          }
          break;

        case DIRECTION:
          if (changeMessage == 0) {
            Serial.println("Changed to Direction");
            changeMessage = 1;
          }
          if (dShiver < s) {
            if (key >= 0 && key <= 1) {
              dValArray[dShiver] = key;
              dShiver = dShiver + 1;
            }
          } if (dShiver == s) {
            changeMessage = 0;
            digitPlace = 15;
            kpControl = 3;
          }
          break;

        case VALUE:
          switch (digitPlace) {
            case HUNDREDS:
              if (changeMessage == 0) {
                Serial.println("Changed to Hundreds");
                changeMessage = 1;
              }
              if (hShiver < s) {
                if (key >= 0 && key <= 9) {
                  Serial.println("boop");
                  hValArray[hShiver] = key;
                  hShiver = hShiver + 1;
                }
              } if (hShiver == s) {
                changeMessage = 0;
                digitPlace = 16;
              }
              break;

            case TENS:
              if (changeMessage == 0) {
                Serial.println("Changed to Tens");
                changeMessage = 1;
              }
              if (tShiver < s) {
                if (key >= 0 && key <= 9) {
                  tValArray[tShiver] = key;
                  tShiver = tShiver + 1;
                }
              } if (tShiver == s) {
                changeMessage = 0;
                digitPlace = 17;
              }
              break;

            case ONES:
              if (changeMessage == 0) {
                Serial.println("Changed to Ones");
                changeMessage = 1;
              }
              if (oShiver < s) {
                if (key >= 0 && key <= 9) {
                  oValArray[oShiver] = key;
                  oShiver = oShiver + 1;
                }
              } if (oShiver == s) {
                changeMessage = 0;
                kpControl = 4;
              }
              break;
          }

          break;

        case VERIFY:
          if (changeMessage == 0) {
            Serial.println("Changed to Ones");
            changeMessage = 1;
          }
          if (vShiver < s) {
            if (sMessage == 0) {
              Serial.print("Command ");
              Serial.println(vShiver + 1);
              Serial.print("The ");
              if (mValArray[vShiver] == 17) {
                Serial.print("Base will move ");
              } if (mValArray[vShiver] == 18) {
                Serial.print("Shoulder will move ");
              } if (mValArray[vShiver] == 19) {
                Serial.print("Elbow will move");
              } if (mValArray[vShiver] == 20) {
                Serial.print("First Wrist will move ");
              } if (mValArray[vShiver] == 21) {
                Serial.print("Second Wrist will move ");
              } if (mValArray[vShiver] == 22) {
                Serial.print("Hand will move ");
              }
              Serial.print(hValArray[vShiver]);
              Serial.print(tValArray[vShiver]);
              Serial.print(oValArray[vShiver]);
              if (dValArray[vShiver] == 0) {
                Serial.println(" steps Backwards.");
              } if (dValArray[vShiver] == 1) {
                Serial.println(" steps Forewards.");
              }
              Serial.println(" ");
              Serial.println("Is this correct?");
              Serial.println("1/Yes - 0/No");
              sMessage = 1;
            } if (key == 1) {
              sMessage = 0;
              vShiver = vShiver + 1;
            } if (key == 0) {

            }
            if (vShiver == s) {
              kpControl = 5;
            }
          }
          break;

        case EXECUTE:
          if (eShiver < s) {
            previousCount = currentCount;
            if (dValArray[eShiver] == 0) {
              dVal = -1;
            } if (dValArray[eShiver] == 1) {
              dVal = 1;
            }
            currentCount = currentCount + ((hValArray[eShiver] * 100) + (tValArray[eShiver] * 10) + oValArray[eShiver]) * dVal;
            if (dValArray[eShiver] == 0) {
              currentCount = currentCount * -1;
            }
            if (mValArray[eShiver] == 17) {
              motorvar1 = 14;
              motorvar2 = 15;
            } if (mValArray[eShiver] == 18) {
              motorvar1 = 4;
              motorvar2 = 5;
            } if (mValArray[eShiver] == 19) {
              motorvar1 = 6;
              motorvar2 = 7;
            } if (mValArray[eShiver] == 20) {
              motorvar1 = 22;
              motorvar2 = 23;
            } if (mValArray[eShiver] == 21) {
              motorvar1 = 24;
              motorvar2 = 25;
            } if (mValArray[eShiver] == 22) {
              motorvar1 = 26;
              motorvar2 = 27;
            }
            if (currentCount > 0);
            for (previousCount; previousCount < currentCount; previousCount = previousCount + 9.5 ) {
              Serial.print("Current Value: ");
              Serial.println(currentCount);
              Serial.print("Previous Value: ");
              Serial.println(previousCount);
              digitalWrite(motorvar1, HIGH);
              digitalWrite(motorvar2, LOW);
            }
            if (currentCount < 0);
            for (previousCount; previousCount > currentCount; previousCount = previousCount - 9.5 ) {
              Serial.print("Current Value: ");
              Serial.println(currentCount);
              Serial.print("Previous Value: ");
              Serial.println(previousCount);
              digitalWrite(motorvar1, LOW);
              digitalWrite(motorvar2, HIGH);
            }
            previousCount = currentCount - (hValArray[eShiver] * 100) + (tValArray[eShiver] * 10) + oValArray[eShiver];
            digitalWrite(motorvar1, LOW);
            digitalWrite(motorvar2, LOW);
            eShiver = eShiver + 1;
          }
          if (eShiver == s) {
            kpControl = 0;
          }
          break;

        case EDIT:

          break;


      }
      break;
  }
}

