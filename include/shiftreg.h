/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://github.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
*/
#ifndef TLC591x_LIBRARY
#define TLC591x_LIBRARY

#include <Arduino.h>

class TLC591x {
public:
  enum {NO_PIN=255};

  TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE);
  TLC591x(byte n, byte SDI, byte CLK, byte LE);

  void print(const char* s);
  void print(unsigned int n);
  void displayEnable();
  void displayDisable();

private:
  enum POWER_MODE {WAKEUP = 1, SHUTDOWN = 0};
  enum {NO_DATA_COMING = 0, DATA_COMING = 1};
  enum {MINCHIPS = 1};
  byte SDI_pin, CLK_pin, LE_pin, OE_pin, numchips;

  void write(byte n);
  void toggleLE();
  void toggleCLK();
  void init();
};
#endif
