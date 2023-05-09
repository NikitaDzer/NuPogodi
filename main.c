#include "lib.h"
#include "init.h"
#include "driver.h"


#define WINDOW_WIDTH  32
#define WINDOW_HEIGHT 32
#define WINDOW_SIZE   (WINDOW_WIDTH * WINDOW_HEIGHT / ROWS_PER_PAGE )

#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64
#define DISPLAY_SIZE   (DISPLAY_WIDTH * DISPLAY_HEIGHT / ROWS_PER_PAGE )

static void
delay( void )
{
    volatile int time = 1000000;
    while ( time-- );
}

static void
clear_display( Display *display )
{
    memset( display->pixels, 0x00, DISPLAY_SIZE );
}

int 
main()
{
    board_clocking_init();
    board_gpio_init();

    uint8_t buffer1[ WINDOW_SIZE ] = { 0 };
    uint8_t buffer2[ WINDOW_SIZE ] = { 0 };
    uint8_t buffer3[ WINDOW_SIZE ] = { 0 };
    uint8_t buffer4[ WINDOW_SIZE ] = { 0 };

    uint8_t vram[ DISPLAY_SIZE ] = { 0 };

    Display display = {
        .i2c = I2C1,
        .address = 0x78,

        .pixels = vram,

        .width = 128,
        .height = 64
    };

    Window window1 = {
        .pixels = buffer1,

        .width  = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,

        .present = {
            .display = &display,
            .offset_x = 0,
            .offset_y = 0 
        }
    };

    Window window2 = {
        .pixels = buffer2,

        .width  = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,

        .present = {
            .display = &display,
            .offset_x = 0,
            .offset_y = 33,
        }
    };

    Window window3 = {
        .pixels = buffer3,

        .width  = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,

        .present = {
            .display = &display,
            .offset_x = 96,
            .offset_y = 0,
        }
    };

    Window window4 = {
        .pixels = buffer4,

        .width  = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,

        .present = {
            .display = &display,
            .offset_x = 96,
            .offset_y = 33,
        }
    };


    // Config hardware
    display_init( 
        &display,
        (GPin){ GPIOB, PIN6 }, 
        (GPin){ GPIOB, PIN7 }
    );

    delay();

    display_config(
        &display
    );

    memset( buffer1, 0xFF, WINDOW_SIZE );
    memset( buffer2, 0xFF, WINDOW_SIZE );
    memset( buffer3, 0xFF, WINDOW_SIZE );
    memset( buffer4, 0xFF, WINDOW_SIZE );

    clear_display( &display );

    display_render( &display, &window1 );
    display_render( &display, &window2 );
    display_render( &display, &window3 );
    display_render( &display, &window4 );

    display_draw( &display );

    while ( true )
    {
        delay();

        clear_display( &display );

        window1.present.offset_x += 1;
        window1.present.offset_y += 1;

        display_render( &display, &window1 );
        display_render( &display, &window2 );
        display_render( &display, &window3 );
        display_render( &display, &window4 );

        display_draw( &display );
    }

    return 0;
}
