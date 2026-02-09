#include <Joystick.h>

// ================== STICK MODI ==================
String StickMode1 = "Achse";   // A0
String StickMode2 = "Tasten";  // A1
String StickMode3 = "Achse";   // A2
String StickMode4 = "Achse";   // A3

int deadzone = 200;
float smoothFactor = 0.5;





































// ================== ACHSEN ==================
const int axisPins[4] = {A0, A1, A2, A3};
float smoothValues[4] = {0, 0, 0, 0};
int modeResolved[4] = {0, 0, 0, 0};

// ================== DIGITALE TASTER ==================
const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// ================== JOYSTICK ==================
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  8,   // 8 Buttons
  0,
  true, true, true, true,
  false, false, false, false, false, false, false
);

// ================== HILFSFUNKTION ==================
int parseMode(String txt) {
  txt.toLowerCase();
  txt.trim();
  if (txt.startsWith("a")) return 0;
  if (txt.startsWith("t")) return 1;
  return 0;
}

// ================== SETUP ==================
void setup() {
  Joystick.begin();

  // Achsen initialisieren
  for (int i = 0; i < 4; i++) {
    smoothValues[i] = analogRead(axisPins[i]);
  }

  // Stick-Modi auswerten
  String modesRaw[4] = {StickMode1, StickMode2, StickMode3, StickMode4};
  for (int i = 0; i < 4; i++) {
    modeResolved[i] = parseMode(modesRaw[i]);
  }

  // Digitale Taster
  for (int i = 0; i < 8; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {

  // ----- ACHSEN / ACHS-TASTEN -----
  for (int i = 0; i < 4; i++) {

    int raw = analogRead(axisPins[i]);
    smoothValues[i] =
      (smoothFactor * raw) +
      ((1.0 - smoothFactor) * smoothValues[i]);

    int val = (int)smoothValues[i];

    if (modeResolved[i] == 0) {
      switch (i) {
        case 0: Joystick.setXAxis(val); break;
        case 1: Joystick.setYAxis(val); break;
        case 2: Joystick.setZAxis(val); break;
        case 3: Joystick.setRxAxis(val); break;
      }
    } else {
      switch (i) {
        case 0: Joystick.setXAxis(511); break;
        case 1: Joystick.setYAxis(511); break;
        case 2: Joystick.setZAxis(511); break;
        case 3: Joystick.setRxAxis(511); break;
      }

      int buttonLow  = i * 2;
      int buttonHigh = i * 2 + 1;

      Joystick.setButton(buttonLow,  (val <= deadzone));
      Joystick.setButton(buttonHigh, (val >= (1023 - deadzone)));
    }
  }

  // ----- DIGITALE TASTER -----
  for (int i = 0; i < 8; i++) {
    bool pressed = (digitalRead(buttonPins[i]) == LOW);
    Joystick.setButton(i, pressed);
  }
}