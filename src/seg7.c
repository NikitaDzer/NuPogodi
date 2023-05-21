#include "../include/seg7.h"

void
SEG7_set_number_quarter( Seg7Display* seg7 )
{
    static unsigned tick = 0;
    uint32_t divisors[4] = {1000, 100, 10, 1};

    unsigned int quarter = tick++ % 4;
    unsigned int divisor = divisors[quarter];

    seg7->display = DIGITS[(seg7->number / divisor) % 10] | POSITIONS[quarter];
}

// Sent changes:
void
SEG7_push_display_state_to_mc( Seg7Display* seg7 )
{
    uint32_t surrounding_state = ~PINS_USED & *GPIOA_ODR;
    uint32_t to_write = PINS_USED & seg7->display;

    *GPIOA_ODR = surrounding_state | to_write;
}
