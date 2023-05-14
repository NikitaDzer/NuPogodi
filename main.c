#include <string.h>

#include "lib.h"
#include "init.h"
#include "driver.h"
#include "window.h"
#include "utils.h"

#define WINDOW_WIDTH  128
#define WINDOW_HEIGHT 64
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

    window_clear( &window1, COLOR_BLACK );
    window_clear( &window2, COLOR_WHITE );
    window_clear( &window3, COLOR_WHITE );
    window_clear( &window4, COLOR_WHITE );

    clear_display( &display );
    window_draw( &window1, &display );
    display_draw( &display );

    uint32_t x = 0;
    uint32_t y = 0;

    while ( true )
    {
        delay();
        delay();
        delay();
        delay();
        delay();

        clear_display( &display );

        window_clear( &window1, COLOR_BLACK );
        window_render_circle( 
                &window1,
                5 + x,
                4 + y,
                4,
                COLOR_WHITE 
        ); 

        window_render_circle( 
                &window1,
                WINDOW_WIDTH - 6 - x,
                4 + y,
                4,
                COLOR_WHITE 
        );

        window_render_circle( 
                &window1,
                5 + x,
                40 + y,
                4,
                COLOR_WHITE 
        );

        window_render_circle( 
                &window1,
                WINDOW_WIDTH - 6 - x,
                40 + y,
                4,
                COLOR_WHITE 
        ); 

        window_render_line(
                &window1,
                0,
                10,
                41,
                15,
                COLOR_WHITE
        );

        window_render_line(
                &window1,
                WINDOW_WIDTH - 42,
                15,
                WINDOW_WIDTH - 1,
                10,
                COLOR_WHITE
        );

        window_render_line(
                &window1,
                0,
                47,
                41,
                52,
                COLOR_WHITE
        );

        window_render_line(
                &window1,
                WINDOW_WIDTH - 42,
                52,
                WINDOW_WIDTH - 1,
                47,
                COLOR_WHITE
        );

        window_render_rectangle(
                &window1,
                35,
                57,
                20,
                4,
                COLOR_WHITE
        );

        if ( y < 5 )
        {
            x += 8;
            y += 1;
        }

        window_draw( &window1, &display );
        display_draw( &display );
    }

    return 0;
}

