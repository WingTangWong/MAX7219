/*
* MAX7219 BarGraph Example Sketch
* Written by Radu - Eosif Mihailescu
*
* This example sketch illustrates how to use some of the basic commands in the
* MAX7219 Library. The sketch will use the MAX7219/7221 to display some
* levels on a bar graph display.
* More information on the MAX7219/7221 chips can be found in the datasheet.
*
* HARDWARE SETUP:
* This sketch assumes you have connected your MAX7219/7221 to the SPI
* interface of the Arduino as explained in the README file. Four bar graph 
* displays should be connected to the first 4 digits of the MAX7219/7221 in
* common cathode mode (all cathodes of a single display connect together and 
* to the corresponding DIG input of the MAX7219/7221). All anodes are then 
* connected together across all displays and to the corresponding SEG output 
* of the MAX7219/7221. Finally, a current limiting resistor should be 
* connected between ISET and V+, whose value can be determined from MAX7219's
* and the display's datasheets.
* 
* BARGRAPH WIRING DIAGRAM:
* For those of you who appreciate images more than words, here it goes:
* 
*  SEGDP ----|>|---\  SEGDP ----|>|---\  SEGDP ----|>|---\  SEGDP ----|>|---\ 
*  SEGA  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---|
*  SEGB  ----|>|---|  SEGB  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---| 
*  SEGC  ----|>|---|  SEGC  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---|
*  SEGD  ----|>|---|  SEGD  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---| 
*  SEGE  ----|>|---|  SEGE  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---| 
*  SEGF  ----|>|---|  SEGF  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---| 
*  SEGG  ----|>|---|  SEGG  ----|>|---|  SEGA  ----|>|---|  SEGA  ----|>|---| 
*                  |                  |                  |                  |
*               DIG0               DIG1               DIG2               DIG3
*
* USING THE SKETCH:
* Compile, upload, enjoy :-)
*
*/

//Due to a bug in Arduino, this needs to be included here too/first
#include <SPI.h>

#include <MAX7219.h>

const MAX7219_Topology topology[2] = {{MAX7219_MODE_BARGRAPH, 0, 0, 0, 3},
                                      {MAX7219_MODE_NC, 0, 4, 0, 7}};
/* we always wait a bit between updates of the display */
const byte delaytime = 250;

MAX7219 maxled;

void setup() {
  maxled.begin(topology, 2);
  maxled.zeroDisplay();
}

/* This will scroll a wave across the display, in both bar/dot mode */
void scrollWave(bool mode){
  byte framebuffer[4], peak, oldpeak = 0;
  signed char slope;
  
  for(byte i = 0; i < 16; i++) {
    peak = oldpeak;
    for(byte j = 0; j < 4; j++) {
      if(peak == 8) slope = -1;
      if(peak == 0) slope = 1;
      framebuffer[j] = peak;
      if(j == 1) oldpeak = peak;
      peak += slope;
    }
    maxled.setBarGraph(framebuffer, mode);
    delay(delaytime);
  }
}

void loop() {
  scrollWave(true);
  delay(delaytime);
  scrollWave(false);
}
