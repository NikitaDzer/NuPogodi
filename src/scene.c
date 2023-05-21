#include <stdlib.h>
#include <string.h>

#include "../include/scene.h"
#include "../include/utils.h"
#include "../include/window.h"

static void
pogodi_static_scene_render( Window *window )
{
    assert( window );

    uint32_t width = window->width;

    /**
     * Some empirical positions of lines.
     */
    window_render_line( window,          0, 10,        41, 15, COLOR_WHITE );
    window_render_line( window, width - 42, 15, width - 1, 10, COLOR_WHITE );
    window_render_line( window,          0, 47,        41, 52, COLOR_WHITE );
    window_render_line( window, width - 42, 52, width - 1, 47, COLOR_WHITE );
}

static Position 
pogodi_acquire_egg_position( void )
{
    return (Position)( rand16() % (POSITION_LAST - 1) + 1 );
}

void
pogodi_scene_emit_egg( Scene *scene )
{
    assert( scene );

    Position position = pogodi_acquire_egg_position();

    const uint32_t rel_x = 5;
          uint32_t rel_y = 0;

    switch ( position )
    {
        case POSITION_LEFT_TOP:
        case POSITION_RIGHT_TOP:
            rel_y = 4;
            break;

        case POSITION_LEFT_BOTTOM:
        case POSITION_RIGHT_BOTTOM:
            rel_y = 40;
            break;

        default:
            assert( 0 && "Unreachable." );
            break;
    }

    scene->next_egg->rel_x = rel_x;
    scene->next_egg->rel_y = rel_y;
    scene->next_egg->position = position;

    if ( scene->next_egg == scene->eggs + EGGS_MAX_NUMBER - 1 )
    {
        scene->next_egg = scene->eggs;
    } else
    {
        scene->next_egg++;
    }
}

static void
pogodi_egg_render( Egg *egg,
                   Window *window )
{
    if ( egg->position == POSITION_INVISIBLE )
    {
        return;
    }

    const uint32_t offset_y = egg->rel_y; 
          uint32_t offset_x = 0;

    switch ( egg->position )
    {
        case POSITION_LEFT_TOP:
        case POSITION_LEFT_BOTTOM:
            offset_x = egg->rel_x;
            break;

        case POSITION_RIGHT_TOP:
        case POSITION_RIGHT_BOTTOM:
            offset_x = window->width - 1 - egg->rel_x;
            break;

        default:
            assert( 0 && "Unreachable." );
            break;
    }

    window_render_circle(
        window,
        offset_x,
        offset_y,
        EGG_RAGIUS,
        COLOR_WHITE
    );
}

static void
pogodi_basket_render( Basket *basket,
                      Window *window )
{
    assert( basket );
    assert( window );

    const uint32_t TOP_Y    = 19u;
    const uint32_t BOTTOM_Y = 57u;
    const uint32_t REL_X    = 38u;

    uint32_t offset_x = 0;
    uint32_t offset_y = 0;

    switch ( basket->position )
    {
        case POSITION_LEFT_TOP:
            offset_x = REL_X;
            offset_y = TOP_Y;
            break;

        case POSITION_RIGHT_TOP:
            offset_x = window->width - 1 - REL_X - BASKET_WIDTH;
            offset_y = TOP_Y;
            break;

        case POSITION_RIGHT_BOTTOM:
            offset_x = window->width - 1 - REL_X - BASKET_WIDTH;
            offset_y = BOTTOM_Y;
            break;

        case POSITION_LEFT_BOTTOM:
            offset_x = REL_X;
            offset_y = BOTTOM_Y;
            break;

        default:
            assert( 0 && "Unreachable." );
            break;
    }

    window_render_rectangle(
        window,
        offset_x,
        offset_y,
        BASKET_WIDTH,
        BASKET_HEIGHT,
        COLOR_WHITE
    );
}

void
pogodi_scene_init( Scene *scene )
{
    assert( scene );

    /**
     * Initially all eggs are invisible,
     * so we fill them with POSITION_INVISIBLE, which is 0x00.
     */
    qmemset( scene, 0x00, sizeof( Scene ) );

    scene->basket.position = POSITION_LEFT_BOTTOM;
    scene->next_egg = scene->eggs;
}

void
pogodi_scene_render( Scene *scene, 
                     Window *window )
{
    assert( scene );
    assert( window );

    pogodi_static_scene_render( window );

    for ( uint32_t i = 0; i < EGGS_MAX_NUMBER; i++ )
    {
        pogodi_egg_render( scene->eggs + i, window );
    }

    pogodi_basket_render( &scene->basket, window );
}

static void
pogodi_egg_move( Egg *egg )
{
    assert( egg );

    if ( egg->position == POSITION_INVISIBLE )
    {
        return;
    }

    /**
     * Some empirical numbers.
     */
    egg->rel_x += 7;
    egg->rel_y += 1;
}

void
pogodi_scene_move_eggs( Scene *scene )
{
    assert( scene );

    for ( uint32_t i = 0; i < EGGS_MAX_NUMBER; i++ )
    {
        pogodi_egg_move( scene->eggs + i );
    }
}

void
pogodi_scene_move_basket( Scene *scene,
                          Position position )
{
    scene->basket.position = position;
}
