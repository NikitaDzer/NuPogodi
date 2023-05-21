#pragma once

#include <stdint.h>
#include <stdbool.h>

void
inverse_bits( volatile uint32_t* reg_addr, 
              uint32_t mask );

void
mask_bits( volatile uint32_t* reg_addr, 
           uint32_t mask, 
           uint32_t value );

void
set_bits( volatile uint32_t* reg_addr, 
          uint32_t mask );

void
clear_bits( volatile uint32_t* reg_addr, 
            uint32_t mask );

bool
compare_bits( volatile uint32_t* reg_addr, 
              uint32_t mask,
              uint32_t value );

bool
test_bits( volatile uint32_t* reg_addr, 
           uint32_t mask );
