#include <Joystick.h>


//Einstellen des Modus des jeweiligen Joysticks: Achse (für Ausgabe als Achse), Tasten (für Ausgabe als je eine Taste pro Richtung)
String StickMode1 = "Achse";   // Joystick am Anschluss A0
String StickMode2 = "Tasten";  // Joystick am Anschluss A1
String StickMode3 = "Achse";   // Joystick am Anschluss A2
String StickMode4 = "Achse";   // Joystick am Anschluss A3

int deadzone = 200;  //je größer der Wert umso kleiner ist die Auslenkung des Sticks bis die Taste ausgelöst wird (Maximum: 450)
float smoothFactor = 0.5; // 0,05 - 1 (je größer der Wert umso schwächer die Glättung)










































// Analogeingänge
const int axisPins[4] = {A0, A1, A2, A3};

// Interne Zwischenspeicher
float smoothValues[4] = {0, 0, 0, 0};

// Ergebnis der Modus-Auswertung
// 0 = Achse, 1 = Tasten
int modeResolved[4] = {0, 0, 0, 0};


//Konvertierung des Eingabetextes in boolschen Wert
int parseMode(String txt) {
  txt.toLowerCase();
  txt.trim();

  if (txt.startsWith("a")) return 0;  // Achse
  if (txt.startsWith("t")) return 1;  // Tasten

  return 0; // Fallback: Achse
}

//Initialisierung des Joysticks
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  8,
  0,
  true, true, true, true,
  false, false, false, false, false, false, false
);



void setup() {
  Joystick.begin();

  for (int i = 0; i < 4; i++) {
    smoothValues[i] = analogRead(axisPins[i]);
  }

  // --- Kunden-Einstellungen EINMAL auswerten ---
  String modesRaw[4] = {StickMode1, StickMode2, StickMode3, StickMode4};

  for (int i = 0; i < 4; i++) {
    modeResolved[i] = parseMode(modesRaw[i]);
  }
}


void loop() {

  for (int i = 0; i < 4; i++) {

    // Wert glätten
    int raw = analogRead(axisPins[i]);
    smoothValues[i] =
      (smoothFactor * raw) +
      ((1.0 - smoothFactor) * smoothValues[i]);

    int val = (int)smoothValues[i];


    //  Achse
    if (modeResolved[i] == 0) {
      switch (i) {
        case 0: Joystick.setXAxis(val); break;
        case 1: Joystick.setYAxis(val); break;
        case 2: Joystick.setZAxis(val); break;
        case 3: Joystick.setRxAxis(val); break;
      }
    }

    //  Tasten
    else {
      //Zentrieren der hinterlegten Achse wenn sie als Taste genutzt wird
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
}