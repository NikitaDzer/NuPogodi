#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "button.h"

//-------------------
// RCC configuration
//-------------------

#define ONE_MILLISECOND 48000U

struct led_t led8;
struct led_t led9;

button_t btn1;
button_t btn2;

void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    REG_RCC_CR_HSEON(REG_RCC_CR);
    WAIT_FOR(REG_RCC_CR, REG_RCC_HSEON_CHECK);
    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    REG_RCC_CFGR2_CONF_PLL(REG_RCC_CFGR2, 2U);

    // (3) Select PREDIV output as PLL input (4 MHz):
    REG_RCC_CFGR_PLL_SET_SRC(REG_RCC_CFGR, HSI_SRC);

    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    REG_RCC_CFGR_PLLMUL_SET_MULT(REG_RCC_CFGR, 12);

    // (5) Enable PLL:
    REG_RCC_CR_PLL_ENABLE(REG_RCC_CR);
    WAIT_FOR(REG_RCC_CR, REG_RCC_CR_PLL_ENABLE_CHECK);
    // (6) Configure AHB frequency to 48 MHz:
    REG_RCC_CFGR_SET_AHB(REG_RCC_CFGR, AHB_FREQ_48);

    // (7) Select PLL as SYSCLK source:
    REG_RCC_CFGR_SET_SYSCLK_SRC(REG_RCC_CFGR, PLL_SRC);
    WAIT_FOR(REG_RCC_CFGR, REG_RCC_CFGR_SET_SYSCLK_SRC_CHECK);
    // (8) Set APB frequency to 48 MHz
    REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_1(REG_RCC_CFGR);
}

void timing_perfect_delay(uint32_t millis)
{
    unsigned ticks;
    ticks = millis * (ONE_MILLISECOND) / 10;

    __asm__(
        "1: ldr r3, [r7, #12] \n\t"//2
        "cmp r3, #0 \n\t"//1
        "beq 2f \n\t" //1
        "sub r3, #1 \n\t" //1
        "str r3, [r7, #12] \n\t" //2
        "b 1b \n\t" //3
        "2: \n\t"
    );
    ticks+=0;
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 and PC9:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);
}


//works for ports A_F and pins 0 or 1
void EXTI0_IRQHandler(void)
{
    if (READ_BIT(EXTI_PR, 0) == 1)
    {
        led_on(&led9);
        timing_perfect_delay(100);
        led_off(&led9);
        button_on_response(&btn1);
        SET_BIT(EXTI_PR, 0);
    }
    else if(READ_BIT(EXTI_PR, 1) == 1)
    {
        button_on_response(&btn2);
        SET_BIT(EXTI_PR, 1);
    }
}


//------
// Main
//------

void cmd1(void)
{
    led_on(&led8);
    timing_perfect_delay(100);
    led_off(&led8);
    timing_perfect_delay(100);
    led_on(&led8);
    timing_perfect_delay(100);
}

void cmd2(void)
{
    led_off(&led8);
    timing_perfect_delay(100);
    led_on(&led8);
    timing_perfect_delay(100);
    led_off(&led8);
    timing_perfect_delay(100);
    led_on(&led8);
    timing_perfect_delay(100);
    led_off(&led8);
    timing_perfect_delay(100);
}

int main(void)
{
    board_clocking_init();
    board_gpio_init();

    button_init(&btn1, GPIOA, 0, &cmd1);
    button_init(&btn2, GPIOC, 1, &cmd2);

    led_init(&led8, GPIOC, 8);
    led_init(&led9, GPIOC, 9);

    while(1)
    {}
    return 0;
}

