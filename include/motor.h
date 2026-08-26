#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>


// max_P = also das maximale signal anm pwm was man senden kann 
// dz = deadzone dammit kann man steurn von en potties dammit wenn man in der mitte ist das es stillbleibt 


class motor {
  private:
    int pwmPin;
    int in1Pin;
    int in2Pin;

    int aktuellerWert;
    int zielWert;
    unsigned long letzteZeit;

    int deadzone;
    int max_pwm;
    int ramp_step;
    int ramp_delay_ms;

  public:
    motor(int pwm, int in1, int in2, int dz = 240, int max_p = 1200); 
    void setSpeed(int wert);
    void update();          
};

#endif