#include "../include/interrupts.h"
#include "../include/lib.h"
#include "../include/bit.h"
#include "../include/bits.h"
#include "../include/init.h"

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
    enable_GPIOx_clock( GPIOA );
    enable_GPIOx_clock( GPIOB );
    enable_GPIOx_clock( GPIOC );

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

} // board_gpio_init

void
board_i2c_init()
{
    select_I2Cx_source( I2C1, I2Cx_SOURCE_SYSCLK );
    disable_I2Cx_peripheral( I2C1 );
    enable_APB1_peripheral_clock( I2C1EN );

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

/*
 * Configure timer to output compare mode.
 */
void
board_timers_init( uint32_t prescaler )
{
    /*
     * Configure output channel
     */
    enable_GPIOx_clock( GPIOA );
    set_GPIOx_MODER( GPIOA, PIN0, ALTERNATE_FUNCTION_MODE );
    set_GPIOx_AFRL( GPIOA, PIN0, AF2 );

    /*
     * Setup timer to output compare mode
     */
    enable_APB1_peripheral_clock( TIM2EN );
    enable_APB1_peripheral_clock( TIM3EN );

    set_TIM3_prescaler( 0 );
    set_TIM3_auto_reload( 65535 );
    set_TIM3_counter_mode( COUNTER_MODE_UP );
    enable_TIM3_counter();

    set_TIM2_prescaler( prescaler );
    set_TIM2_auto_reload( 999 );
    set_TIM2_compare_ch1( 700 );
    enable_TIM2_ch1();
    set_TIM2_polarity( POLARITY_HIGH );
    set_TIM2_ch1_mode( CAPTURE_COMPARE_PWM1 );
    set_TIM2_counter_mode( COUNTER_MODE_UP );
    enable_TIM2_counter();

}

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define SHPR3 (volatile uint32_t*)(0xE000ED20)

void 
board_systick_init( uint32_t poll_period_us )
{
    // (0) Read STM32F051 SysTick configuration:
    // Assumptions:
    // - There is a reference clock and it can be chosen as clock source.
    // - The SYST_CALIB SKEW bit is 1.
    uint32_t reload_value = (poll_period_us * ( CPU_FREQENCY / 1000000U)) / 8;

    // (1) Program the reload value:
    mask_bits(SYSTICK_RVR, SYST_RVR_RELOAD, reload_value);

    // (2) Clear the current value:
    clear_bits(SYSTICK_CVR, SYST_CVR_CURRENT);

    // (3) Program the CSR:
    // Watch out for the clock source!
    set_bits(SYSTICK_CSR, SYST_CSR_ENABLE | SYST_CSR_TICKINT);

    uint32_t bitmask = create_bitmask( 24, 8, 0xFFFF );
    mask_bits( SHPR3, bitmask, 0 );
}

