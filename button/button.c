#include "button.h"

static void syscfgr_Enable(GPIOx port_, unsigned pin_)
{
    //stm32f0xx_rm 169 page
    unsigned offset = port_; //offset
    volatile uint32_t* reg = (void* )0; //register

    if(pin_ < 4)
    {
        reg = SYSCFG_EXTICR1;
    }
    else if(pin_ >= 4 && pin_ < 8)
    {
        reg = SYSCFG_EXTICR2;
    }
    else if(pin_ >= 8 && pin_ < 12)
    {
        reg = SYSCFG_EXTICR3;
    }
    else if(pin_ >= 12 && pin_ < 16)
    {
        reg = SYSCFG_EXTICR4;
    }
    *reg |= offset << (4 * (pin_ % 4));
}

void button_init(button_t* btn_, GPIOx port_, unsigned pin_, void (*cmd_)(void))
{
    btn_->pressed  = false;
    btn_->port = port_;
    btn_->pin  = pin_;
    btn_->cmd  = cmd_;

    //REG_RCC_AHBENR_PORT ENABLE
    *REG_RCC_AHBENR |= 1u << (port_ + 17);
    set_GPIOx_MODER(port_, pin_, ALTERNATE_FUNCTION_MODE);
    set_GPIOx_OTYPER(port_, pin_, PUSH_PULL_TYPE);
    set_GPIOx_PUPDR(port_, pin_, PULL_DOWN_PUPDR);

    SET_BIT(REG_RCC_APB2ENR, 0);
    syscfgr_Enable(port_, pin_);

    EXTI_IMR_UNMASK_IRQ(EXTI_IMR, pin_);
    EXTI_RTSR_ENABLE(EXTI_RTSR, pin_);
    EXTI_FTSR_ENABLE(EXTI_RTSR, pin_);

    SET_BIT(NVIC_ISER, 5);//gug
    *NVIC_IPR0 = 0b00000000U; // gug
}

void button_on_response(button_t* btn_)
{
    if(btn_->pressed)
        btn_->cmd();
    btn_->pressed = !btn_->pressed;
}

// No need to eliminate contact bounce due to Schmidt trigger, see page 155 on stm32f0xx_rm.pdf