#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "huffman.h"

void test_string( const char* string );

int main( void ) {
   HUFFMAN_Construct();

   test_string( "" );
   test_string( "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+" );
   test_string(
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+\n"
      "abcdefghijklmnopqrstuvwxyz123456789!@#$%^&*()_+"
   );

   HUFFMAN_Destruct();
   return 0;
}

void test_string( const char* string ) {
   enum { BUFFER_SIZE = 1000 };

   unsigned char encoded[ BUFFER_SIZE ];
   unsigned char decoded[ BUFFER_SIZE ];

   // We include the NUL byte in the length.
   size_t length = strlen( string ) + 1;
   assert( length <= BUFFER_SIZE );

   memcpy( decoded, string, length );

   int encodedLength = BUFFER_SIZE;
   HUFFMAN_Encode( decoded, length, encoded, &encodedLength );

   int decodedLength = BUFFER_SIZE;
   HUFFMAN_Decode( encoded, encodedLength, decoded, &decodedLength );

   assert( decodedLength == length );
   assert( memcmp( string, decoded, decodedLength ) == 0 );

   printf( "%s\n", decoded );
}