#include "lib.h"

/**
  * System Clock Configuration
  * The system Clock is configured as follow :
  *    System Clock source            = PLL (HSI/2)
  *    SYSCLK(Hz)                     = 48000000
  *    HCLK(Hz)                       = 48000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 1
  *    HSI Frequency(Hz)              = 8000000
  *    PLLMUL                         = 12
  */

static void
init_clock()
{
    enable_HSI_clock();

    set_PLL_prescaler( PREDIV_DIVIDED_BY_2 );
    set_PLL_source( HSI_DIVIDED_BY_PREDIV );
    set_PLL_multiplier( PLL_MULTIPLIED_BY_12 );
    enable_PLL_clock();

    set_AHB_prescaler( SYSCLK_NOT_DIVIDED );
    select_SYSCLK_source( SYSCLK_SOURCE_PLL );
    set_APB_prescaler( HCLK_NOT_DIVIDED );
}

/*
 * Clock on GPIOC and set one led
 */
static void
gpio_config()
{
    enable_GPIOx_clock( GPIOC );
    /* set_GPIOx_OTYPER( GPIOC, PIN8, PUSH_PULL_TYPE ); */
    set_GPIOx_MODER( GPIOC, PIN8, GENERAL_PURPOSE_OUTPUT_MODE );
}


/*
 * Configure timer to output compare mode
 */
static void
timers_config( uint32_t prescaler )
{
    /*
     * Configure output channel
     */
    enable_GPIOx_clock( GPIOA );
    set_GPIOx_MODER( GPIOA, PIN5, ALTERNATE_FUNCTION_MODE );
    set_GPIOx_AFRL( GPIOA, PIN5, AF2 );

    /*
     * Setup timer to output compare mode
     */
    enable_APB1_peripheral_clock( TIM2EN );
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
#define ONE_MILLISECOND CPU_FREQENCY/1000U

void delay()
{
    for (uint32_t i = 0; i <  ( ONE_MILLISECOND * 1000 )/3U; ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

int main()
{
    init_clock();
    gpio_config();


    // prescaler define the frequency
    // ( the frequency is counted as 48 MHz / ( prescaler + 1 ) / 1000
    timers_config( 10999 );
    delay();
    /* set_TIM2_prescaler( 49 ); */
    set_TIM2_prescaler( 49 );

    while ( 1 ) {};
}
