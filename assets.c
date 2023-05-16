#include "assets.h"

void
draw_RT( Window *window )
{
    window_render_circle(
        window,
        40,
        20,
        15,
        COLOR_WHITE
    );

    window_render_circle(
        window,
        40,
        20,
        10,
        COLOR_BLACK
    );

    window_render_rectangle(
        window,
        36,
        5,
        8,
        50,
        COLOR_WHITE
    );

    window_render_rectangle(
        window,
        25,
        5,
        15,
        50,
        COLOR_BLACK
    );

    window_render_rectangle(
        window,
        62,
        5,
        30,
        4,
        COLOR_WHITE
    );

    window_render_rectangle(
        window,
        75,
        5,
        4,
        50,
        COLOR_WHITE
    );
}
