/*  MCP23017 library for Arduino
    Copyright (C) 2009 David Pye    <davidmpye@gmail.com
    Modified for use on the MBED ARM platform

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef     MBED_MCP23017_H
#define     MBED_MCP23017_H

#include    "mbed.h"

//
// Register defines from data sheet - we set IOCON.BANK to 0
// as it is easier to manage the registers sequentially.
//
#define     IODIR       0x00
#define     IPOL        0x02
#define     GPINTEN     0x04
#define     DEFVAL      0x06
#define     INTCON      0x08
#define     IOCON       0x0A
#define     GPPU        0x0C
#define     INTF        0x0E
#define     INTCAP      0x10
#define     GPIO        0x12
#define     OLAT        0x14

#define     I2C_BASE_ADDRESS    0x40

#define     DIR_OUTPUT      0
#define     DIR_INPUT       1

/** MCP23017 class
 *
 * Allow access to an I2C connected MCP23017 16-bit I/O extender chip
 * Example:
 * @code
 *      MCP23017     *par_port; 
 * @endcode
 *
 */
class MCP23017 {
public:
    /** Constructor for the MCP23017 connected to specified I2C pins at a specific address
     *
     * 16-bit I/O expander with I2C interface
     *
     * @param   sda         I2C data pin
     * @param   scl         I2C clock pin
     * @param   i2cAddress  I2C address
     */
    MCP23017(PinName sda, PinName scl, int i2cAddress);

    /** Reset MCP23017 device to its power-on state
     */    
    void reset(void);

    /** Write a 0/1 value to an output bit
     *
     * @param   value         0 or 1
     * @param   bit_number    bit number range 0 --> 15
     */   
    void write_bit(int value, int bit_number);
      
    /** Write a masked 16-bit value to the device
     *
     * @param   data    16-bit data value
     * @param   mask    16-bit mask value
     */       
    void write_mask(unsigned short data, unsigned short mask);

    /** Read a 0/1 value from an input bit
     *
     * @param   bit_number    bit number range 0 --> 15
     * @return                0/1 value read
     */       
    int  read_bit(int bit_number);
    
    /** Read a 16-bit value from the device and apply mask
     *
     * @param   mask    16-bit mask value
     * @return          16-bit data with mask applied
     */     
    int  read_mask(unsigned short mask);

    /** Configure an MCP23017 device
     *
     * @param   dir_config         data direction value (1 = input, 0 = output)
     * @param   pullup_config      100k pullup value (1 = enabled, 0 = disabled)
     * @param   polarity_config    polarity value (1 = flip, 0 = normal)
     */           
    void config(unsigned short dir_config, unsigned short pullup_config, unsigned short polarity_config);

    void writeRegister(int regAddress, unsigned char  val);
    void writeRegister(int regAddress, unsigned short val);
    int  readRegister(int regAddress);

/*----------------------------------------------------------------------------- 
 * pinmode
 * Set units to sequential, bank0 mode
 */  
    void pinMode(int pin, int mode); 
    void digitalWrite(int pin, int val);
    int  digitalRead(int pin);

// These provide a more advanced mapping of the chip functionality
// See the data sheet for more information on what they do

//Returns a word with the current pin states (ie contents of the GPIO register)
    unsigned short digitalWordRead();
// Allows you to write a word to the GPIO register
    void digitalWordWrite(unsigned short w);
// Sets up the polarity mask that the MCP23017 supports
// if set to 1, it will flip the actual pin value.
    void inputPolarityMask(unsigned short mask);
//Sets which pins are inputs or outputs (1 = input, 0 = output) NB Opposite to arduino's
//definition for these
    void inputOutputMask(unsigned short mask);
// Allows enabling of the internal 100k pullup resisters (1 = enabled, 0 = disabled)
    void internalPullupMask(unsigned short mask);
    int read(void);
    void write(int data);

protected:
    I2C     _i2c;
    int     MCP23017_i2cAddress;                        // physical I2C address
    unsigned short   shadow_GPIO, shadow_IODIR, shadow_GPPU, shadow_IPOL;     // Cached copies of the register values
    
};

#endif