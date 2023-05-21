#include "../include/delay.h"

#define CPU_FREQUENCY     (16000000U) // CPU frequency: 16 MHz
#define MS_PER_SEC        (1000U)
#define TICKS_PER_MS      ((CPU_FREQUENCY) / (MS_PER_SEC))
#define TICKS_PER_CYCLE   (1U + 3U)

 
static void __attribute__((optimize("-O0")))
delay_cycles( [[maybe_unused]] volatile uint32_t cycles )
{
    __asm__ volatile
    (
        ".syntax unified  \n"

        ".delay:          \n"
        "subs r0, r0, #1  \n"   // 1 cycle
        "bne .delay       \n"   // 3 cycles
    );
}

void
delay_ms( uint32_t ms )
{
    uint32_t cycles = (ms * TICKS_PER_MS) / TICKS_PER_CYCLE;
    delay_cycles( cycles );
}
