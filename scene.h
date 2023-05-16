#pragma once

#include <stdint.h>
#include "window.h"

#define EGGS_MAX_NUMBER (12)
#define EGG_RAGIUS      (4)

#define BASKET_WIDTH  (20)
#define BASKET_HEIGHT (4)

typedef enum Position 
{
    POSITION_INVISIBLE     = 0, 
    POSITION_LEFT_TOP      = 1,
    POSITION_RIGHT_TOP     = 2,
    POSITION_RIGHT_BOTTOM  = 3,
    POSITION_LEFT_BOTTOM   = 4,
    POSITION_LAST          = 5,
} Position;

typedef struct Basket
{
    Position position;
} Basket;

typedef struct Egg
{
    uint32_t rel_x;
    uint32_t rel_y;

    Position position;
} Egg;

typedef struct Scene
{
    Basket basket;
    Egg eggs[ EGGS_MAX_NUMBER ];

    Egg *next_egg;
} Scene;

void
pogodi_scene_init( Scene *scene );

void
pogodi_scene_render( Scene *scene, 
                     Window *window );

void
pogodi_scene_move_eggs( Scene *scene );

void
pogodi_scene_emit_egg( Scene *scene );

void
pogodi_scene_move_basket( Scene *scene,
                          Position position );
