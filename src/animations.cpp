#include <Arduino.h>
#include "animations.h"
#include "main.h"

void voogtled(){
  static int i = 0;
  static int imax = 25;
  
  float arg = 6.3*i/imax;
  int val0 =maxbr*pow(sin(arg),2);
  int val1 = maxbr*pow(sin(arg+1.6),2);

  for (int j = 0; j < N; j += 2) leds[j] = val0;
  for (int j = 1; j < N; j += 2) leds[j] = val1;

  i++; if (i > imax) i = 0;
}

void fadeall(){
  static int i = 0;
  static int imax = 50;
  
  float arg = 6.3*i/imax;
  int val0 = maxbr*pow(sin(arg),2);

  for (int j = 0; j < N; j++) leds[j] = val0;

  i++; if (i > imax) i = 0;
}

void runaround(){
  static bool direction = true;
  if (animationSwitched){
    animationSwitched = false;
    direction = random(2);
  }
  static int i = 0;
  static int imax = 25;
  static int iled0 = 0;

  leds[iled0] = maxbr*pow(sin(3.14*i/imax),2);

  i++; 
  if (i > imax){
    if (direction){
      iled0++;
      if (iled0 == N) iled0 = 0;
    }
    else{
      iled0--;
      if (iled0 < 0) iled0 = N-1;
    }
    i = 0;
  }
}

void pulserun(){
  static bool direction = true;
  int temp;

  if (animationSwitched){
    for (int j = 0; j < 7; j++){
      leds[j] = maxbr*pow(sin(3.14*j/7),2);
    }
    animationSwitched = false;
    direction = random(2);
  }

  if (direction){
    temp = leds[N-1];
    for (int j = N-1; j >= 0; j--){
      leds[j] = leds[j-1];
    }
    leds[0] = temp;
  }
  else{
    temp = leds[0];
    for (int j = 0; j < N; j++){
      leds[j] = leds[j+1];
    }
    leds[N-1] = temp;
  }
}

void randomleds() {
  //pulse creation rate. lower is more
  static int creationDivisor = 12;
  
  const int pulseLength = 33;
  const float gauss[pulseLength] = {0, 0.011, 0.02, 0.034, 0.057, 0.09, 0.136, 0.199, 0.279, 0.376, 0.488, 
                                    0.607, 0.727, 0.836, 0.923, 0.98, 1.0, 0.98, 0.923, 0.836, 0.727, 0.607, 
                                    0.488, 0.376, 0.279, 0.199, 0.136, 0.09, 0.057, 0.034, 0.02, 0.011, 0};
  //[round(np.e**(-x**2/2),3) for x in np.linspace(-3,3,31)]

  /* clear buffer */
  for (int j = 0; j < N; j++){
    leds[j] = 0;
  }

  //set up variables
  static ledImpulseNode* head = nullptr;
  static ledImpulseNode* current = head;
  static ledImpulseNode* temp;
  
  /* Remove dead pulses */
  current = head;
  while (current != nullptr){
    if (current->phase == current->period){
      if (current == head) head = head->next;
      
      //cout << "Removing finished pulse from chain" << endl;
      if (current->next != nullptr) current->next->prev = current->prev;
      if (current->prev != nullptr) current->prev->next = current->next;
      
      //cout << "Freeing memory" << endl;
      temp = current->next;
      delete current;
      current = temp;
    }
    if (current != nullptr) current = current->next;
  }
  
  /* Draw all pulses */
  current = head;
  while (current != nullptr){
    leds[current->lednum] = current->amplitude * gauss[(int) (((float) pulseLength * current->phase)/current->period)];
    current->phase++;
    current = current->next;
  }
  
  /* Create new pulse at random */
  if (random(creationDivisor) == 1){
    // Looking for last element
    current = head;
    temp = head;
    while (current != nullptr){
      temp = current;
      current = current->next;
    }
    current = temp;
    
    // Make new objects
    temp = new ledImpulseNode;
    if (current != nullptr){
      current->next = temp;
      temp->prev = current;
    }
    else{
      temp->prev = nullptr;
      head = temp;
    }
    temp->next = nullptr;
    
    // Providing random parameters
    temp->amplitude = maxbr/3 + random( (int) (maxbr-maxbr/3));
    temp->lednum = random(N);
    temp->phase = 0;
    temp->period = 50;
  }
}