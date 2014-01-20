#ifndef BINARYUTILITIES_H
#define BINARYUTILITIES_H

#include <cstring>
#include <stdint.h>

/**
 * @class BinaryUtilities
 * @brief Class with utilities to read basic types from a buffer
 */
class BinaryUtilities
{
   public:
      /**
       * Compare part of the buffer
       * @param buffer: the buffer
       * @param pos: the position of the buffer
       * @param compareTo: the bytestring to compare to
       * @param len: the length of compareTo
       */
      static bool compare( const char* buffer, size_t pos, const char* compareTo, size_t len );

      /**
       * Read a byte from buffer at @param pos
       */
      static char readByte( const char* buffer, size_t pos );
      /**
       * Read a short from buffer at @param pos
       */
      static short readShort( const char* buffer, size_t pos );
      /**
       * Read an int from buffer at @param pos
       */
      static int readInt( const char* buffer, size_t pos );

      /**
       * Write a byte, @param value, to buffer @param buffer at position @param pos
       */
      static void writeByte( char* buffer, size_t pos, char value );
      /**
       * Write a short, @param value, to buffer @param buffer at position @param pos
       */
      static void writeShort( char* buffer, size_t pos, short value );
      /**
       * Write an int, @param value, to buffer @param buffer at position @param pos
       */
      static void writeInt( char* buffer, size_t pos, int value );
      /**
       * Write a zero-terminated c-string, @param string, to buffer @param buffer at position @param pos
       */
      static void writeCString( char* buffer, size_t pos, const char* string );
};

/// apparently not needed
// //! Byte swap unsigned int
// inline uint32_t swap_uint32( uint32_t val )
// {
//   std::cout << "val = " << val << std::endl;
//     val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
//     return (val << 16) | (val >> 16);
// }
//
// //! Byte swap int
// inline int32_t swap_int32( int32_t val )
// {
//     val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
//     return (val << 16) | ((val >> 16) & 0xFFFF);
// }

#endif // BINARYUTILITIES_H
