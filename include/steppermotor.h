
#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include <Arduino.h>


class nema {
private:
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t enPin;
    uint8_t m0Pin;
    uint8_t m1Pin;
    uint8_t m2Pin;
    uint8_t faultPin; // Neu: Pin für die Fehlerüberwachung
    
    long currentStepPosition;
    int microsteppingMultiplier; 
    const int baseStepsPerRev = 200; 

    void stepMotor(int steps, bool direction);

public:
    nema(uint8_t step, uint8_t dir, uint8_t en, uint8_t m0, uint8_t m1, uint8_t m2 , u_int8_t fault);
    void begin();
    void setMicrostepping(int multiplier);
    void moveToDegree(int degree);
    void enable();
    void disable();
    bool checkFault(); // Neu: Methode zum Auslesen des Fehlerstatus




};


#endif