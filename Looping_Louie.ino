#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


#define keyHoch 3
#define keyRunter 2
#define keyWeniger 5
#define keyMehr 4
#define TurboEins 22
#define TurboZwei 23
#define TurboDrei 26
#define TurboVier 27


int menu = 1;                   //Speichern des aktuellen Menüpunkts

//Standardeinstellungen
int Turbo_modus = 1;            //0:deaktiviert, 1: Aufladen nach Zeit, 2: Aufladen nach Benutzung
int Turbo_ladezeit = 5000;      //Ladezeit in ms
int Turbo_dauer = 200;          //Dauer in ms
int Zufallsmodus = 1;           //1: aktiviert, 0: deaktiviert
int Geschwindigkeitsstufe = 1;  //Stufen 1-10
int Geschwindigkeit = 55;       //Standardgeschwindigkeit (Stufe 1)

int TurbostatusEins = 1;        //1: Turbo aufgeladen, 0: Turbo nicht aufgeladen
int TurbostatusZwei = 1;
int TurbostatusDrei = 1;
int TurbostatusVier = 1;

unsigned long turbo_ladezeit_eins = 0;
unsigned long turbo_ladezeit_zwei = 0;
unsigned long turbo_ladezeit_drei = 0;
unsigned long turbo_ladezeit_vier = 0;

unsigned long turbo_ladezeit_eins_vorher = 0;
unsigned long turbo_ladezeit_zwei_vorher = 0;
unsigned long turbo_ladezeit_drei_vorher = 0;
unsigned long turbo_ladezeit_vier_vorher = 0;

unsigned long previousMillis = 0;   //Zeit
unsigned long verzoegerung = 0;

int in1 = 9;
int in2 = 8;
int ConA = 10;

void setup() {
  lcd.init();                   //Im Setup wird der LCD gestartet
  lcd.backlight();

  pinMode(keyHoch, INPUT_PULLUP);
  pinMode(keyRunter, INPUT_PULLUP);
  pinMode(keyWeniger, INPUT_PULLUP);
  pinMode(keyMehr, INPUT_PULLUP);

  pinMode(TurboEins, INPUT_PULLUP);
  pinMode(TurboZwei, INPUT_PULLUP);
  pinMode(TurboDrei, INPUT_PULLUP);
  pinMode(TurboVier, INPUT_PULLUP);

  pinMode(in1, OUTPUT);         //Declaring the pin modes, obviously they're outputs
  pinMode(in2, OUTPUT);
  pinMode(ConA, OUTPUT);

  updateMenu();
}

void loop() {
  if (!digitalRead(keyHoch)) {
    menu--;
    updateMenu();
    delay(100);
    while (!digitalRead(keyHoch));
  }

  if (!digitalRead(keyRunter)) {
    if ((menu == 5) && Zufallsmodus == 1) {
      menu = 5;
    }
    else {
      menu++;
      updateMenu();
      delay(100);
      while (!digitalRead(keyRunter));
    }
  }

  if (!digitalRead(keyWeniger)) {
    EingabeWeniger();
    updateMenu();
    delay(100);
    while (!digitalRead(keyWeniger));
  }

  if (!digitalRead(keyMehr)) {
    EingabeMehr();
    updateMenu();
    delay(100);
    while (!digitalRead(keyMehr));
  }

  unsigned long currentMillis = millis();                            //Laufzeit aktualisieren
  unsigned long turbo_cooldown_eins = millis();
  unsigned long turbo_cooldown_zwei = millis();
  unsigned long turbo_cooldown_drei = millis();
  unsigned long turbo_cooldown_vier = millis();

  if ((Zufallsmodus == 1) && (currentMillis - previousMillis >= verzoegerung)) {
    previousMillis = currentMillis;

    Geschwindigkeit = random(55, 100);                               //zufällige Geschwindigkeit

    int random_verzoegerung[5] = {1000, 1500, 2000, 2500, 3000};     //zufällige Verzögerung
    int arrayIndex = (int)random(0, 5);
    verzoegerung = long(random_verzoegerung[arrayIndex]);
  }
  else if ((Zufallsmodus == 0) && (currentMillis - previousMillis >= verzoegerung)) {
    previousMillis = currentMillis;

    if (Geschwindigkeitsstufe == 1) {
      Geschwindigkeit = 55;
    }
    else if (Geschwindigkeitsstufe == 2) {
      Geschwindigkeit = 77;
    }
    else if (Geschwindigkeitsstufe == 3) {
      Geschwindigkeit = 99;
    }
    else if (Geschwindigkeitsstufe == 4) {
      Geschwindigkeit = 121;
    }
    else if (Geschwindigkeitsstufe == 5) {
      Geschwindigkeit = 143;
    }
    else if (Geschwindigkeitsstufe == 6) {
      Geschwindigkeit = 165;
    }
    else if (Geschwindigkeitsstufe == 7) {
      Geschwindigkeit = 187;
    }
    else if (Geschwindigkeitsstufe == 8) {
      Geschwindigkeit = 210;
    }
    else if (Geschwindigkeitsstufe == 9) {
      Geschwindigkeit = 232;
    }
    else if (Geschwindigkeitsstufe == 10) {
      Geschwindigkeit = 255;
    }
    verzoegerung = 500;
  }

  if (Turbo_modus == 1) {                 //Turbo nach Zeit
    if ((!digitalRead(TurboEins)) && (TurbostatusEins == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusEins = 0;
      turbo_ladezeit_eins = Turbo_ladezeit;
      turbo_ladezeit_eins_vorher = turbo_cooldown_eins;
    }
    if ((!digitalRead(TurboZwei)) && (TurbostatusZwei == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusZwei = 0;
      turbo_ladezeit_zwei = Turbo_ladezeit;
      turbo_ladezeit_zwei_vorher = turbo_cooldown_zwei;
    }
    if ((!digitalRead(TurboDrei)) && (TurbostatusDrei == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusDrei = 0;
      turbo_ladezeit_drei = Turbo_ladezeit;
      turbo_ladezeit_drei_vorher = turbo_cooldown_drei;
    }
    if ((!digitalRead(TurboVier)) && (TurbostatusVier == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusVier = 0;
      turbo_ladezeit_vier = Turbo_ladezeit;
      turbo_ladezeit_vier_vorher = turbo_cooldown_vier;
    }

    //Turbos freischalten nach abgelaufener Zeit
    if (turbo_cooldown_eins - turbo_ladezeit_eins_vorher >= turbo_ladezeit_eins) {
      TurbostatusEins = 1;
    }
    if (turbo_cooldown_zwei - turbo_ladezeit_zwei_vorher >= turbo_ladezeit_zwei) {
      TurbostatusZwei = 1;
    }
    if (turbo_cooldown_drei - turbo_ladezeit_drei_vorher >= turbo_ladezeit_drei) {
      TurbostatusDrei = 1;
    }
    if (turbo_cooldown_vier - turbo_ladezeit_vier_vorher >= turbo_ladezeit_vier) {
      TurbostatusVier = 1;
    }
  }

  else if (Turbo_modus == 2) {            //Turbo wenn alle ihn genutzt haben
    if ((TurbostatusEins == 0) && (TurbostatusZwei == 0) && (TurbostatusDrei == 0) && (TurbostatusVier == 0)) {
      delay(400);                 //Vermeidung von Mehrfachregistrierung der Taster
      TurbostatusEins = 1;
      TurbostatusZwei = 1;
      TurbostatusDrei = 1;
      TurbostatusVier = 1;
    }
    if ((!digitalRead(TurboEins)) && (TurbostatusEins == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusEins = 0;
    }
    if ((!digitalRead(TurboZwei)) && (TurbostatusZwei == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusZwei = 0;
    }
    if ((!digitalRead(TurboDrei)) && (TurbostatusDrei == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusDrei = 0;
    }
    if ((!digitalRead(TurboVier)) && (TurbostatusVier == 1)) {
      Geschwindigkeit = 255;
      previousMillis = currentMillis;
      verzoegerung = Turbo_dauer;
      TurbostatusVier = 0;
    }
  }
  else if (Turbo_modus == 0) {                       //Turbo aus
    TurbostatusEins = 0;
    TurbostatusZwei = 0;
    TurbostatusDrei = 0;
    TurbostatusVier = 0;
  }

  MotorVorwaerts(Geschwindigkeit);
}

//Menüübersicht
void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
    case 1:
      lcd.clear();
      lcd.print("Looping Louie");
      lcd.setCursor(0, 1);
      lcd.print("Extrem-Variante");
      break;
    case 2:
      lcd.clear();
      lcd.print("Turbomodus");
      if (Turbo_modus == 1) {
        lcd.setCursor(0, 1);
        lcd.print("> nach Zeit");
      }
      else if (Turbo_modus == 2) {
        lcd.setCursor(0, 1);
        lcd.print("> alle genutzt");
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("> deaktiviert");
      }
      break;
    case 3:
      lcd.clear();
      lcd.print("Ladezeit Turbo");
      lcd.setCursor(0, 1);
      lcd.print("> " + (String) Turbo_ladezeit + " ms");
      break;
    case 4:
      lcd.clear();
      lcd.print("Turbodauer");
      lcd.setCursor(0, 1);
      lcd.print("> " + (String) Turbo_dauer + " ms");
      break;
    case 5:
      lcd.clear();
      lcd.print("Zufallsmodus");
      if (Zufallsmodus == 1) {
        lcd.setCursor(0, 1);
        lcd.print("> aktiviert");
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("> deaktiviert");
      }
      break;
    case 6:
      if (Zufallsmodus == 1) {
        menu = 5;
      }
      else {
        lcd.clear();
        lcd.print("Geschwindigkeit");
        lcd.setCursor(0, 1);
        lcd.print("> Stufe " + (String)Geschwindigkeitsstufe);
      }
    case 7:
      menu = 6;
  }
}


//Eingabetaste "Weniger"
void EingabeWeniger() {
  switch (menu) {
    case 1:
      break;
    case 2:
      if (Turbo_modus == 1) {
        Turbo_modus = 0;
      }
      else if (Turbo_modus == 2) {
        Turbo_modus = 1;
      }
      else {
        Turbo_modus = 2;
      }
      break;
    case 3:
      Turbo_ladezeit = Turbo_ladezeit - 100;
      if (Turbo_ladezeit < 100) {
        Turbo_ladezeit = 10000;
      }
      break;
    case 4:
      Turbo_dauer = Turbo_dauer - 50;
      if (Turbo_dauer <= 50) {
        Turbo_dauer = 750;
      }
      break;
    case 5:
      if (Zufallsmodus == 1) {
        Zufallsmodus = 0;
        verzoegerung = 0;
      }
      else {
        Zufallsmodus = 1;
      }
      break;
    case 6:
      if (Geschwindigkeitsstufe == 1) {
        Geschwindigkeitsstufe = 1;
      }
      else {
        Geschwindigkeitsstufe--;
      }
  }
}


//Eingabetaste "Mehr"
void EingabeMehr() {
  switch (menu) {
    case 1:
      break;
    case 2:
      if (Turbo_modus == 1) {
        Turbo_modus = 2;
      }
      else if (Turbo_modus == 2) {
        Turbo_modus = 0;
      }
      else {
        Turbo_modus = 1;
      }
      break;
    case 3:
      Turbo_ladezeit = Turbo_ladezeit + 100;
      if (Turbo_ladezeit > 10000) {
        Turbo_ladezeit = 100;
      }
      break;
    case 4:
      Turbo_dauer = Turbo_dauer + 50;
      if (Turbo_dauer > 750) {
        Turbo_dauer = 50;
      }
      break;
    case 5:
      if (Zufallsmodus == 1) {
        Zufallsmodus = 0;
        verzoegerung = 0;
      }
      else {
        Zufallsmodus = 1;
      }
      break;
    case 6:
      if (Geschwindigkeitsstufe == 10) {
        Geschwindigkeitsstufe = 10;
      }
      else {
        Geschwindigkeitsstufe++;
      }
  }
}

//Motorsteuerung
int MotorVorwaerts(int speed_louie) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(ConA, speed_louie);
}

void MotorAus() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ConA, 0);
}

int MotorRueckwaerts(int speed_louie) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(ConA, speed_louie);
}
