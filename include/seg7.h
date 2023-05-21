#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../include/bit.h"
#include "../include/bits.h"
#include "../include/memory.h"

//-------------------
// 7-segment display
//-------------------

// TRUSTWORTHY Pin Mapping:
#define A  (1U << 11U)
#define B  (1U <<  7U)
#define C  (1U <<  4U)
#define D  (1U <<  2U)
#define E  (1U <<  1U)
#define F  (1U << 10U)
#define G  (1U <<  5U)
#define DP (1U <<  3U)

#define DIG1 (1U << 12U)
#define DIG2 (1U <<  9U)
#define DIG3 (1U <<  8U)
#define DIG4 (1U <<  6U)

static const uint32_t PINS_USED = A|B|C|D|E|F|G|DP|DIG1|DIG2|DIG3|DIG4;

// Digit composition:
static const uint32_t DIGITS[10] =
{
    A|B|C|D|E|F,   // 0
    B|C,           // 1
    A|B|D|E|G,     // 2
    A|B|C|D|G,     // 3
    B|C|F|G,       // 4
    A|C|D|F|G,     // 5
    A|C|D|E|F|G,   // 6
    A|B|C,         // 7
    A|B|C|D|E|F|G, // 8
    A|B|C|D|F|G    // 9
};

static const uint32_t POSITIONS[4] =
{
         DIG2|DIG3|DIG4, // 0
    DIG1     |DIG3|DIG4, // 1
    DIG1|DIG2     |DIG4, // 2
    DIG1|DIG2|DIG3       // 3
};

// Display state:
typedef struct Seg7Display
{
    uint32_t display;
    uint16_t number;
} Seg7Display;

void
SEG7_set_number_quarter(Seg7Display* seg7);

// Sent changes:
void
SEG7_push_display_state_to_mc(Seg7Display* seg7);
