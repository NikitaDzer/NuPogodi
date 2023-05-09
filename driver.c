#include <string.h>

#include "driver.h"
#include "lib.h"

#define CHUNK_SIZE (UINT8_MAX - 1)

#define ROWS_PER_PAGE (8)

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

static uint32_t
get_visible_extent( uint32_t window_offset,
                    uint32_t window_extent,
                    uint32_t display_offset,
                    uint32_t display_extent )
{
    return (uint32_t)min(
        clamp( 
            (int32_t)window_extent  - (int32_t)window_offset,  
            0, 
            (int32_t)window_extent
        ),
        clamp( 
            (int32_t)display_extent - (int32_t)display_offset, 
            0, 
            (int32_t)display_extent
        )
    );
}

static uint32_t
get_window_offset( int32_t present_offset )
{
    return (uint32_t)max( -present_offset, 0 );
}

static uint32_t
get_display_offset( int32_t present_offset )
{
    return (uint32_t)max( present_offset, 0 );
}

static uint32_t
get_nearest_page( uint32_t offset_y )
{
    return offset_y / ROWS_PER_PAGE;
}

static uint32_t
get_aligned_page( uint32_t offset_y )
{
    if ( offset_y % ROWS_PER_PAGE == 0 )
    {
        return get_nearest_page( offset_y );
    } else
    {
        return 1 + get_nearest_page( offset_y );
    }
}

static uint32_t
get_aligned_offset( uint32_t offset_y )
{
    return get_aligned_page( offset_y ) * ROWS_PER_PAGE;
}

static uint32_t
get_full_pages( uint32_t offset_y,
                uint32_t height )
{
    uint32_t offset_diff = get_aligned_offset( offset_y ) - offset_y;
    return ( height - offset_diff ) / ROWS_PER_PAGE;
}

static void
display_render_page( Display *display,
                     Window *window,
                     uint32_t display_offset_x,
                     uint32_t display_offset_page,
                     uint32_t window_offset_x,
                     uint32_t window_offset_page,
                     uint32_t visible_width,
                     uint32_t page )
{
    uint32_t display_start_pixel =
        display_offset_x + (display_offset_page + page) * display->width;

    uint32_t window_start_pixel = 
        window_offset_x + (window_offset_page + page) * window->width;

    memcpy( 
        &display->pixels[ display_start_pixel ],
        &window->pixels[  window_start_pixel  ],
        visible_width
    );
}

/**
 * Returns rendered height.
 */
static uint32_t 
display_render_pages( Display *display,
                      Window *window,
                      uint32_t visible_width,
                      uint32_t visible_height )
{
    WindowPresent *present = &window->present;

    uint32_t window_offset_x    = get_window_offset( present->offset_x );
    uint32_t window_offset_y    = get_window_offset( present->offset_y );
    uint32_t window_offset_page = get_aligned_page( window_offset_y );

    uint32_t display_offset_x    = get_display_offset( present->offset_x );
    uint32_t display_offset_y    = get_display_offset( present->offset_y );
    uint32_t display_offset_page = get_aligned_page( display_offset_y );

    uint32_t visible_full_pages = 
        get_full_pages( window_offset_y, visible_height );

    for ( uint32_t page = 0; page < visible_full_pages; page++ )
    {
        display_render_page(
            display,
            window,
            display_offset_x,
            display_offset_page,
            window_offset_x,
            window_offset_page,
            visible_width,
            page 
        );
    }

    return visible_full_pages * ROWS_PER_PAGE;
}

static void
display_render_partial( Display *display,
                        Window *window,
                        uint32_t display_offset_y,
                        uint32_t window_offset_y,
                        uint32_t partial_height,
                        uint32_t visible_width )
{
    WindowPresent *present = &window->present;

    uint32_t window_offset_start_x = 
        get_window_offset( present->offset_x );
    uint32_t window_offset_page = get_nearest_page( window_offset_y );
    
    uint32_t display_offset_start_x = 
        get_display_offset( present->offset_x );
    uint32_t display_offset_page = get_nearest_page( display_offset_y );

    uint8_t display_shift = display_offset_y % ROWS_PER_PAGE;
    uint8_t window_shift  = window_offset_y % ROWS_PER_PAGE;

    for ( uint32_t column = 0; column < visible_width; column++ )
    {
        uint32_t display_offset_x = display_offset_start_x + column;
        uint32_t display_pixel =
            display_offset_x + display_offset_page * display->width;

        uint32_t window_offset_x = window_offset_start_x + column;
        uint32_t window_pixel =
            window_offset_x + window_offset_page * window->width;

        uint8_t display_pixel_value =
            display->pixels[ display_pixel ];

        uint8_t display_access_mask =
            (uint8_t)create_bitmask(
                display_shift,
                partial_height,
                UINT8_MAX
            );

        uint8_t display_pixel_saved_mask =
            display_pixel_value & ~display_access_mask;

        uint8_t window_pixel_mask =
            (uint8_t)create_bitmask(
                window_shift,
                partial_height,
                window->pixels[ window_pixel ]
            );

        uint8_t update_pixel_mask = window_pixel_mask;
        update_pixel_mask >>= window_shift;
        update_pixel_mask <<= display_shift;

        display->pixels[ display_pixel ] =
            (uint8_t)(display_pixel_saved_mask | update_pixel_mask);
    }
}

static uint8_t
get_partial_shift( uint32_t display_offset_y,
                   uint32_t window_offset_y )
{
    if ( display_offset_y % ROWS_PER_PAGE != 0 )
    {
        return display_offset_y % ROWS_PER_PAGE;
    } else
    {
        return window_offset_y % ROWS_PER_PAGE;
    }
}

void
display_render( Display *display,
                Window *window )
{
    WindowPresent *present = &window->present;

    uint32_t window_offset_x = get_window_offset( present->offset_x );
    uint32_t window_offset_y = get_window_offset( present->offset_y );

    uint32_t display_offset_x = get_display_offset( present->offset_x );
    uint32_t display_offset_y = get_display_offset( present->offset_y );

    uint32_t visible_width = get_visible_extent(
        window_offset_x,
        window->width,
        display_offset_x,
        display->width
    );

    uint32_t visible_height = get_visible_extent(
        window_offset_y,
        window->height,
        display_offset_y,
        display->height
    );
    uint32_t rendered_height = 0;

    uint32_t partial_shift = 
        get_partial_shift( display_offset_y, window_offset_y );

    if ( partial_shift == 0 )
    {
        rendered_height +=
            display_render_pages( 
                display, 
                window,
                visible_width,
                visible_height
            );
    }

    while ( rendered_height != visible_height )
    {
        partial_shift = 
            get_partial_shift( 
                display_offset_y + rendered_height, 
                window_offset_y  + rendered_height 
            );

        uint32_t rest_height = visible_height - rendered_height;

        uint32_t partial_height = 
            (uint32_t)clamp(
                (int32_t)rest_height, 
                (int32_t)rest_height, 
                (int32_t)(ROWS_PER_PAGE - partial_shift)
            );

        display_render_partial(
            display,
            window,
            display_offset_y + rendered_height,
            window_offset_y  + rendered_height,
            partial_height,
            visible_width
        );

        rendered_height += partial_height;
    }
}

void
window_set_pix( Window *window,
                uint32_t x, 
                uint32_t y, 
                Color color )
{
    uint8_t  pixel_bit     = (uint8_t)( 1 << (y % ROWS_PER_PAGE) );
    uint32_t pixel_address = window->width * (y / ROWS_PER_PAGE) + x;

    switch ( color )
    {
        case COLOR_WHITE:
            window->pixels[ pixel_address ] |= pixel_bit;
            break;

        case COLOR_BLACK:
            window->pixels[ pixel_address ] &= ~pixel_bit;
            break;

        default:
            // Unreachable.
            break;

    }
}

void
display_draw( Display *display )
{
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
}
