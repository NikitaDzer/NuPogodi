#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "lib.h"

/**
 * BUFFER_SIZE must be at least 2.
 */
#define BUFFER_SIZE 64

//---------------
// RCC Registers
//---------------

#define REG_RCC_CR      (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR    (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR  (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_APB2ENR (volatile uint32_t*)(uintptr_t)0x40021018U // APB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2   (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2
#define REG_RCC_CFGR3   (volatile uint32_t*)(uintptr_t)0x40021030U // Clock configuration register 2

//----------------
// GPIO Registers
//----------------

#define GPIOA_MODER   (volatile uint32_t*)(uintptr_t)0x48000000U // GPIO port mode register
#define GPIOA_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000008U // GPIO port output speed register
#define GPIOA_AFRH    (volatile uint32_t*)(uintptr_t)0x48000024U // GPIO alternate function high register

#define GPIOC_MODER   (volatile uint32_t*)(uintptr_t)0x48000800U // GPIO port mode register
#define GPIOC_TYPER   (volatile uint32_t*)(uintptr_t)0x48000804U // GPIO port output type register

//----------------
// UART Registers
//----------------

#define USART1_CR1 (volatile uint32_t*)(uintptr_t)0x40013800U // Control register 1
#define USART1_CR2 (volatile uint32_t*)(uintptr_t)0x40013804U // Control register 2
#define USART1_BRR (volatile uint32_t*)(uintptr_t)0x4001380CU // Baud rate register
#define USART1_ISR (volatile uint32_t*)(uintptr_t)0x4001381CU // Interrupt and status register
#define USART1_TDR (volatile uint32_t*)(uintptr_t)0x40013828U // Transmit data register

//-------------------
// RCC configuration
//-------------------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    enable_HSE_clock(); 

    // (2) Configure PLL:
    // (3) Select PREDIV output as PLL input (4 MHz):
    // (4) Set PLLMUL to 12:
    // (5) Enable PLL:
    set_PLL_prescaler( PREDIV_DIVIDED_BY_2 );
    set_PLL_source( HSE_DIVIDED_BY_PREDIV );
    set_PLL_multiplier( PLL_MULTIPLIED_BY_12 );
    enable_PLL_clock();

    // (6) Configure AHB frequency to 48 MHz:
    set_AHB_prescaler( SYSCLK_NOT_DIVIDED );

    // (7) Select PLL as SYSCLK source:
    select_SYSCLK_source( SYSCLK_SOURCE_PLL );

    // (8) Set APB frequency to 48 MHz
    set_APB_prescaler( HCLK_NOT_DIVIDED );
}

//--------------------
// GPIO configuration
//--------------------
void board_gpio_init()
{
    // (2) Configure pins PA9 and PA10 for UART
    enable_GPIOx_clock( GPIOA );

    // Set alternate functions:
    set_range_GPIOx_AFRH(
        GPIOA,
        PIN9, PIN10,
        AF1
    );

    // Configure pin operating speed:
    set_range_GPIOx_OSPEEDR(
        GPIOA,
        PIN9, PIN10,
        HIGH_SPEED
    );

    // Configure mode register:
    set_range_GPIOx_MODER(
        GPIOA,
        PIN9, PIN10,
        ALTERNATE_FUNCTION_MODE
    );
}

//--------------------
// GPIO configuration
//--------------------
void 
uart_init( uint32_t baudrate, uint32_t frequency )
{
    // (1) Configure USART1 clocking:
    enable_APB2_peripheral_clock( USART1EN );
    select_USART1_source( USARTx_SOURCE_PCLK );

    // (2) Set USART1 parameters:
    set_USARTx_word_length   ( USART1, USARTx_WORD_LENGTH_8BIT   );
    set_USARTx_oversampling  ( USART1, USARTx_OVERSAMPLING_BY_16 );
    disable_USARTx_parity    ( USART1 );
    enable_USARTx_transmitter( USART1 );
    enable_USARTx_receiver   ( USART1 );

    set_USARTx_endianness( USART1, USARTx_ENDIANNESS_LSB );
    set_USARTx_stop_bit  ( USART1, USARTx_STOP_2BIT );

    // (3) Configure USART baud rate:
    set_USARTx_baudrate( USART1, baudrate, frequency );

    // (4) Enable UART:
    enable_USARTx_peripheral( USART1 );

    // (5) Wait for TX to enable:
    wait_USARTx_transmit_enabled( USART1 );
    wait_USARTx_receive_enabled ( USART1 );
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

    buffer_push( buffer, '\n' );
    buffer_push( buffer, '\0' );
}


void uart_send_byte(uint8_t sym)
{
    // Wait for TXE:
    while ((*USART1_ISR & (1U <<  7U)) == 0U);

    // Put data into DR:
    *USART1_TDR = sym;
}

uint8_t
uart_receive_data( void )
{

    while ( (*USART1_ISR & 0x00000020) == 0U )
    { /* Do nothing */ }

    /* Receive data, flag will be cleared by hardware after read */
    uint8_t recieved = (uint8_t)(*(uint8_t*)(0x40013824U));
    return recieved;
}

void 
uart_print_string( const uint8_t *buffer )
{
    for ( uint32_t i = 0; buffer[i] != '\0'; i++ )
    {
        uart_send_byte( buffer[i] );
    }
}

//------
// Main
//------

#define UART_BAUDRATE 9600
#define UART_BAUDRATE_FIX -300

void
uart_gpt_print( const uint8_t *user_string )
{
    const uint8_t gpt_string[] = "No you ";

    uart_print_string( gpt_string );
    uart_print_string( user_string );
}


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
            uart_gpt_print( usart_buffer.data );
            buffer_clear( &usart_buffer );
        } else
        {
            buffer_push( &usart_buffer, data );
        }
    }

    while (1);
}
