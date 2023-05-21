#pragma once

#include <stdint.h>

void
board_clocking_init();

void
board_gpio_init();

void
board_i2c_init();

void
board_timers_init( uint32_t prescaler );

void 
board_systick_init( uint32_t poll_period_us );
