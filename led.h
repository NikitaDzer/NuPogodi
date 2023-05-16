#pragma once
#include <stdint.h>

#include "lib.h"

typedef struct led_t
{
    GPIOx port;
    unsigned pin;
} led_t;

void led_init(led_t* led_, GPIOx port_, unsigned pin_)
{
    set_GPIOx_MODER(port_, pin_, GENERAL_PURPOSE_OUTPUT_MODE);
    set_GPIOx_OTYPER(port_, pin_, PUSH_PULL_TYPE);
    led_->pin = pin_;
    led_->port = port_;
}

void led_on(led_t* led_)
{
    set_GPIOx_ODR(led_->port, led_->pin, 1);
}

void led_off(led_t* led_)
{
    set_GPIOx_ODR(led_->port, led_->pin, 0);
}
