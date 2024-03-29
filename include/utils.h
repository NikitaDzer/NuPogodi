#pragma once

#include <stdint.h>
#include <stddef.h>

int32_t 
max( int32_t a, 
     int32_t b );

int32_t
min( int32_t a, 
     int32_t b );

int32_t
clamp( int32_t value,
       int32_t low,
       int32_t high );

void
qmemset( void *dst, 
         uint8_t value, 
         size_t n_bytes );

uint32_t 
qrand( void );

uint16_t 
rand16( void );

void 
timing_perfect_delay( uint32_t millis );

void
assert_impl( const char *filename, 
             long line, 
             const char *function );

#ifdef NDEBUG
#define assert( condition )
#else
#define assert( condition )                                     \
                                                                \
    do                                                          \
    {                                                           \
        if ( !(condition) )                                     \
        {                                                       \
            assert_impl( __FILE__, __LINE__, __FUNCTION__ );    \
        }                                                       \
    } while ( 0 )
        
#endif
