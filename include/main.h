#include "settings.h"

#ifndef FOTOLIJST_MAIN_H
#define FOTOLIJST_MAIN_H

//shift register
#define SLEPIN 7
#define SCLKPIN 8
#define SSDIPIN 9

//leds
#define NUM_LEDS 26
#define NUM_SHIFT_REGS 4
extern const int N;
extern char leds[];
extern const int bambits;
extern const int maxbr;

//animations
extern bool firstRun;
extern bool lastRun;
struct ledImpulseNode {
    int lednum;
    int period;
    int phase;
    int amplitude;

    ledImpulseNode* next;
    ledImpulseNode* prev;
};


#endif