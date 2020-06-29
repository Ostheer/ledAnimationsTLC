#ifndef FOTOLIJST_ANIMATIONS_H
#define FOTOLIJST_ANIMATIONS_H

void voogtled();
void fadeall();
void runaround();
void pulserun();
void randomleds();
void bounce();

struct ledImpulseNode {
    int lednum;
    int period;
    int phase;
    int amplitude;

    ledImpulseNode* next;
    ledImpulseNode* prev;
};

#endif