#include "include/macro.h"
#include "include/rcc_regs.h"
#include "include/usart_regs.h"
#include "include/gpio_regs.h"
#include "include/bitmask.h"
#include <stdbool.h>
#include <stddef.h>

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

    *gUsart1Cr1 = (UsartCr1RegBits) { .over8 = false, .pce = false, .te = true };
    // Endianness: LSB first
    // Number of stop bits: 2 stop bit
    *gUsart1Cr2 = (UsartCr2RegBits) { .msbfirst = false, .stop = 0b10 };

    // (3) Configure USART baud rate:
    gUsart1Brr->whole_reg = ( frequency + baudrate / 2 ) / baudrate;

    // (4) Enable UART:
    gUsart1Cr1->ue = true;

    // (5) Wait for TX to enable:
    while ( !gUsart1Isr->teack ) ; 
}


void
uart_send_byte( char sym)
{
    // Wait for TXE:
    while ( !gUsart1Isr->txe ) ;

    // Put data into DR:
    gUsart1Tdr->tdr = sym;
}

void
print_string( const char* buf)
{
    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        uart_send_byte(buf[i]);
    }
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

    print_string("Hello, world!\r");

    while ( 1 )
    {
    }
}
