#include <Arduino.h>
//#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "shiftreg.h"
#include "settings.h"
#include "main.h"
#include "animations.h"

/* Shift registers */
TLC591x shiftreg(NUM_SHIFT_REGS, SSDIPIN, SCLKPIN, SLEPIN);

/* WiFi connection */
//const char* ssid = WIFI_SSID;
//const char* password = WIFI_PASSWORD;
//WiFiClient client;

/* Declare functions */
void writeLeds();
void switchAnimations();

/* Leds */
char leds[NUM_LEDS] = {};
const int ledmap[NUM_LEDS] = { 7,  6,  4,  5,  2,  3,  1,  0,
                              15, 14, 12, 13, 10, 11,  9,  8,
                              23, 22, 20, 21, 18, 19, 17, 16,
                              25, 24};
const int N = NUM_LEDS;
const int bambits = BAM_RESOLUTION;
const int maxbr = (1<<bambits)-1;

/* System callbacks */
Ticker updateLeds(writeLeds, 100, 0, MICROS_MICROS);
Ticker animationSwitcher(switchAnimations,ANIMATION_SWITCH_DELAY*1000);

/* Animations */
Ticker animations[] = {Ticker(runaround, 30), 
                       Ticker(randomleds, 30), 
                       Ticker(voogtled, 30), 
                       Ticker(pulserun, 30),
                       Ticker(pulserun, 10)};
const int numAnimations = 5;
int currentAnimation = 0;

void setup() {
  /* Make WiFi connection */
  //WiFi.disconnect();
  //WiFi.hostname("DISCOLIJST");
  //WiFi.begin(ssid, password);
  //WiFi.mode(WIFI_STA);
  //while (WiFi.status() != WL_CONNECTED) delay(100);

  /* Enable screen update routine */
  updateLeds.start();
  animationSwitcher.start();

  // initialise random number generator
  srand (time(NULL));
}

void loop() {
  updateLeds.update();
  animationSwitcher.update();
  animations[currentAnimation].update();
}

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
  animations[currentAnimation].stop();
  for (int j = 0; j < N; j++) leds[j] = 0;
  currentAnimation = rand()%numAnimations;
  animations[currentAnimation].start();
}