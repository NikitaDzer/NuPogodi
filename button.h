#pragma once

#include <stdint.h>
#include "lib.h"

typedef struct button_t
{
    bool pressed;
    bool active;

    GPIOx port;
    unsigned pin;
    void (*cmd)(struct button_t*);
} button_t;

void button_init(button_t* btn_, GPIOx port_, unsigned pin_, void (*cmd_)(button_t*));
