#include "../include/button.h"
#include "../include/interrupts.h"

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

void button_init(button_t* btn_, GPIOx port_, unsigned pin_, void (*cmd_)(button_t*))
{
    btn_->pressed  = false;
    btn_->active   = false;
    btn_->port = port_;
    btn_->pin  = pin_;
    btn_->cmd  = cmd_;

    //REG_RCC_AHBENR_PORT ENABLE
    *REG_RCC_AHBENR |= 1u << (port_ + 17);
    set_GPIOx_MODER(port_, pin_, ALTERNATE_FUNCTION_MODE);
    set_GPIOx_OTYPER(port_, pin_, PUSH_PULL_TYPE);
    set_GPIOx_PUPDR(port_, pin_, PULL_DOWN_PUPDR);

    SET_BIT(RCC_BASE_ADDR + RCC_APB2ENR_OFFSET, 0);
    syscfgr_Enable(port_, pin_);

    SET_BIT(EXTI_IMR,  pin_);
    // SET_BIT(EXTI_FTSR, pin_);
    SET_BIT(EXTI_RTSR, pin_);

    // stm32f0xx_rm 209 page
    const uint32_t EXTI0_SHIFT = 5U;
    const uint32_t EXTI1_SHIFT = 6U;
    const uint32_t EXTI2_SHIFT = 7U;
    
    if(pin_ == 0 || pin_ == 1)
        SET_BIT( NVIC_ISER, EXTI0_SHIFT );
    else if(pin_ == 2 || pin_ == 3)
        SET_BIT( NVIC_ISER, EXTI1_SHIFT );
    else if(pin_ >= 4 && pin_ <= 15)
        SET_BIT( NVIC_ISER, EXTI2_SHIFT );

    *NVIC_IPR0 = 0b00000000U; // gug
}

// No need to eliminate contact bounce due to Schmidt trigger, see page 155 on stm32f0xx_rm.pdf
