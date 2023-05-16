#include <string.h>
#include <stdlib.h>

#include "lib.h"
#include "init.h"
#include "driver.h"
#include "window.h"
#include "utils.h"
#include "scene.h"
#include "assets.h"
#include "button.h"
#include "game.h"

#define WINDOW_WIDTH  128
#define WINDOW_HEIGHT 64
#define WINDOW_SIZE   (WINDOW_WIDTH * WINDOW_HEIGHT / ROWS_PER_PAGE )

#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64
#define DISPLAY_SIZE   (DISPLAY_WIDTH * DISPLAY_HEIGHT / ROWS_PER_PAGE )

static button_t btn1;
static Scene scene;
static Display display;
static Window window1;

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
    uint8_t vram[ DISPLAY_SIZE ] = { 0 };

    Display d = {
        .i2c = I2C1,
        .address = 0x78,

        .pixels = vram,

        .width = 128,
        .height = 64
    };

    Window w1 = {
        .pixels = buffer1,

        .width  = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,

        .present = {
            .display = &display,
            .offset_x = 0,
            .offset_y = 0
        }
    };

    display = d;
    window1 = w1;

    // Config hardware
    display_init( 
        &display,
        (GPin){ GPIOB, PIN6 }, 
        (GPin){ GPIOB, PIN7 }
    );

    display_config(
        &display
    );

    pogodi_game_init( &window1 );
    pogodi_game_greetings();
    pogodi_game_loop();

    return 0;
}

