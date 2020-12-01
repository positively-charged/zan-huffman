/*
 * skulltag::BitReader class - Allows reading arbitrary bit lengths of data.
 * Version 1 - Revsion 0
 *
 * Copyright 2009 Timothy Landers
 * email: code.vortexcortex@gmail.com
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
 */

#include "bitreader.h"

// BitReader class implementation

/** Fills the internal bit buffer.
 * @return true if successful, false if an error occurs. */
static Bool fill( BitReader * );
/** Initializes this BitReader. */
static void init( BitReader * );

static int intBitSize = 0;
static int intSize = 0;
static int mask[32] = {0};

/** Creates a new BitReader. */
void BitReader_Init( BitReader * br ) {
   init( br );
}

/** Creates a new BitReader.
 * @param input      Source of data that bits will be read from.
 * @param max     Maximum number of chars to read. */
void BitReader_InitWithInput( BitReader * br, unsigned char const * input, 
   int max ){
   BitReader_InputBuffer( br, input, max );
}
   
/** Sets the input buffer that bytes will be read from.
 * @param input      Source of data that bits will be read from.
 * @param max     Maximum number of chars to input.
 * @return true if successful, false if an error occurs. */
Bool BitReader_InputBuffer( BitReader * br, unsigned char const * input,
   int max ){
   init( br ); // zero the vars.
   br->currentByte = input;
   if ( input == 0 ) return false;
   if ( max < 1 ) return false;
   br->bytesAvailable = max;
   br->bitsAvailable = max << 3;
   br->maximumBytes = max;
   return true;
}

/** Initializes this BitReader */
void init( BitReader * br ){
   // initialize static variables if not initialized yet.
   if ( intSize == 0 ){
      intSize = sizeof br->maximumBytes;
      mask[0] = 0;

      // fill mask such that m = { 0, 1, 3, 7, 15, etc. }
      for ( int i = 1; i < 32; i++ ) mask[i] = (mask[i-1] << 1) | 1;
      
      intBitSize = intSize << 3;
   }

   // initialize member variables.
   br->bitsAvailable =      0;
   br->bytesAvailable =  0;
   br->bufferBits =      0;
   br->currentByte =     0;
   br->maximumBytes =    0;
   br->bitsUsed =        0;
}

/** Fills the internal bit buffer.
 * @return true if successful, false if an error occurs. */
Bool fill( BitReader* br ){
   if ( (br->currentByte == 0) || (br->bytesAvailable <= 0) ) return false;

   // while there's at least one octet free in the buffer, and one byte to read.
   while ( (br->bitsUsed < (intBitSize - 8)) && (br->bytesAvailable > 0) ){

      // put a byte into the bottom end of the buffer.
      br->bufferBits |= (*br->currentByte & mask[8]) << (intBitSize - br->bitsUsed - 8);

      // Set variables to reflect the change.
      br->currentByte++;
      br->bytesAvailable--;
      br->bitsUsed += 8;
   }
   return true;
}

/** Fetches a specified number of bits and stores them in an int.
 * @param bits       destination of the retrieved bits. <br>
 *    The bits will be stored in the least significant portion of the int.
 * @param count      the number of bits to fetch.
 * @return the number of bits read -- may not be equal to the amount requested. */
int BitReader_Get( BitReader * br, int * bits, int count ){
   *bits = 0;
   // Requesting more bits than are available.
   if ( count > br->bitsAvailable ) return 0;
   if ( (count > br->bitsUsed) && (!fill( br )) ) return 0;
   *bits = (br->bufferBits >> (intBitSize - count)) & mask[count];
   // lesser of bits in buffer or requested bits.
   int got = (br->bitsUsed < count) ? br->bitsUsed : count;
   // get as many bits from the buffer as we can.
   if ( got > 0 ){
      br->bufferBits <<= got;
      br->bitsUsed -= got;
      br->bitsAvailable -= got;
   }
   // if more bits are requested.
   if ( count > got ){
      if (!fill( br )) {
         *bits = (*bits >> (count - got)) & mask[count - got];
         return got;
      }
      got = count - got;
      // avoid reading more bits than available.
      if ( got <= br->bitsAvailable ){
         *bits |= (br->bufferBits >> (intBitSize - got)) & mask[got];
         br->bufferBits <<= got;
         br->bitsUsed -= got;
         br->bitsAvailable -= got;
      }
   }
   return count;
}

/** @return Amount of bits that can be read from this BitReader. */
int BitReader_AvailableBits( BitReader* br ){ return br->bitsAvailable; }
