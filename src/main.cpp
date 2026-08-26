#include <Arduino.h>
#include <esp_now.h>

#include "steppermotor.h"
#include "motor.h"


// Datenstruktur passend zum Sender-Repository
typedef struct struct_message {
    int poti_left;
    int poti_right;
} struct_message;

struct_message empfangeneDaten;



















// Initialisierung der Motor-Objekte mit den entsprechenden GPIO-Pins
motor motorLinks(25, 26, 27);
motor motorRechts(21, 19, 18);

void setup() {
printf("works on my machine");






}

void loop() {


  // put your main code here, to run repeatedly:








  
}

