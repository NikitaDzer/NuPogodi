#include "include/lib.h"
#include "include/init.h"
#include "include/driver.h"
#include "include/window.h"
#include "include/utils.h"
#include "include/scene.h"
#include "include/assets.h"
#include "include/button.h"
#include "include/game.h"
#include "include/delay.h"

#define WINDOW_WIDTH  128
#define WINDOW_HEIGHT 64
#define WINDOW_SIZE   (WINDOW_WIDTH * WINDOW_HEIGHT / ROWS_PER_PAGE )

#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64
#define DISPLAY_SIZE   (DISPLAY_WIDTH * DISPLAY_HEIGHT / ROWS_PER_PAGE )

static volatile Display display;
static volatile Window  window;

static void
init_display( void )
{
    static uint8_t buffer[ DISPLAY_SIZE ] = { 0 };

    display.i2c     = I2C1;
    display.address = 0x78;
    display.pixels  = buffer;

    display.width  = DISPLAY_WIDTH;
    display.height = DISPLAY_HEIGHT;

    display_init( 
        (Display *)&display,
        (GPin){ GPIOB, PIN6 }, 
        (GPin){ GPIOB, PIN7 }
    );

    display_config(
        (Display *)&display
    );
}

static void
init_window( void )
{
    static uint8_t buffer[ WINDOW_SIZE ] = { 0 };
    
    window.pixels = buffer;

    window.width  = WINDOW_WIDTH;
    window.height = WINDOW_HEIGHT;

    window.present.display = (Display *)&display;

    window.present.offset_x = 0;
    window.present.offset_y = 0;
}

int 
main()
{
    board_clocking_init();
    board_gpio_init();
    board_timers_init( 1000 );
    board_systick_init( 1000 );

    init_display();
    init_window();

    pogodi_game_init( (Window *)&window );
    pogodi_game_greetings();
    pogodi_game_loop();

    return 0;
}

