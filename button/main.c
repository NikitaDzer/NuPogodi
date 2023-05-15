#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "button.h"
#include "../init.h"

#define ONE_MILLISECOND 48000U

led_t led8;
led_t led9;

button_t btn0;
button_t btn1;

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


//works for pins 0 ... 15 for each one port
void EXTI0_IRQHandler(void)
{
    if (READ_BIT(EXTI_PR, 0) == 1)
    {
        btn0.cmd();
        SET_BIT(EXTI_PR, 0);
    }
    else if(READ_BIT(EXTI_PR, 1) == 1)
    {
        btn1.cmd();
        SET_BIT(EXTI_PR, 0);
    }
}

void EXTI1_IRQHandler(void)
{
    if (READ_BIT(EXTI_PR, 2) == 1)
    {
        btn0.cmd();
        SET_BIT(EXTI_PR, 0);
    }
    else if(READ_BIT(EXTI_PR, 3) == 1)
    {
        btn1.cmd();
        SET_BIT(EXTI_PR, 0);
    }
}

void EXTI2_IRQHandler(void)
{
    if (READ_BIT(EXTI_PR, 4) == 1)
    {
        //btn4.cmd();
        SET_BIT(EXTI_PR, 0);
    }
    else if(READ_BIT(EXTI_PR, 5) == 1)
    {
        //btn5.cmd();
        SET_BIT(EXTI_PR, 0);
    }
    //...
}

//------
// Main
//------

void cmd1(void)
{
    static bool flag = false;
    flag = !flag;
    if(flag)
        led_on(&led8);
    else
        led_off(&led8);
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

    button_init(&btn0, GPIOA, 2, &cmd1);
    button_init(&btn1, GPIOC, 1, &cmd2);

    led_init(&led8, GPIOC, 8);
    led_init(&led9, GPIOC, 9);

    while(1)
    {}
    return 0;
}
