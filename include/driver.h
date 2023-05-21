#pragma once

#include <stdint.h>
#include "lib.h"

#define ROWS_PER_PAGE (8)

typedef struct Display
{
    I2Cx i2c;
    uint8_t address;

    uint8_t *pixels;

    uint32_t width;
    uint32_t height;
} Display;

void
display_init( Display *display, 
              GPin gpin1, 
              GPin gpin2 );

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
display_draw( Display *display );
