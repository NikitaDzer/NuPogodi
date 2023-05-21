#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "../include/driver.h"
#include "../include/window.h"
#include "../include/utils.h"

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
window_render_page( Window *window,
                    Display *display,
                    uint32_t window_offset_x,
                    uint32_t window_offset_page,
                    uint32_t display_offset_x,
                    uint32_t display_offset_page,
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
window_render_pages( Window *window,
                     Display *display,
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
        window_render_page(
            window,
            display,
            window_offset_x,
            window_offset_page,
            display_offset_x,
            display_offset_page,
            visible_width,
            page 
        );
    }

    return visible_full_pages * ROWS_PER_PAGE;
}

static void
window_render_partial( Window *window,
                       Display *display,
                       uint32_t window_offset_y,
                       uint32_t display_offset_y,
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

        uint8_t window_pixel_value =
                window->pixels[ window_pixel ];

        uint8_t update_pixel_mask = window_pixel_value;
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
window_set_pixel( Window *window,
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
window_draw( Window *window,
             Display *display )
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
            window_render_pages( 
                window,
                display, 
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

        window_render_partial(
            window,
            display,
            window_offset_y  + rendered_height,
            display_offset_y + rendered_height,
            partial_height,
            visible_width
        );

        rendered_height += partial_height;
    }
}

void
window_render_rectangle( Window *window,
                         uint32_t x,
                         uint32_t y,
                         uint32_t width,
                         uint32_t height,
                         Color color )
{
    for ( uint32_t i = 0; i < height; i++ )
    {
        for ( uint32_t j = 0; j < width; j++ )
        {
            window_set_pixel( window, x + j, y + i, color );
        }
    }
}


/**
 * The Bresenham algorithm to draw filled circles.
 */
void
window_render_circle( Window *window,
                      uint32_t center_x,
                      uint32_t center_y,
                      uint32_t radius,
                      Color color )
{
    int32_t x = 0;
    int32_t y = radius;

    int32_t delta = 1 - 2 * (int32_t)radius;
    int32_t error = 0;

    while ( y >= x )
    {
        for ( uint32_t i = center_x - x; i <= center_x + x; i++ )
        {
            window_set_pixel(
                window,
                i,
                center_y - y,
                color
            );
            window_set_pixel(
                window,
                i,
                center_y + y,
                color
            );
        }

        for ( uint32_t i = center_x - y; i <= center_x + y; i++ )
        {
            window_set_pixel(
                window,
                i,
                center_y - x,
                color
            );
            window_set_pixel(
                window,
                i,
                center_y + x,
                color
            );
        }

        error = 2 * (delta + y) - 1;

        if ( (delta < 0) && (error <= 0) )
        {
            x++;
            delta += 2 * x + 1;
            continue;
        }

        if ( (delta > 0) && (error > 0) )
        {
            y--;
            delta -= 2 * y + 1;
            continue;
        }

        x++;
        y--;
        delta += 2 * (x - y);
    }
}

void
window_render_line( Window *window,
                    uint32_t x0,
                    uint32_t y0,
                    uint32_t x1,
                    uint32_t y1,
                    Color color )
{
    int32_t delta_x = abs(x1 - x0);
    int32_t delta_y = abs(y1 - y0);
    int32_t error = 0;
    int32_t delta_err = (delta_y + 1);
    int32_t y = y0;
    int32_t diry = y1 - y0;

    if ( diry > 0 )
    {
        diry = 1;
    }

    if ( diry < 0 )
    {
        diry = -1;
    }

    for ( uint32_t x = x0; x <= x1; x++ )
    {
        window_set_pixel(
            window,
            x,
            y,
            color
        );

        error += delta_err;

        if ( error >= ( delta_x + 1 ) )
        {
            y += diry;
            error -= delta_x + 1;
        }
    }
}

void
window_clear( Window *window,
              Color color )
{
    assert( window != NULL );
    assert( window->pixels != NULL );

    qmemset( 
        window->pixels, 
        (uint8_t)color, 
        window->width * window->height / ROWS_PER_PAGE
    );
}

