#pragma once

#include <stdint.h>
#include "lib.h"

#define ROWS_PER_PAGE (8)

typedef enum Color
{
    COLOR_BLACK = 0x00,
    COLOR_WHITE = 0xFF,
} Color;

typedef struct Display
{
    I2Cx i2c;
    uint8_t address;

    uint8_t *pixels;

    uint32_t width;
    uint32_t height;
} Display;

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
display_init( Display *display, GPin gpin1, GPin gpin2 );

void
display_config( Display *display );

void
display_send_cmd( Display *display, 
                  uint8_t cmd );

void
display_send_data( Display *display, 
                   uint8_t *data, 
                   uint32_t nbytes );

void
display_render( Display *display,
                Window *window );

void
display_draw( Display *display );


void
window_set_pix( Window *window,
                uint32_t x, 
                uint32_t y, 
                Color color );



/*

void
oled_send_cmd( uint8_t cmd );

void
oled_send_data( uint8_t *data, uint8_t nbytes );

void
oled_set_pix( uint8_t x, uint8_t y, Color color );

void
oled_update( Window *window );
*/
