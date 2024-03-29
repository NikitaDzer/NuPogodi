#include <stdint.h>

#include "../include/utils.h"
#include "../include/lib.h"

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
qmemset( void *dst, 
         uint8_t value, 
         size_t n_bytes )
{
    assert( dst );

    uint8_t *data = (uint8_t *)dst;

    for ( size_t i = 0; i < n_bytes; i++ )
    {
        data[ i ] = value;
    }
}

void
assert_impl( [[maybe_unused]] const char *filename, 
             [[maybe_unused]] long line, 
             [[maybe_unused]] const char *function )
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

uint32_t 
qrand( void ) 
{
    static uint32_t seed = 0;

    seed = seed * 1103515245 + 12345;
    return seed / 65536;
}

uint16_t 
rand16( void )
{
    return get_TIM3_counter();
}

void 
timing_perfect_delay( uint32_t millis )
{
    #define ONE_MILLISECOND 48000U
    volatile unsigned ticks = millis * (ONE_MILLISECOND);

    __asm__ volatile (
        "1: ldr r3, [r7, #12] \n\t"//2
        "cmp r3, #0 \n\t"//1
        "beq 2f \n\t" //1
        "sub r3, #1 \n\t" //1
        "str r3, [r7, #12] \n\t" //2
        "b 1b \n\t" //3
        "2: \n\t"
    );

    ticks+=0;
}
