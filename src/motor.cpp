#include "motor.h"

motor::motor(int pwm, int in1, int in2, int dz, int max_p) {
  pwmPin = pwm;
  in1Pin = in1;
  in2Pin = in2;
  deadzone = dz;
  max_pwm = max_p;

  // Konfiguration für das weiche Anfahren (Soft-Start)
  ramp_step = 15;      // Schrittgröße pro Intervall
  ramp_delay_ms = 5;   // Zeitintervall in Millisekunden

  aktuellerWert = 2047; // Start im absoluten Stillstand
  zielWert = 2047;
  letzteZeit = 0;
  
  pinMode(pwmPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);

  // Initialer Hardware-Zustand (Motoren aus)
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(pwmPin, 0);
}

void motor::setSpeed(int wert) {
  // Überprüfung, ob sich der Wert innerhalb der Deadzone befindet
  if (abs(wert - 2047) < deadzone) {
    zielWert = 2047;
  } else {
    // Werte auf den Bereich 0-4095 begrenzen
    zielWert = constrain(wert, 0, 4095);
  }
}

void motor::update() {
  // Nicht-blockierender Timer für die Ramping-Verzögerung
  if (millis() - letzteZeit > ramp_delay_ms) {
    letzteZeit = millis();

    // Ramping-Logik: Aktuellen Wert dem Zielwert annähern
    if (aktuellerWert < zielWert) {
      aktuellerWert += ramp_step;
      if (aktuellerWert > zielWert) aktuellerWert = zielWert; 
    } 
    else if (aktuellerWert > zielWert) {
      aktuellerWert -= ramp_step;
      if (aktuellerWert < zielWert) aktuellerWert = zielWert;
    }

    // Hardware-Logik für den TB6612FNG Treiber
    int gas = 0;

    if (aktuellerWert == 2047) {
      // Absoluter Stopp (Freilauf)
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, LOW);
      gas = 0;
    } 
    else if (aktuellerWert > 2047) {
      // Eine Laufrichtung
      digitalWrite(in1Pin, HIGH);
      digitalWrite(in2Pin, LOW);
      // Skalierung des Wertes 2048-4095 auf das PWM-Limit
      gas = map(aktuellerWert, 2048, 4095, 0, max_pwm);
    } 
    else {
      // Entgegengesetzte Laufrichtung
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, HIGH);
      // Skalierung des Wertes 2046-0 auf das PWM-Limit
      gas = map(aktuellerWert, 2046, 0, 0, max_pwm);
    }

    // Übertragung des Signals an den Treiber
    analogWrite(pwmPin, gas); 
  }
}