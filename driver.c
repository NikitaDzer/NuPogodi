#include <string.h>

#include "driver.h"
#include "lib.h"

#define CHUNK_SIZE (UINT8_MAX - 1)

#define SET_DISPLAY_ON          (0xAF)
#define SET_DISPLAY_OFF         (0xAE)

#define SET_RENDER_GRAM_ON      (0xA4)
#define SET_RENDER_GRAM_OFF     (0xA5)

#define SET_NORM_INDICATION_ON  (0xA6)
#define SET_NORM_INDICATION_OFF (0xA7)

#define SET_VERTICAL_FLIP_ON    (0xC0)
#define SET_VERTICAL_FLIP_OFF   (0xC8)

#define SET_HORIZONTAL_FLIP_ON  (0xA1)
#define SET_HORIZONTAL_FLIP_OFF (0xA0)

#define SET_START_LINE_ADDRESS  (0x40)
#define SET_DISPLAY_OFFSET      (0xD3)
#define SET_DISPLAY_RATE        (0xD5)
#define SET_ADDRESSING_MODE     (0x20)
#define SET_CONTRAST_LEVEL      (0X81)
#define SET_ACTIVE_LINES_NUMBER (0xA8)
#define SET_COMPINS             (0xDA)
#define SET_CHARGE_PUMP         (0x8D)

static void
display_init_gpin( GPin gpin )
{
    GPIOx gpio = gpin.gpio;
    uint8_t pin = gpin.pin;

    enable_GPIOx_clock( gpio );

    set_GPIOx_OTYPER ( gpio, pin, OPEN_DRAIN_TYPE         );
    set_GPIOx_MODER  ( gpio, pin, ALTERNATE_FUNCTION_MODE );
    set_GPIOx_AFRL   ( gpio, pin, AF1                     );
    set_GPIOx_OSPEEDR( gpio, pin, HIGH_SPEED              );
}

static void
display_init_I2C( I2Cx i2c )
{
    select_I2Cx_source( i2c, I2Cx_SOURCE_SYSCLK );
    disable_I2Cx_peripheral( i2c );

    switch ( i2c )
    {
        case I2C1:
            enable_APB_peripheral_clock( I2C1EN );
            break;
        
        case I2C2:
            enable_APB_peripheral_clock( I2C2EN );
            break;

        default:
            // Unreachable.
            break;
    }

    disable_I2Cx_analog_filter( i2c );
    set_I2Cx_digital_filter( i2c, 1 );

    set_I2Cx_400khz( i2c );
    disable_I2Cx_clock_stretching( i2c );

    set_I2Cx_addressing_mode( i2c, I2Cx_7BIT );

    set_I2Cx_own_address_mode( i2c, I2Cx_7BIT );    
    enable_I2Cx_own_address( i2c );

    set_I2Cx_mode_I2C( i2c );

    enable_I2Cx_peripheral( i2c );
}


void
display_init( Display *display, 
              GPin gpin1, 
              GPin gpin2 )
{
    display_init_gpin( gpin1 );
    display_init_gpin( gpin2 );

    display_init_I2C( display->i2c );
}

void
display_send_cmd( Display *display, 
                  uint8_t cmd )
{
    I2Cx i2c = display->i2c;
    uint8_t address = display->address;

    /**
     * Initiate transfer.
     */
    start_I2Cx_transfer( i2c, I2Cx_7BIT, address, 2 );

    /**
     * Send Control byte (Co = 0, D/C# = 0).
     */
    transmit_I2Cx( i2c, 0x00 );

    /**
     * Send cmd.
     */
    transmit_I2Cx( i2c, cmd );

    /**
     * Check for end of transfer.
     */
    end_I2Cx_transfer( i2c );
}

/**
 * ATTENTION!
 * Must be nbytes <= 254.
 */
static void
display_send_data_chunk( Display *display,
                         uint8_t *data,
                         uint8_t nbytes )
{
    I2Cx i2c = display->i2c;
    uint8_t address = display->address;

    /**
     * Initiate transfer. 
     */
    start_I2Cx_transfer( i2c, I2Cx_7BIT, address, nbytes + 1 ); 

    /**
     * Send Control byte (Co = 0, D/C# = 1).
     */
    transmit_I2Cx( i2c, 0x40 );

    /**
     * Send data.
     */
    for ( uint16_t i = 0; i < nbytes; i++ )
    {
        transmit_I2Cx( i2c, data[i] );
    }

    /**
     * Check for end of transfer.
     */
    end_I2Cx_transfer( i2c );
}

static uint32_t
calculate_chunks_number( uint32_t nbytes )
{
    uint32_t chunks_number = nbytes / CHUNK_SIZE;

    if ( chunks_number * CHUNK_SIZE != nbytes )
    {
        chunks_number++;
    }

    return chunks_number;
}

void
display_send_data( Display *display, 
                   uint8_t *data, 
                   uint32_t nbytes )
{
    uint8_t* chunk = data;
    uint32_t chunks_number = calculate_chunks_number( nbytes );
    uint32_t full_chunks_number = chunks_number - 1;

    for ( uint32_t i = 0; i < full_chunks_number; i++ )
    {
        display_send_data_chunk( display, chunk, CHUNK_SIZE );
        chunk += CHUNK_SIZE;
    }

    /**
     * Conversion is safe, 
     * because rest data must be no more 255 (CHUNK_SIZE) bytes.
     */
    uint8_t rest_nbytes = 
        (uint8_t)( nbytes - full_chunks_number * CHUNK_SIZE );

    display_send_data_chunk( display, chunk, rest_nbytes );
}

void
display_config( Display *display )
{
    const uint8_t VERTICAL_ADDRESSING_MODE = 0x10;
    const uint8_t START_LINE_ADDRESS       = 0x00;
    const uint8_t CONTRAST_LEVEL           = 0xFF;
    const uint8_t ACTIVE_LINES             = 0x3F;
    const uint8_t DISPLAY_OFFSET           = 0x00;
    const uint8_t DISPLAY_RATE             = 0xF0;
    const uint8_t COMPINS                  = 0x12;
    const uint8_t ENABLE_CHARGE_PUMP       = 0x14;

    uint8_t config_cmds[] = {
        SET_DISPLAY_OFF,
        SET_ADDRESSING_MODE,        VERTICAL_ADDRESSING_MODE,
        SET_VERTICAL_FLIP_OFF,
        SET_START_LINE_ADDRESS +    START_LINE_ADDRESS,
        SET_CONTRAST_LEVEL,         CONTRAST_LEVEL,
        SET_HORIZONTAL_FLIP_ON,
        SET_NORM_INDICATION_ON,
        SET_ACTIVE_LINES_NUMBER,    ACTIVE_LINES,
        SET_RENDER_GRAM_ON,
        SET_DISPLAY_OFFSET,         DISPLAY_OFFSET,
        SET_DISPLAY_RATE,           DISPLAY_RATE,
        SET_COMPINS,                COMPINS,
        SET_CHARGE_PUMP,            ENABLE_CHARGE_PUMP,
        SET_DISPLAY_ON
    };

    for ( uint32_t i = 0; i < sizeof( config_cmds ); i++ )
    {
        display_send_cmd( display, config_cmds[ i ] );
    }
}

void
display_draw( Display *display )
{
    __asm__ volatile ("CPSID i");
    
    uint32_t pages = display->height / ROWS_PER_PAGE;

    for ( uint8_t page = 0; page < pages; page++ )
    {
        // Set page start address (number of vertical byte).
        display_send_cmd( display, 0xB0 + page );

        // Set lower column number.
        display_send_cmd( display, 0x00 );

        // Set higher column number.
        display_send_cmd( display, 0x00 );

        // Send 128 pixels.
        display_send_data( 
            display,
            &display->pixels[ display->width * page ], 
            display->width
        );
    }

    __asm__ volatile ("CPSIE i"); 
}
