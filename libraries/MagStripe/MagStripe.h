/*
 * MagStripe - Read data from a magnetic stripe card.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 * Modified by Marc Davis - March 12, 2021
 * This version only supports insert-type card readers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Based on this: http://cal.freeshell.org/2009/11/magnetic-stripe-reader/
 *   ...and this: http://www.abacus21.com/Magnetic-Strip-Encoding-1586.html
 */


#ifndef MAGSTRIPE_H
#define MAGSTRIPE_H


#if ARDUINO < 100
#  include <WProgram.h>
#else
#  include <Arduino.h>
#endif


// The data and clock pins depend on the board model...
#if defined(__AVR_ATmega32U4__)
   // Arduino Leonardo and Arduino Micro:
#  define MAGSTRIPE_RDT 21  /* data pin */
#  define MAGSTRIPE_RCL 20  /* clock pin */
#else
   // Arduino Uno and Arduino Mega:
#  define MAGSTRIPE_RDT 20  /* data pin */
#  define MAGSTRIPE_RCL 21  /* clock pin */
#endif

#define MAGSTRIPE_CLS 8  /* card present pin */
#define MAGSTRIPE_CLD 9  /* card fully inserted pin */


// Cards can be read in one of these possible ways...
enum ReadDirection { READ_UNKNOWN, READ_FORWARD, READ_BACKWARD };


class MagStripe {
    public:
        // The CLS pin can be changed from the default...
        MagStripe(unsigned char cls=MAGSTRIPE_CLS, unsigned char cld=MAGSTRIPE_CLD);

        // Initialize the library (attach interrupts)...
        void begin(unsigned char track);

        // Deinitialize the library (detach interrupts)...
        void stop();

        // Check if there is a card present for reading...
        bool available();

        // Check if there is a card fully inserted for reading...
        bool available2();

        // Read the data from the card as ASCII...
        short read(char *data, unsigned char size);

        // The direction of the last card read...
        enum ReadDirection read_direction();

    private:
        unsigned char pin_cls;
        unsigned char pin_cld;
        unsigned char track;
        enum ReadDirection direction;

        void reverse_bits();
        bool verify_parity(unsigned char c);
        bool verify_lrc(short start, short length);
        short find_sentinel(unsigned char pattern);
        short decode_bits(char *data, unsigned char size);
};


#endif  /* MAGSTRIPE_H */


/* vim: set expandtab ts=4 sw=4: */
