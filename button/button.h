#pragma once

#include <stdint.h>
#include "../lib.h"
#include "interrupts.h"

typedef struct button_t
{
    bool pressed;
    GPIOx port;
    unsigned pin;
    void (*cmd)(void);
} button_t;

void button_init(button_t* btn_, GPIOx port_, unsigned pin_, void (*cmd_)(void));
void button_on_response(button_t* btn_);
