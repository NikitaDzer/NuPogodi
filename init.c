#include "lib.h"
#include "init.h"

void 
board_clocking_init()
{
    enable_HSE_clock();

    set_PLL_prescaler( PREDIV_DIVIDED_BY_2 );
    set_PLL_source( HSE_DIVIDED_BY_PREDIV );
    set_PLL_multiplier( PLL_MULTIPLIED_BY_12 );
    enable_PLL_clock();

    set_AHB_prescaler( SYSCLK_NOT_DIVIDED );

    select_SYSCLK_source( PLL );

    set_APB_prescaler( HCLK_DIVIDED_BY_2 );
} // board_clocking_init

void 
board_gpio_init()
{
    // (1) Configure PA1-PA12 as output:
    enable_GPIOx_clock( GPIOA );
    enable_GPIOx_clock( GPIOC );

    set_range_GPIOx_OTYPER(
        GPIOA,
        PIN0, PIN15,
        PUSH_PULL_TYPE
    );

    set_range_GPIOx_MODER(
        GPIOA, 
        PIN1, PIN12,
        GENERAL_PURPOSE_OUTPUT_MODE
    );

    set_GPIOx_MODER( GPIOA, PIN0, INPUT_MODE );
    set_GPIOx_PUPDR( GPIOA, PIN0, PULL_DOWN_PUPDR );

    set_range_GPIOx_OTYPER(
        GPIOC,
        PIN0, PIN15, 
        PUSH_PULL_TYPE
    );

    set_GPIOx_MODER( GPIOC, PIN0, GENERAL_PURPOSE_OUTPUT_MODE );
    set_GPIOx_ODR( GPIOC, PIN0, 1 );

    set_range_GPIOx_MODER(
        GPIOC, 
        PIN8, PIN9, 
        GENERAL_PURPOSE_OUTPUT_MODE 
    );

    set_range_GPIOx_MODER(
        GPIOC,
        PIN1, PIN2,
        INPUT_MODE
    );
    set_range_GPIOx_PUPDR(
        GPIOC,
        PIN1, PIN2,
        PULL_DOWN_PUPDR
    );
} // 


