#include "include/macro.h"
#include "include/rcc_regs.h"
#include "include/usart_regs.h"
#include "include/gpio_regs.h"
#include "include/bitmask.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/*
#include "core/stm32f0xx_ll_rcc.h"
#include "core/stm32f0xx_ll_system.h"
#include "core/stm32f0xx_ll_bus.h"
#include "core/stm32f0xx_ll_gpio.h"
#include "core/stm32f0xx_ll_usart.h"
*/

/**
 * BUFFER_SIZE must be at least 2.
 */
#define BUFFER_SIZE 64

void
board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    gRccCr->hseon = true;
    while ( !gRccCr->hserdy ) ;

    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    gRccCfgr2->prediv = toPreDiv( 2 );

    // (3) Select PREDIV output as PLL input (4 MHz):
    // (4) Set PLLMUL to 12:
    gRccCfgr->pllsrc = PLL_SRC_HSE_PREDIV;
    gRccCfgr->pllmul = toPllMul( 12 );

    // (5) Enable PLL:
    gRccCr->pllon = true;
    while ( !gRccCr->pllrdy ) ;

    // (6) Configure AHB frequency to 48 MHz:
    gRccCfgr->hpre = AHB_DIV_FACTOR_DIV_NO;

    // (7) Select PLL as SYSCLK source:
    gRccCfgr->sw = SYSCLOCK_SOURCE_PLL;
    while ( gRccCfgr->sws != SYSCLOCK_SOURCE_PLL ) ;

    // (8) Set APB frequency to 24 MHz
    gRccCfgr->ppre = APB_DIV_FACTOR_DIV_NO;
}

void
board_gpio_init()
{
    // (1) Configure PC8 as LED
    gRccAhbenr->iopcen = true;

    // Configure PC8 mode:
    gGpioCModer->whole_reg |= SET_2BIT( 8, MODER_PIN_MODE_GPO);

    // Configure PC8 type:
    gGpioCTyper->whole_reg &= CLEAR_1BIT( 8 );

    // (2) Configure pins PA9 and PA10 for UART
    gRccAhbenr->iopaen = true;

    // set 9 and 10 pin alternate function
    gGpioAAfhdr->whole_reg |= RANGE_SET_4BIT( 1, 2, AF1 );

    // Configure pin operating speed:
    gGpioAOspeedr->whole_reg |= RANGE_SET_2BIT( 9, 10, OSPEEDR_HIGHT );

    // Configure mode register:
    gGpioAModer->whole_reg |= RANGE_SET_2BIT( 9, 10, MODER_PIN_MODE_ALTERNATE );
}

void
uart_init( size_t baudrate,
	   size_t frequency)
{
    // (1) Configure USART1 clocking:
    gRccApb2enr->usart1en = true;
    gRccCfgr3->usart1sw = USART_SOURCE_PCLK;
    
    // (2) Set USART1 parameters:
    // Data length: 8 bits
    // Use oversampling by 16
    // Parity control: disabled
    // Transmitter: enabled
    *gUsart1Cr1 = (UsartCr1RegBits) { .over8 = false, .pce = false, .te = true, .re = true };

    // Endianness: LSB first
    // Number of stop bits: 2 stop bit
    *gUsart1Cr2 = (UsartCr2RegBits) { .msbfirst = false, .stop = 0b10 };

    // (3) Configure USART baud rate:
    gUsart1Brr->whole_reg = ( frequency + baudrate / 2 ) / baudrate;

    // (4) Enable UART:
    gUsart1Cr1->ue = true;

    // (5) Wait for TX and RX to enable:
    while ( !gUsart1Isr->teack ||
            !gUsart1Isr->reack ) ; 
    while ( !gUsart1Isr->reack ) ; 

    /*
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);
    */
}

void
uart_send_byte( char sym)
{
    // Wait for TXE:
    while ( !gUsart1Isr->txe ) ;

    // Put data into DR:
    gUsart1Tdr->tdr = sym;

    while ( !gUsart1Isr->tc ) ;

    /*
    while (!LL_USART_IsActiveFlag_TXE(USART1));
    LL_USART_TransmitData8(USART1, sym);
    while (!LL_USART_IsActiveFlag_TC(USART1));
    */
}

uint8_t
uart_receive_data( void )
{
    while ( !gUsart1Isr->rxne );

    return gUsart1Rdr->rdr;

}

void
print_string( volatile const uint8_t* buf)
{
    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        uart_send_byte(buf[i]);
    }
}


typedef struct Buffer
{
    uint8_t data[ BUFFER_SIZE ];
    volatile uint8_t *position;
} Buffer; 


static bool
buffer_is_last_char( volatile Buffer *buffer )
{
    return buffer->position == buffer->data + BUFFER_SIZE - 1;
}

static void 
buffer_push( volatile Buffer *buffer, uint8_t data )
{ 
    *buffer->position = data;

    if ( !buffer_is_last_char( buffer ) )
    {
        buffer->position++;
    }
}

static void
buffer_clear( volatile Buffer *buffer )
{
    memset( buffer->data, 0, BUFFER_SIZE );
    buffer->position = buffer->data;
}

static void
buffer_complete( volatile Buffer *buffer )
{
    if ( buffer_is_last_char( buffer ) )
    {
        buffer->position--;
    }

    buffer_push( buffer, '\r' );
    buffer_push( buffer, '\0' );
}



#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

#define UART_BAUDRATE 9600
#define UART_BAUDRATE_FIX -300

int main()
{
    board_clocking_init();
    board_gpio_init();
    uart_init(UART_BAUDRATE + UART_BAUDRATE_FIX, CPU_FREQENCY);


Buffer usart_buffer = 
{ 
    .data = { 0 }, 
    .position = usart_buffer.data
};

    while ( 1 )
    {
        uint8_t data = uart_receive_data();

        if ( data == '\r' )
        {
            buffer_complete( &usart_buffer );
            print_string( usart_buffer.data );
            buffer_clear( &usart_buffer );
        } else
        {
            buffer_push( &usart_buffer, data );
        }
    }
}
