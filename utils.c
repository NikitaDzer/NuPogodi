#include <stdint.h>
#include "utils.h"
#include "lib.h"

int32_t 
max( int32_t a, 
     int32_t b )
{
    return a > b ? a : b;
}

int32_t
min( int32_t a, 
     int32_t b )
{
    return a > b ? b : a;
}

int32_t
clamp( int32_t value,
       int32_t low,
       int32_t high )
{
    if ( value < low )
    {
        return low;
    }

    if ( value > high )
    {
        return high;
    }
    
    return value;
}

void
assert_impl( const char *filename, 
             long line, 
             const char *function )
{
    enable_GPIOx_clock( GPIOC );

    set_GPIOx_MODER(
        GPIOC,
        PIN8,
        GENERAL_PURPOSE_OUTPUT_MODE
    );

    set_GPIOx_ODR( GPIOC, PIN8, 1 );

    while ( 1 )
    {
    }
}
