#include "lib.h"
#include "game.h"
#include "utils.h"
#include "button.h"
#include "assets.h"

#include <stdlib.h>
#include <stdint.h>

#define BASKET_POSITIONS_NUMBER (4)

typedef enum CatchResult
{
    CATCH_DONT_NEED,
    CATCH_FAILURE,
    CATCH_SUCCESS,
} CatchResult;

static Scene scene;
static button_t buttons[ BASKET_POSITIONS_NUMBER ];
static Window *window = NULL;

void
EXTI0_IRQHandler( void )
{
    uint32_t pin = 0;

    if ( READ_BIT( EXTI_PR, 0 ) == 1 )
    {
        pin = 0;
        SET_BIT( EXTI_PR, 0 );   
    } else if ( READ_BIT( EXTI_PR, 1 ) == 1 )
    {
        pin = 1;
        SET_BIT( EXTI_PR, 0 );   
    }

    buttons[ pin ].cmd( buttons + pin );    

}

void EXTI1_IRQHandler(void)
{
    uint32_t pin = 0;

    if ( READ_BIT( EXTI_PR, 2 ) == 1 )
    {
        pin = 2;
        SET_BIT( EXTI_PR, 0 );   
    } else if ( READ_BIT( EXTI_PR, 3 ) == 1 )
    {
        pin = 3;
        SET_BIT( EXTI_PR, 0 );   
    }

    buttons[ pin ].cmd( buttons + pin );    
}

void
EXTI2_IRQHandler( void )
{
}

static void
move_basket_and_render( button_t *button )
{
    assert( button );
    assert( window );
    assert( window->present.display );

    window_clear( window, COLOR_BLACK );

    pogodi_scene_move_basket( 
        &scene,
        (Position)( button - buttons + 1 )
    );

    pogodi_scene_render( &scene, window );

    window_draw( window, window->present.display );
    display_draw( window->present.display );
}

void
pogodi_game_init( Window *bound_window )
{
    assert( bound_window );

    enable_GPIOx_clock( GPIOC );
    window = bound_window;

    for ( uint32_t pin = 0; pin < BASKET_POSITIONS_NUMBER; pin++ )
    {
        button_init( buttons + pin, GPIOC, pin, move_basket_and_render );
    }
    

    pogodi_scene_init( &scene );
}

static void
delay( uint32_t number )
{
    volatile uint32_t i = number;
    while ( i-- != 0 );
}

void
pogodi_game_greetings( void )
{
    window_clear( window, COLOR_BLACK );
    draw_RT( window );

    window_draw( window, window->present.display );
    display_draw( window->present.display );

    delay( 3000000 );
}

static bool
is_egg_falling( Egg *egg )
{
    uint32_t fall_level = 0;

    switch ( egg->position )
    {
        case POSITION_LEFT_TOP:
        case POSITION_RIGHT_TOP:
            fall_level = egg->rel_y - 4;
            break;

        case POSITION_LEFT_BOTTOM:
        case POSITION_RIGHT_BOTTOM:
            fall_level = egg->rel_y - 40;
            break;

        case POSITION_INVISIBLE:
            fall_level = 0;
            break;

        default:
            assert( 0 && "Unreachable." );
            break;
    }

    return fall_level > 4;
}

static bool 
is_egg_caught( Egg *egg )
{
    return scene.basket.position == egg->position;
}

static void
egg_release( Egg *egg )
{
    egg->position = POSITION_INVISIBLE;
}

/**
 * try-catch :)
 */
static CatchResult
try_catch_egg( void )
{
    CatchResult result = CATCH_DONT_NEED;

    for ( uint32_t i = 0; i < EGGS_MAX_NUMBER; i++ )
    {
        Egg *egg = scene.eggs + i;

        if ( is_egg_falling( egg ) )
        {
            if ( is_egg_caught( egg ) )
            {
                result = CATCH_SUCCESS;
            } else
            {
                result = CATCH_FAILURE;
            }

            egg_release( egg );
            break;
        }
    }

    return result;
}

void
pogodi_game_loop( void )
{
    uint32_t tick = 0;
    uint32_t delay_time = 3500000;

    while ( true )
    {
        window_clear( window, COLOR_BLACK );

        if ( try_catch_egg() == CATCH_FAILURE ) 
        {
            assert( 0 );
        }

        pogodi_scene_move_eggs( &scene );

        if ( tick++ % 3 == 0 )
        {
            pogodi_scene_emit_egg(
                &scene  
            );
        } 

        pogodi_scene_render( &scene, window ); 
        window_draw( window, window->present.display );
        display_draw( window->present.display );

        delay( delay_time );

        if ( delay_time > 300000 )
        {
            delay_time -= 100000;
        }
    }
}
