#pragma once

#include <stdbool.h>


//--------------------------------/--------------------------------//

//---------------
// Bit arithmetic 
//---------------

#define SET_BIT(REG, BIT) (*(REG) |= 1u << (BIT))                   // выставление бита BIT в регистре REG в 1
#define CLEAR_BIT(REG, BIT) (*(REG) &= (~(1u << (BIT))))            // выставление бита BIT в регистре REG в 0
#define READ_BIT(REG, BIT) ((*(REG) >> (BIT)) & 1u)                 // возвращает бит BIT регистра REG
#define WRITE_REG(REG, VAL) (*(REG) = (VAL))                        // выставление регистра REG в VAL
#define READ_REG(REG) (*(REG))                                    // возвращает регистр REG

#define MODIFY_REG(REG, MODIFYMASK, VALUE) (*(REG) = ((*(REG) & (~(MODIFYMASK))) | (VALUE & MODIFYMASK)))
#define WAIT_FOR(REG, MASK) ({ while ((READ_REG(REG) & (MASK)) != (MASK));})


//--------------------------------/--------------------------------//

//-------
// Masks
//-------

#define REG_RCC_CFGR_PLL_SET_SRC_MASK (0b11U << 15)
#define REG_RCC_CFGR_PLLMUL_MASK (15U << 18)
#define REG_RCC_CFGR_AHB_MASK (0b1111U << 4)
#define REG_RCC_CFGR_SYSCLK_MASK (0b11U)
#define REG_RCC_CFGR_PCLK_PRESCALER_MASK (0b111U << 8)
#define REG_RCC_CFGR2_PLL_MASK (0b11U)
#define REG_RCC_HSEON_CHECK (1U << 17)
#define REG_RCC_CR_PLL_ENABLE_CHECK (1U << 25)
#define REG_RCC_CFGR_SET_SYSCLK_SRC_CHECK (1U << 3)
 
//------------
// Constants
//------------

#define PLL_SRC (0b10U)
#define AHB_FREQ_48 0U
#define HSI_SRC (1U << 15)
#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
 
//---------------
// RCC Registers
//---------------
 
#define REG_RCC_CR     (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR   (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2  (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2
#define REG_RCC_APB2ENR (volatile uint32_t*)(uintptr_t)0x40021018U // APB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR3   (volatile uint32_t*)(uintptr_t)0x40021030U // Clock configuration register 2
#define REG_RCC_APB1RSTR (volatile uint32_t*)(uintptr_t)0x40021010U
#define REG_RCC_APB2RSTR (volatile uint32_t*)(uintptr_t)0x4002100CU

//--------------
// RCC macro
//--------------

#define REG_RCC_CR_HSEON(REG) (SET_BIT((REG), 16U))
#define REG_RCC_CFGR2_CONF_PLL(REG, DIV) (MODIFY_REG((REG), REG_RCC_CFGR2_PLL_MASK, ((DIV) - 1))) //DIV - divider
#define REG_RCC_CFGR_PLL_SET_SRC(REG, SRC) (MODIFY_REG((REG), REG_RCC_CFGR_PLL_SET_SRC_MASK, (SRC)))
#define REG_RCC_CFGR_PLLMUL_SET_MULT(REG, MULT) (MODIFY_REG((REG), REG_RCC_CFGR_PLLMUL_MASK, ((MULT) - 2U) << 18))
#define REG_RCC_CR_PLL_ENABLE(REG) (SET_BIT((REG), 24))
#define REG_RCC_CFGR_SET_AHB(REG, FREQ) (MODIFY_REG((REG), REG_RCC_CFGR_AHB_MASK, (FREQ) << 4))
#define REG_RCC_CFGR_SET_SYSCLK_SRC(REG, SRC) (MODIFY_REG((REG), REG_RCC_CFGR_SYSCLK_MASK, (SRC)))
#define REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_2(REG) (MODIFY_REG((REG), REG_RCC_CFGR_PCLK_PRESCALER_MASK, (0b100 << 8)))
#define REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_1(REG) (MODIFY_REG((REG), REG_RCC_CFGR_PCLK_PRESCALER_MASK, (0b000 << 8)))
#define REG_RCC_AHBENR_PORT_A_ENABLE(REG) (SET_BIT((REG), 17))
#define REG_RCC_AHBENR_PORT_B_ENABLE(REG) (SET_BIT((REG), 18))
#define REG_RCC_AHBENR_PORT_C_ENABLE(REG) (SET_BIT((REG), 19))
#define REG_RCC_AHBENR_PORT_D_ENABLE(REG) (SET_BIT((REG), 20))
#define REG_RCC_AHBENR_PORT_E_ENABLE(REG) (SET_BIT((REG), 21))
#define REG_RCC_AHBENR_PORT_F_ENABLE(REG) (SET_BIT((REG), 22))


//--------------------------------/--------------------------------//


//------------------
// SYSCFGR registers
//------------------

#define SYSCFG_CFGR1    (volatile uint32_t*)(uintptr_t)0x40010000U
#define SYSCFG_CFGR2    (volatile uint32_t*)(uintptr_t)0x40010018U
#define SYSCFG_EXTICR1  (volatile uint32_t*)(uintptr_t)0x40010008U
#define SYSCFG_EXTICR2  (volatile uint32_t*)(uintptr_t)0x4001000CU
#define SYSCFG_EXTICR3  (volatile uint32_t*)(uintptr_t)0x40010010U
#define SYSCFG_EXTICR4  (volatile uint32_t*)(uintptr_t)0x40010014U


//---------------
// EXTI registers
//---------------

#define EXTI_IMR    (volatile uint32_t*)(uintptr_t)0x40010400U
#define EXTI_EMR    (volatile uint32_t*)(uintptr_t)0x40010404U
#define EXTI_RTSR   (volatile uint32_t*)(uintptr_t)0x40010408U
#define EXTI_FTSR   (volatile uint32_t*)(uintptr_t)0x4001040CU
#define EXTI_SWIER  (volatile uint32_t*)(uintptr_t)0x40010410U
#define EXTI_PR     (volatile uint32_t*)(uintptr_t)0x40010414U

//------
// Macro
//------

#define EXTI_IMR_MASK_IRQ(REG, LINE) (CLEAR_BIT(REG, LINE))
#define EXTI_IMR_UNMASK_IRQ(REG, LINE) (SET_BIT(REG, LINE))
#define EXTI_IER_MASK_IRQ(REG, LINE) (CLEAR_BIT(REG, LINE))
#define EXTI_IER_UNMASK_IRQ(REG, LINE) (SET_BIT(REG, LINE))
#define EXTI_RTSR_ENABLE(REG, BIT) (SET_BIT(REG, BIT))
#define EXTI_FTSR_ENABLE(REG, BIT) (SET_BIT(REG, BIT))
#define SYSCFGR_EXTICR1_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * PIN)), (0b0000u << (4 * PIN))))
#define SYSCFGR_EXTICR2_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * (PIN - 4))), ((LETTER - 'A') << (4 * (PIN - 4)))))
#define SYSCFGR_EXTICR3_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * (PIN - 8))), ((LETTER - 'A') << (4 * (PIN - 8)))))
#define SYSCFGR_EXTICR4_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * (PIN - 12))), ((LETTER - 'A') << (4 * (PIN - 12)))))


//--------------------------------/--------------------------------//


//----------
// Registers
//----------

#define NVIC_ISER (volatile uint32_t*)(uintptr_t)0xE000E100
#define NVIC_ICER (volatile uint32_t*)(uintptr_t)0xE000E180
#define NVIC_ISPR (volatile uint32_t*)(uintptr_t)0xE000E200
#define NVIC_ICPR (volatile uint32_t*)(uintptr_t)0xE000E280
#define NVIC_IPR0 (volatile uint32_t*)(uintptr_t)0xE000E400
#define NVIC_IPR1 (volatile uint32_t*)(uintptr_t)0xE000E404
#define NVIC_IPR2 (volatile uint32_t*)(uintptr_t)0xE000E408
#define NVIC_IPR3 (volatile uint32_t*)(uintptr_t)0xE000E40C
#define NVIC_IPR4 (volatile uint32_t*)(uintptr_t)0xE000E410
#define NVIC_IPR5 (volatile uint32_t*)(uintptr_t)0xE000E414
#define NVIC_IPR6 (volatile uint32_t*)(uintptr_t)0xE000E418
#define NVIC_IPR7 (volatile uint32_t*)(uintptr_t)0xE000E41C


//--------------------------------/--------------------------------//


//----------------
// GPIO Registers
//----------------

#define GPIOA_MODER (volatile uint32_t*)(uintptr_t)0x48000000U // GPIO port mode register
#define GPIOA_TYPER (volatile uint32_t*)(uintptr_t)0x48000004U // GPIO port output type register
#define GPIOA_PUPDR (volatile uint32_t*)(uintptr_t)0x4800000CU // GPIO port pull-up/pull-down register
#define GPIOA_IDR   (volatile uint32_t*)(uintptr_t)0x48000010U // GPIO port input  data register
#define GPIOA_ODR   (volatile uint32_t*)(uintptr_t)0x48000014U // GPIO port output data register
#define GPIOA_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000008U // GPIO port output speed register
#define GPIOA_AFRH    (volatile uint32_t*)(uintptr_t)0x48000024U // GPIO alternate function high register
#define GPIOA_AFRL    (volatile uint32_t*)(uintptr_t)0x48000020U

#define GPIOC_MODER (volatile uint32_t*)(uintptr_t)0x48000800U      // GPIO port mode register
#define GPIOC_TYPER (volatile uint32_t*)(uintptr_t)0x48000804U      // GPIO port output type register
#define GPIOC_ODR (volatile uint32_t*)(uintptr_t) 0x48000814U       // GPIO port output register
#define GPIOC_BSRR (volatile uint32_t*)(uintptr_t) 0x48000818U      // GPIO port bit set/reset register
#define GPIOC_PUPDR (volatile uint32_t*)(uintptr_t)0x4800080CU

//--------------
// GPIO macro
//--------------

#define GPIO_MODER_PORT_SET_MODE_OUTPUT(REG, BIT) (SET_BIT((REG), 2u * (BIT)))
#define GPIO_MODER_PORT_SET_MODE_INPUT(REG, BIT) (MODIFY_REG(REG, (0b11u << (2 * BIT)), (0b11u << (2 * BIT))))
#define GPIO_MODER_PORT_SET_MODE_ALT(REG, BIT) (MODIFY_REG(REG, (0b11u << (2 * BIT)), (0b10u << (2 * BIT))))
#define GPIO_TYPER_PORT_SET_PUSH_PULL(REG, BIT) (CLEAR_BIT((REG), (BIT)))
#define GPIO_TYPER_PORT_SET_OPEN_DRAIN(REG, BIT) (SET_BIT((REG), (BIT)))
#define GPIO_BSRR_BIT_SET(REG, BIT) (SET_BIT(REG, BIT))
#define GPIO_BSRR_BIT_RESET(REG, BIT) (SET_BIT(REG, (BIT + 16)))
#define GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT) (*(REG) = *(REG) & (~(0b11u << (2 * BIT))))
#define GPIO_PUPDR_PORT_PULL_UP_SET(REG, BIT) ({GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT); SET_BIT((REG), 2u * (BIT));})
#define GPIO_PUPDR_PORT_PULL_DOWN_SET(REG, BIT) ({GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT); SET_BIT((REG), (2u  * (BIT) + 1u));})
#define GPIO_AFRH_SET_ALT(REG, PIN) (MODIFY_REG(REG, (0b1111u << (4u * (PIN - 8u))), (0b0001u << (4u * (PIN - 8u)))))
#define GPIO_AFRL_SET_ALT(REG, PIN) (MODIFY_REG(REG, (0b1111u << (4u * (PIN))), (0b0001u << (4u * (PIN)))))
#define GPIO_OSPEEDR_SET_FAST(REG, PIN) (MODIFY_REG(REG, (0b11u << (2u * (PIN))), (0b11u << (2u * (PIN)))))
#define GPIO_OSPEEDR_SET_MEDIUM(REG, PIN) (MODIFY_REG(REG, (0b11u << (2u * (PIN))), (0b01u << (2u * (PIN)))))
#define GPIO_OSPEEDR_SET_LOW(REG, PIN) (MODIFY_REG(REG, (0b11u << (2u * (PIN))), (0b00u << (2u * (PIN)))))


//--------------------------------/--------------------------------//
