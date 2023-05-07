#include "lib.h"
#include "init.h"

void 
board_clocking_init()
{
    enable_HSE_clock();

    set_PLL_prescaler( PREDIV_DIVIDED_BY_2 );
    set_PLL_source( HSE_DIVIDED_BY_PREDIV );
    set_PLL_multiplier( PLL_MULTIPLIED_BY_4 ); // 16 MHz
    enable_PLL_clock();

    set_AHB_prescaler( SYSCLK_NOT_DIVIDED );
    select_SYSCLK_source( SYSCLK_SOURCE_PLL );
    set_APB_prescaler( HCLK_NOT_DIVIDED );

} // board_clocking_init

void 
board_gpio_init()
{
    enable_GPIOx_clock( GPIOB );

    set_range_GPIOx_OTYPER(
        GPIOB,
        PIN6, PIN7,
        OPEN_DRAIN_TYPE
    );

    set_range_GPIOx_MODER(
        GPIOB, 
        PIN6, PIN7,
        ALTERNATE_FUNCTION_MODE
    );

    set_range_GPIOx_AFRL(
        GPIOB,
        PIN6, PIN7,
        AF1
    );

    set_range_GPIOx_OSPEEDR(
        GPIOB,
        PIN6, PIN7,
        HIGH_SPEED
    );
} // board_gpio_init

void
board_i2c_init()
{
    select_I2Cx_source( I2C1, I2Cx_SOURCE_SYSCLK );
    disable_I2Cx_peripheral( I2C1 );
    enable_APB_peripheral_clock( I2C1EN );

    disable_I2Cx_analog_filter( I2C1 );
    set_I2Cx_digital_filter( I2C1, 1 );

    set_I2Cx_400khz( I2C1 );
    disable_I2Cx_clock_stretching( I2C1 );

    set_I2Cx_addressing_mode( I2C1, I2Cx_7BIT );

    set_I2Cx_own_address_mode( I2C1, I2Cx_7BIT );    
    enable_I2Cx_own_address( I2C1 );

    set_I2Cx_mode_I2C( I2C1 );

    enable_I2Cx_peripheral( I2C1 );
} // board_i2c_init

