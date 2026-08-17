#include "steppermotor.h"



nema::nema(uint8_t step, uint8_t dir, uint8_t en, uint8_t m0, uint8_t m1, uint8_t m2 , uint8_t fault ){

     stepPin = step;
     dirPin = dir;
     enPin = en;
     m0Pin = m0 ;
     m1Pin = m1;
     m2Pin = m2;
     faultPin = fault; 

    currentStepPosition = 0;
     microsteppingMultiplier = 1; 

}

void nema::begin() {

    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(m0Pin, OUTPUT);
    pinMode(m1Pin, OUTPUT);
    pinMode(m2Pin, OUTPUT);
    
    // FAULT Pin als reiner Eingang (Pull-Up ist extern auf PCB)
    pinMode(faultPin, INPUT);
    
    disable(); // Startzustand: Stromlos
    setMicrostepping(1); // Startzustand: Vollschritt
}


void nema::enable() {
    digitalWrite(enPin, LOW); 
}

void nema::disable() {
    digitalWrite(enPin, HIGH); 
}

bool nema::checkFault() {
    // DRV8825 zieht bei Überhitzung/Überstrom den Pin auf LOW
    if (digitalRead(faultPin) == LOW) {
        Serial.println("KRITISCHER FEHLER: DRV8825 Fault registriert!");
        return true; // Fehler ist vorhanden
    }
    return false; // Kein Fehler
}