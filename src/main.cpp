#include <Arduino.h>
#include "shiftreg.h"
#include "settings.h"
#include "main.h"
#include "animations.h"
#include <TaskScheduler.h>

/* Shift registers */
TLC591x shiftreg(NUM_SHIFT_REGS, SSDIPIN, SCLKPIN, SLEPIN);

/* Declare functions */
void writeLeds();
void switchAnimations();

/* Leds */
char leds[NUM_LEDS] = {};
const int ledmap[NUM_LEDS] = { 7,  6,  4,  5,  2,  3,  1,  0,
                              15, 14, 12, 13, 10, 11,  9,  8,
                              23, 22, 20, 21, 18, 19, 17, 16,
                              25, 24};
const int ledgrid[NUM_ROWS][NUM_COLS] = {{ 10,  11,  12,  13,  14,  15},
                                         {  9, EOF, EOF, EOF, EOF,  16},
                                         {  8, EOF, EOF, EOF, EOF,  17},
                                         {  7, EOF, EOF, EOF, EOF,  18},
                                         {  6, EOF, EOF, EOF, EOF,  19},
                                         {  5, EOF, EOF, EOF, EOF,  20}, 
                                         {  4, EOF, EOF, EOF, EOF,  21}, 
                                         {  3, EOF, EOF, EOF, EOF,  22},
                                         {  2,   1,   0,  25,  24,  23}};
const int N = NUM_LEDS;
const int bambits = BAM_RESOLUTION;
const int maxbr = (1<<bambits)-1;

/* System callbacks */
Scheduler runner;
Task ledupdater(400, TASK_FOREVER, &writeLeds);
Task animswitcher(ANIMATION_SWITCH_DELAY, TASK_FOREVER, &switchAnimations);

/* Animations */
const int numAnimations = 6;
Task t1( 30000, TASK_FOREVER, &fadeall);
Task t2( 60000, TASK_FOREVER, &pulserun);
Task t3( 30000, TASK_FOREVER, &randomleds);
Task t4( 30000, TASK_FOREVER, &voogtled);
Task t5( 20000, TASK_FOREVER, &runaround);
Task t6(120000, TASK_FOREVER, &bounce);
Task* animations[numAnimations] = {&t1, &t2, &t3, &t4, &t5, &t6};
int currentAnimation = 0;
bool firstRun = true;
bool lastRun = false;

void setup() {
  /* Enable screen update routine */
  runner.init();
  runner.addTask(ledupdater);
  runner.addTask(animswitcher);
  for (int i = 0; i < numAnimations; i++) 
    runner.addTask(*animations[i]);

  ledupdater.enable();
  animswitcher.enable();
  animations[0]->enable();

  randomSeed(analogRead(A0));
}

void loop() { runner.execute(); }

void writeLeds() {
  static int i = 0;
  static int bit = 0; //index of BAM bit
  static int imax = 0; //length of BAM bit segment

  //first iteration of this bit: set the outputs
  if (i == 0){
    //led states for this bit
    char sr_states[NUM_SHIFT_REGS] = {};

    int sr_number = 0;
    int sr_counter = 0;
    //update all leds
    for(int j = 0; j < N; j++){
      //determine which Shift Register IC
      if (sr_counter == 8) {sr_number++; sr_counter = 0;}
      sr_counter++;
      
      //set the led on the SR for this bit
      if (leds[ledmap[j]] & imax){ 
        sr_states[sr_number] |= 1<<(j - (8*sr_number));
      }
    }
    shiftreg.print(sr_states);
  }

  //Increase i and check if we have to go to next bit
  if (i < imax) i++;
  else{
    bit++;
    if (bit == bambits) bit = 0;
    i = 0;
    imax = 1 << (bambits-1-bit);
  }
}

/*
connected to MCU                          leds[0]
    |    |                                   |
    v    v                                   v
 ------------- ------------- ------------- -------------
| 24 25 26 27 | 16 17 18 19 |  8  9 10 11 |  0  1  2  3 |
|             |             |             |             |
| 31 30 29 28 | 23 22 21 20 | 15 14 13 12 |  7  6  5  4 |
 ------------- ------------- ------------- -------------

*/

void switchAnimations(){
  static int clkdivide = 0;
  clkdivide++;

  if (clkdivide == ANIMATION_SWITCH_DIVIDE){
    lastRun = true;
    animations[currentAnimation]->forceNextIteration();
    animations[currentAnimation]->disable();
    for (int j = 0; j < N; j++) leds[j] = 0;
    currentAnimation = random(numAnimations);
    animations[currentAnimation]->enable();
    firstRun = true;
    lastRun = false;
    clkdivide = 0;
  }
}