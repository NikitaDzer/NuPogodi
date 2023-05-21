#include "../include/lib.h"
#include "../include/game.h"
#include "../include/utils.h"
#include "../include/button.h"
#include "../include/assets.h"
#include "../include/interrupts.h"
#include "../include/seg7.h"
#include "../include/delay.h"

#include <stdlib.h>
#include <stdint.h>

#define BASKET_POSITIONS_NUMBER (4)

typedef enum CatchResult
{
    CATCH_DONT_NEED,
    CATCH_FAILURE,
    CATCH_SUCCESS,
} CatchResult;

static volatile Scene scene;
static volatile bool  need_draw = false;

static Window      *window = NULL;
static Seg7Display  score;
static button_t     buttons[ BASKET_POSITIONS_NUMBER ];

static void
score_draw( void )
{
    SEG7_set_number_quarter( &score );
    SEG7_push_display_state_to_mc( &score );
}


void
systick_handler( void )
{
    score_draw();
}

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

void 
EXTI1_IRQHandler(void)
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

    pogodi_scene_move_basket( 
        (Scene *)&scene,
        (Position)( button - buttons + 1 )
    );

    need_draw = true;
}

static void
raise_loss_flag( void )
{
    enable_GPIOx_clock( GPIOC );
    set_GPIOx_MODER(
        GPIOC,
        PIN8,
        GENERAL_PURPOSE_OUTPUT_MODE
    );
    set_GPIOx_ODR( GPIOC, PIN8, 1 );

    while ( true )
    { /* do nothing */ }
}

void
pogodi_game_init( Window *bound_window )
{
    enable_GPIOx_clock( GPIOC );
    window = bound_window;

    for ( uint32_t pin = 0; pin < BASKET_POSITIONS_NUMBER; pin++ )
    {
        button_init( buttons + pin, GPIOC, pin, move_basket_and_render );
    }
    
    pogodi_scene_init( (Scene *)&scene );

    score.number = 0;
}

void
pogodi_game_greetings( void )
{
    window_clear( window, COLOR_BLACK );
    draw_RT( window );

    window_draw( window, window->present.display );
    display_draw( window->present.display );

    delay_ms( 2500 );
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
        Egg *egg = (Egg *)scene.eggs + i;

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

static bool
need_move_eggs( void )
{
    const uint32_t INITIAL_MOVE_DELAY = 100000;
    const uint32_t FINAL_MOVE_DELAY   = 40000;
    const uint32_t STEP_MOVE_DELAY    = 5000;

    const uint32_t TICKS_TO_STEP = 100000;

    static volatile uint32_t move_delay = INITIAL_MOVE_DELAY;

    /**
     * Since there are no eggs at the game's start, 
     * movement would cause only egg emit.
     */
    static volatile uint32_t delay_tick = 0;

    /**
     * Don't change emit_delay at the game's start.
     */
    static volatile uint32_t step_tick = 1;

    bool need_move = delay_tick % move_delay == 0;

    if ( need_move )
    {
        delay_tick = 0;
    }

    if ( step_tick % TICKS_TO_STEP == 0
         && move_delay > FINAL_MOVE_DELAY )
    {
        move_delay -= STEP_MOVE_DELAY;

        delay_tick = 0;
        step_tick  = 0;
    }

    delay_tick++;
    step_tick++;

    return need_move;
}

static bool
need_emit_egg( void )
{
    const uint32_t INITIAL_EMIT_DELAY = 4;
    const uint32_t FINAL_EMIT_DELAY   = 2;
    const uint32_t STEP_EMIT_DELAY    = 1;

    const uint32_t TICKS_TO_STEP = 20;

    volatile static uint32_t emit_delay = INITIAL_EMIT_DELAY;

    /**
     * Emit egg at the game's start.
     */
    volatile static uint32_t delay_tick = 0;

    /**
     * Don't change emit_delay at the game's start.
     */
    volatile static uint32_t step_tick = 1;

    bool need_emit = delay_tick % emit_delay == 0;

    if ( need_emit )
    {
        delay_tick = 0;
    }

    if ( step_tick % TICKS_TO_STEP == 0
         && emit_delay > FINAL_EMIT_DELAY )
    {
        emit_delay -= STEP_EMIT_DELAY;

        delay_tick = 0;
        step_tick  = 0;
    }

    delay_tick++;
    step_tick++;

    return need_emit;
}

static void
game_draw( void )
{
    window_clear( window, COLOR_BLACK );
    pogodi_scene_render( (Scene *)&scene, window ); 
    window_draw( window, window->present.display );
    display_draw( window->present.display );
}

void
pogodi_game_loop( void )
{
    while ( true )
    {
        if ( need_move_eggs() )
        {
            CatchResult result = try_catch_egg();

            if ( result == CATCH_SUCCESS )
            {
                score.number++;
            } else if ( result == CATCH_FAILURE )
            {
                raise_loss_flag();
            }

            pogodi_scene_move_eggs( (Scene *)&scene );

            if ( need_emit_egg() )
            {
                pogodi_scene_emit_egg(
                    (Scene *)&scene
                );
            }

            need_draw = true;
        }

        if ( need_draw )
        {
            game_draw();
            need_draw = false;
        }
    }
}
