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

void nema::setMicrostepping(int multiplier) {
    microsteppingMultiplier = multiplier;
    switch (multiplier) {
        case 1:  
            digitalWrite(m0Pin, LOW); digitalWrite(m1Pin, LOW); digitalWrite(m2Pin, LOW); break;
        case 8:  
            digitalWrite(m0Pin, HIGH); digitalWrite(m1Pin, HIGH); digitalWrite(m2Pin, LOW); break;
        case 32: 
            digitalWrite(m0Pin, HIGH); digitalWrite(m1Pin, HIGH); digitalWrite(m2Pin, HIGH); break;
        default:
            microsteppingMultiplier = 1;
            digitalWrite(m0Pin, LOW); digitalWrite(m1Pin, LOW); digitalWrite(m2Pin, LOW); break;
    }
}

void nema::stepMotor(int steps, bool direction) {
    digitalWrite(dirPin, direction ? HIGH : LOW);
    
    for (int i = 0; i < steps; i++) {
        if (checkFault()) {
            break; // Hardware-Schutz: Abbruch bei Fehler
        }

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500); // Statische Verzögerung (Geschwindigkeit)
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500); 
    }
}



void nema::moveToDegree(int degree) {
    // 1. Winkel-Normalisierung (0-359 Grad)
    degree = degree % 360;
    if (degree < 0) degree += 360;

    // 2. Umrechnung in Schritte basierend auf aktuellem Microstepping
    long totalStepsPerRev = baseStepsPerRev * microsteppingMultiplier;
    long targetStep = (long)degree * totalStepsPerRev / 360;

    // 3. Differenz berechnen
    long difference = targetStep - currentStepPosition;

    // 4. Kürzesten Weg ermitteln
    long halfRev = totalStepsPerRev / 2;
    if (difference > halfRev) {
        difference -= totalStepsPerRev;
    } else if (difference < -halfRev) {
        difference += totalStepsPerRev;
    }

    if (difference == 0) return; // Ziel bereits erreicht

    bool direction = (difference > 0);
    
    enable(); // Motor aktivieren
    
    stepMotor(abs(difference), direction);
    
    disable(); // Motor deaktivieren zur Stromeinsparung
    
    // 5. Interne Position aktualisieren und im 360-Grad-Bereich halten
    currentStepPosition = (currentStepPosition + difference) % totalStepsPerRev;
    if (currentStepPosition < 0) {
        currentStepPosition += totalStepsPerRev;
    }
}