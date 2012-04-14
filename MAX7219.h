/* Arduino MAX7219/7221 Library
 * See the README file for author and licensing information. In case it's
 * missing from your distribution, use the one here as the authoritative
 * version: https://github.com/csdexter/MAX7219/blob/master/README
 *
 * This library is for use with Maxim's MAX7219 and MAX7221 LED driver chips.
 * See the example sketches to learn how to use the library in your code.
 *
 * This is the main include file for the library.
 * ---------------------------------------------------------------------------
 * The header of the original file follows:
 *
 * LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 * Copyright (c) 2007 Eberhard Fahle
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _MAX7219_H_INCLUDED
#define _MAX7219_H_INCLUDED

#if defined(ARDUINO) && ARDUINO >= 100
# include <Arduino.h>  
#else
# include <WProgram.h>  
#endif

//Assign the SPI pin numbers
//DIN and CLK always connected to MOSI and SCK
#define MAX7219_PIN_LOAD SS

//Define MAX7219 Register codes
#define MAX7219_REG_NOOP 0x00
#define MAX7219_REG_DIGIT0 0x01
#define MAX7219_REG_DIGIT1 0x02
#define MAX7219_REG_DIGIT2 0x03
#define MAX7219_REG_DIGIT3 0x04
#define MAX7219_REG_DIGIT4 0x05
#define MAX7219_REG_DIGIT5 0x06
#define MAX7219_REG_DIGIT6 0x07
#define MAX7219_REG_DIGIT7 0x08
#define MAX7219_REG_DECODEMODE 0x09
#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_SCANLIMIT 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

//Define MAX7219 Flags
#define MAX7219_FLG_SEGDP 0x80
#define MAX7219_FLG_SEGA 0x40
#define MAX7219_FLG_SEGB 0x20
#define MAX7219_FLG_SEGC 0x10
#define MAX7219_FLG_SEGD 0x08
#define MAX7219_FLG_SEGE 0x04
#define MAX7219_FLG_SEGF 0x02
#define MAX7219_FLG_SEGG 0x01
#define MAX7219_FLG_DIGIT0_RAW 0x00
#define MAX7219_FLG_DIGIT0_CODEB 0x01
#define MAX7219_FLG_DIGIT1_RAW 0x00
#define MAX7219_FLG_DIGIT1_CODEB 0x02
#define MAX7219_FLG_DIGIT2_RAW 0x00
#define MAX7219_FLG_DIGIT2_CODEB 0x04
#define MAX7219_FLG_DIGIT3_RAW 0x00
#define MAX7219_FLG_DIGIT3_CODEB 0x08
#define MAX7219_FLG_DIGIT4_RAW 0x00
#define MAX7219_FLG_DIGIT4_CODEB 0x10
#define MAX7219_FLG_DIGIT5_RAW 0x00
#define MAX7219_FLG_DIGIT5_CODEB 0x20
#define MAX7219_FLG_DIGIT6_RAW 0x00
#define MAX7219_FLG_DIGIT6_CODEB 0x40
#define MAX7219_FLG_DIGIT7_RAW 0x00
#define MAX7219_FLG_DIGIT7_CODEB 0x80
#define MAX7219_FLG_SHUTDOWN 0x01
#define MAX7219_FLG_DISPLAYTEST 0x01

//Define per-digit operation modes
#define MAX7219_MODE_7SEGMENT 0x01
#define MAX7219_MODE_MATRIX 0x02
#define MAX7219_MODE_BARGRAPH 0x03
//Don't touch this digit
#define MAX7219_MODE_OFF 0x04
//Don't scan this digit
#define MAX7219_MODE_NC 0x05

typedef struct _MAX7219_Topology {
    byte ID;
    byte length;
    union {
        struct _MAX7219_Topology *list;
        byte *data;
    }
} MAX7219_Topology;

#define MAX7219_7SEGMENT_TOPOLOGY {.ID = MAX7219_MODE_7SEGMENT, \
                                   .length = 1, \
                                   .list = {.ID = 0, \
                                            .length = 8, \
                                            .data = {0, 1, 2, 3, 4, 5, 6, 7} \
                                            } \
                                   }
#define MAX7219_MATRIX_TOPOLOGY {.ID = MAX7219_MODE_MATRIX, \
                                 .length = 1, \
                                 .list = {.ID = 0, \
                                          .length = 8, \
                                          .data = {0, 1, 2, 3, 4, 5, 6, 7} \
                                          } \
                                 }
#define MAX7219_BARGRAPH_TOPOLOGY {.ID = MAX7219_MODE_BARGRAPH, \
                                   .length = 1, \
                                   .list = {.ID = 0, \
                                            .length = 8, \
                                            .data = {0, 1, 2, 3, 4, 5, 6, 7} \
                                            } \
                                   }
#define MAX7219_DEFAULT_TOPOLOGY MAX7219_7SEGMENT_TOPOLOGY

class MAX7219 
{
    public:
        /*
        * Description:
        *   This is the constructor, it creates a new MAX7219 driver chain.
        *   You will need one instance of this class for each string of chips
        *   which share a LOAD/#CS pin.
        * Parameters:
        *   pinLOAD - digital pin to which LOAD/#CS is wired to, defaults to
        *             SPI SS
        */
        MAX7219(byte pinLOAD = MAX7219_PIN_LOAD) { _pinLOAD = pinLOAD };

        /*
        * Description:
        *   This is the destructor, it simply calls end().
        */
        ~MAX7219() { end() };
        
        /* 
        * Description:
        *   Sets the topology of the display.
        * Parameters:
        *   topology - topology to use, default topology assumed if ignored
        *   length   - number of topology elements described
        */
        void begin(const MAX7219_Topology *topology = NULL, 
                   const byte length = 1);

        /*
        * Description:
        *    Clears the SRAM and sends a shutdown command to the MAX7219(s).
        */
        void end(void);

        /*
        * Description:
        *   Gets the total number of devices attached to this driver, as
        *   extrapolated from the current topology.
        */
        byte getChipCount(void) { return _chips };
        
        /* 
        * Description:
        *   Sets the selected chip to shutdown/powered mode
        * Parameters:
        * 	chip - the index of the chip to control
        */
        void shutdown(byte chip = 0) {
            writeRegister(MAX7219_REG_SHUTDOWN, MAX7219_FLG_SHUTDOWN, chip);
        };
        void noShutdown(byte chip = 0) {
            writeRegister(MAX7219_REG_SHUTDOWN, 0x00, chip);
        };

        /* 
        * Description:
        *   Sets the selected chip to display test/normal mode
        * Parameters:
        * 	chip - the index of the chip to control
        */
        void displayTest(byte chip = 0) {
            writeRegister(MAX7219_REG_DISPLAYTEST, MAX7219_FLG_DISPLAYTEST, 
                          chip);
        };
        void noDisplayTest(byte chip = 0) {
            writeRegister(MAX7219_REG_DISPLAYTEST, 0x00, chip);
        };

        /* 
        * Description:
        *   Set the number of digits, bargraph columns or matrix rows to be
        *   scanned by the selected chip. See datasheet for side effects of
        *   the scan limit on the brightness of the display.
        * Parameters:
        *   limit - number of digits to be scanned-1 (0..7)
        *   chip  - the index of the chip to control
        */
        void setScanLimit(byte limit, byte chip = 0) {
            writeRegister(MAX7219_REG_SCANLIMIT, limit, chip);
        };

        /* 
        * Description:
        *   Set the brightness of the display.
        * Parameters:
        *   intensity - the brightness of the display (0..15)
        *   addr      - the index of the chip to control
        */
        void setIntensity(byte intensity, byte chip = 0) {
            writeRegister(MAX7219_REG_INTENSITY, intensity, chip);
        };

        /* 
        * Description:
        *   Switch all LEDs belonging to the given topology element off. 
        * Parameters:
        *   topo - the index of the topology element to switch off
        */
        void clearDisplay(byte topo = 0);
        
        /*
        * Description:
        *   Reset topology element to a meaningful zero. This involves
        *   displaying an actual 0 on 7-segment displays, turning only the
        *   first bottom line on on bargraphs and displaying just a single
        *   pixel in the bottom left corner on matrices.
        */
        void zeroDisplay(byte topo = 0);
        
        /* 
        * Description:
        *   Displays the given number on the given topology element, 
        *   previously configured as a 7-segment display.
        * Parameters:
        *   number - [0-9-EeHhLlPp ]. Set bit 7 on any character whose
        *            corresponding digit should have DP on.
        *   topo   - topology element to update (must be 7-segment)
        */
        void set7Segment(char *number, byte topo = 0);
        
        /* 
        * Description:
        *   Displays the given bar/dot values on the given topology element, 
        *   previously configured as a bargraph display.
        * Parameters:
        *   values - [0, 8]
        *   dot    - use dot instead of bar mode
        *   topo   - topology element to update (must be bargraph)
        */
        void setBarGraph(byte *values, boolean dot = false, byte topo = 0);
        
        /*
        * Description:
        *   Displays the given pixel values on the given topology element, 
        *   previously configured as a matrix.
        * Parameters:
        *   values - [0, 0xFF]
        *   topo   - topology element to update (must be matrix)
        */
        void setMatrix(byte *values, byte topo = 0);

    private:
        MAX7219_Topology *_topology;
        byte _pinLOAD, _elements, _chips;
        
        /*
        * Description:
        *   Write to one of the chip registers, on a single chip, via SPI.
        */
        void writeRegister(byte addr, byte value, byte chip = 0);

        /*
        * Description:
        *   Write to one of the chip registers, on multiple chips, via SPI.
        * Parameters:
        *   registers - data to be written
        *   size      - length of data to be written
        *   chip      - chip index to start writing at
        */
        void writeRegisters(word *registers, byte size, byte chip = 0);
        
        /*
        * Descriptions:
        *   Sets consecutive digits in a topology element to the given raw
        *   values.
        */
        void setDigits(byte *values, byte topo = 0);
        
        /*
        * Description:
        *   Counts the number of digits spanned by a topology element.
        */
        word getDigitCount(byte topo = 0);
};

#endif
