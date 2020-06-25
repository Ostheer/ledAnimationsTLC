/* 
  ADAPTED FROM:
   Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   adapted from https://github.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.      Original
   1.1.0    22/06/2020  Ostheer   Remove character mapping, add proper way to address an arbitrary number of leds
*/

#include "shiftreg.h"

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE) {
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin  = LE;
  OE_pin  = OE;
  numchips = n;

  digitalWrite(OE_pin, HIGH);
  pinMode(OE_pin, OUTPUT);
  init();
}

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE) {
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin  = LE;
  OE_pin  = NO_PIN;
  numchips = n;

  init();
}

void TLC591x::init() {
  if (numchips < MINCHIPS) numchips = MINCHIPS;

  pinMode(SDI_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  digitalWrite(LE_pin, LOW);
  pinMode(LE_pin, OUTPUT);
}

void TLC591x::print(const char* s) {
  for (int i = 0; i < numchips; i++) {
    write(s[i]);
  }
  toggleLE();
}

void TLC591x::print(unsigned int n){
  for (byte i = 0; i < numchips; i++) {
    write(n);
    n = n/256; // Get the next most significant byte
  }
  toggleLE();
}

void TLC591x::displayEnable() {
  if (OE_pin != NO_PIN) digitalWrite(OE_pin, LOW);
}

void TLC591x::displayDisable() {
  if (OE_pin != NO_PIN) digitalWrite(OE_pin, HIGH);
}

void TLC591x::write(byte n) {
  digitalWrite(SDI_pin, n & 0x01);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x02);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x04);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x08);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x10);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x20);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x40);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x80);
  toggleCLK();
}

void TLC591x::toggleLE() {
  // TLC5916 minimum LE pulse time is 20 ns, so don't need to worry about
  // putting in a hard-coded delay.
  digitalWrite(LE_pin, HIGH);
  digitalWrite(LE_pin, LOW);
}

void TLC591x::toggleCLK() {
  // TLC5916 minimum CLK pulse time is 20 ns, so don't need to worry about
  // putting in a hard-coded delay.
  digitalWrite(CLK_pin, HIGH);
  digitalWrite(CLK_pin, LOW);
}
