#pragma once

#include <stdint.h>
#include "driver.h"

typedef enum Color
{
    COLOR_BLACK = 0x00,
    COLOR_WHITE = 0xFF,
} Color;

typedef struct WindowPresent
{
    Display *display;
    
    int32_t offset_x;
    int32_t offset_y;
} WindowPresent;

typedef struct Window 
{
    uint8_t *pixels;

    uint32_t width;
    uint32_t height;

    WindowPresent present;
} Window;

void
window_set_pixel( Window *window,
                  uint32_t x, 
                  uint32_t y, 
                  Color color );

void
window_render_rectangle( Window *window,
                         uint32_t x,
                         uint32_t y,
                         uint32_t width,
                         uint32_t height,
                         Color color );

void
window_render_circle( Window *window,
                      uint32_t x,
                      uint32_t y,
                      uint32_t radius,
                      Color color );

void
window_render_line( Window *window,
                    uint32_t x0,
                    uint32_t y0,
                    uint32_t x1,
                    uint32_t y1,
                    Color color );

void
window_clear( Window *window,
              Color color );

void
window_draw( Window *window,
             Display *display ); 

