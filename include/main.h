#include "settings.h"

#ifndef FOTOLIJST_MAIN_H
#define FOTOLIJST_MAIN_H

//shift register
#define SLEPIN D6
#define SCLKPIN D7
#define SSDIPIN D8

//leds
#define NUM_LEDS 26
#define NUM_SHIFT_REGS 4
extern const int N;
extern char leds[];
extern const int bambits;
extern const int maxbr;

//animations
struct ledImpulseNode {
    int lednum;
    int period;
    int phase;
    int amplitude;

    ledImpulseNode* next;
    ledImpulseNode* prev;
};


#endif