#pragma once

#include <stdint.h>
#include <stdbool.h>

#define REG_RCC_CR     (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR   (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2  (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2

#define RCC_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x40021000U

#define RCC_CR_OFFSET       (0x00U)
#define RCC_CFGR_OFFSET     (0x04U)
#define RCC_CIR_OFFSET      (0x08U)
#define RCC_APB2RSTR_OFFSET (0x0CU)
#define RCC_APB1RSTR_OFFSET (0x10U)
#define RCC_AHBENR_OFFSET   (0x14U)
#define RCC_APB2ENR_OFFSET  (0x18U)
#define RCC_APB1ENR_OFFSET  (0x1CU)
#define RCC_BDCR_OFFSET     (0x20U)
#define RCC_CSR_OFFSET      (0x24U)
#define RCC_AHBRSTR_OFFSET  (0x28U)
#define RCC_CFGR2_OFFSET    (0x2CU)
#define RCC_CFGR3_OFFSET    (0x30U)
#define RCC_CR2_OFFSET      (0x34U)

#define GPIOA_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000000U
#define GPIOB_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000400U
#define GPIOC_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000800U
#define GPIOD_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000C00U
#define GPIOE_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48001000U
#define GPIOF_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48001400U

#define GPIOx_MODER_OFFSET  (0x00U)
#define GPIOx_OTYPER_OFFSET (0x04U)
#define GPIOx_PUPDR_OFFSET  (0x0CU)
#define GPIOx_IDR_OFFSET    (0x10U)
#define GPIOx_ODR_OFFSET    (0x14U)

#define PIN0  (0U)
#define PIN1  (1U)
#define PIN2  (2U)
#define PIN3  (3U)
#define PIN4  (4U)
#define PIN5  (5U)
#define PIN6  (6U)
#define PIN7  (7U)
#define PIN8  (8U)
#define PIN9  (9U)
#define PIN10 (10U)
#define PIN11 (11U)
#define PIN12 (12U)
#define PIN13 (13U)
#define PIN14 (14U)
#define PIN15 (15U)

typedef enum SYSCLK
{
    HSI,
    HSE,
    PLL,
    HSI48,
} SYSCLK;

typedef enum HCLK
{
    SYSCLK_NOT_DIVIDED    = 0b0000,
    SYSCLK_DIVIDED_BY_2   = 0b1000,
    SYSCLK_DIVIDED_BY_4   = 0b1001,
    SYSCLK_DIVIDED_BY_8   = 0b1010,
    SYSCLK_DIVIDED_BY_16  = 0b1011,
    SYSCLK_DIVIDED_BY_64  = 0b1100,
    SYSCLK_DIVIDED_BY_128 = 0b1101,
    SYSCLK_DIVIDED_BY_256 = 0b1110,
    SYSCLK_DIVIDED_BY_512 = 0b1111,
} HCLK;

typedef enum PCLK
{
    HCLK_NOT_DIVIDED   = 0b000,
    HCLK_DIVIDED_BY_2  = 0b100,
    HCLK_DIVIDED_BY_4  = 0b101,
    HCLK_DIVIDED_BY_8  = 0b111,
} PCLK;

typedef enum PREDIV
{
    PREDIV_NOT_DIVIDED   = 0b0000,
    PREDIV_DIVIDED_BY_2  = 0b0001,
    PREDIV_DIVIDED_BY_3  = 0b0010,
    PREDIV_DIVIDED_BY_4  = 0b0011,
    PREDIV_DIVIDED_BY_5  = 0b0100,
    PREDIV_DIVIDED_BY_6  = 0b0101,
    PREDIV_DIVIDED_BY_7  = 0b0110,
    PREDIV_DIVIDED_BY_8  = 0b0111,
    PREDIV_DIVIDED_BY_9  = 0b1000,
    PREDIV_DIVIDED_BY_10 = 0b1001,
    PREDIV_DIVIDED_BY_11 = 0b1010,
    PREDIV_DIVIDED_BY_12 = 0b1011,
    PREDIV_DIVIDED_BY_13 = 0b1100,
    PREDIV_DIVIDED_BY_14 = 0b1101,
    PREDIV_DIVIDED_BY_15 = 0b1110,
    PREDIV_DIVIDED_BY_16 = 0b1111,
} PREDIV;

typedef enum PLLSRC
{
    HSI_DIVIDED_BY_2        = 0b00,
    HSI_DIVIDED_BY_PREDIV   = 0b01,
    HSE_DIVIDED_BY_PREDIV   = 0b10,
    HSI48_DIVIDED_BY_PREDIV = 0b11,
} PLLSRC;

typedef enum PLLMUL
{
    PLL_MULTIPLIED_BY_2  = 0b0000,
    PLL_MULTIPLIED_BY_3  = 0b0001,
    PLL_MULTIPLIED_BY_4  = 0b0010,
    PLL_MULTIPLIED_BY_5  = 0b0011,
    PLL_MULTIPLIED_BY_6  = 0b0100,
    PLL_MULTIPLIED_BY_7  = 0b0101,
    PLL_MULTIPLIED_BY_8  = 0b0110,
    PLL_MULTIPLIED_BY_9  = 0b0111,
    PLL_MULTIPLIED_BY_10 = 0b1000,
    PLL_MULTIPLIED_BY_11 = 0b1001,
    PLL_MULTIPLIED_BY_12 = 0b1010,
    PLL_MULTIPLIED_BY_13 = 0b1011,
    PLL_MULTIPLIED_BY_14 = 0b1100,
    PLL_MULTIPLIED_BY_15 = 0b1101,
    PLL_MULTIPLIED_BY_16 = 0b1110,
} PLLMUL;

typedef enum GPIOx
{
        GPIOA,
        GPIOB,
        GPIOC,
        GPIOD,
        GPIOE,
        GPIOF,
} GPIOx;

typedef enum GPIOx_MODER
{
        INPUT_MODE                  = 0b00,
        GENERAL_PURPOSE_OUTPUT_MODE = 0b01,
        ALTERNATE_FUNCTION_MODE     = 0b10,
        ANALOG_MODE                 = 0b11,
} GPIOx_MODER;

typedef enum GPIOx_OTYPER
{
    PUSH_PULL_TYPE  = 0b0,
    OPEN_DRAIN_TYPE = 0b1,
} GPIOx_OTYPER;

typedef enum GPIOx_PUPDR
{
    NO_PUPDR        = 0b00,
    PULLUP_PUPDR    = 0b01,
    PULL_DOWN_PUPDR = 0b10,
} GPIOx_PUPDR;


volatile uint8_t *
get_GPIOx_base_addr( GPIOx gpio );

volatile uint32_t *
get_GPIOx_reg_addr( GPIOx gpio, 
                    uint32_t offset );

void 
set_GPIOx_MODER( GPIOx gpio, 
                 uint32_t pin, 
                 GPIOx_MODER moder);

void 
set_GPIOx_OTYPER( GPIOx gpio, 
                  uint32_t pin, 
                  GPIOx_OTYPER typer );

void 
set_GPIOx_PUPDR( GPIOx gpio, 
                 uint32_t pin, 
                 GPIOx_PUPDR pupdr );

void 
set_GPIOx_ODR( GPIOx gpio, 
               uint32_t pin, 
               uint32_t value );

void
set_GPIOx_ODR_whole( GPIOx gpio,
                     uint32_t value );

uint32_t
get_GPIOx_ODR_whole( GPIOx gpio );

uint32_t
get_GPIOx_ODR( GPIOx gpio,
               uint32_t pin );

uint32_t 
get_GPIOx_IDR_whole( GPIOx gpio );

uint32_t 
get_GPIOx_IDR( GPIOx gpio, 
               uint32_t pin );

void 
set_range_GPIOx_MODER( GPIOx gpio, 
                       uint32_t pin_start, uint32_t pin_end, 
                       GPIOx_MODER moder );

void 
set_range_GPIOx_OTYPER( GPIOx gpio, 
                        uint32_t pin_start, uint32_t pin_end, 
                        GPIOx_OTYPER typer );

void 
set_range_GPIOx_PUPDR( GPIOx gpio, 
                       uint32_t pin_start, uint32_t pin_end, 
                       GPIOx_PUPDR pupdr );

void
enable_HSE_clock();

void
enable_PLL_clock();

void
select_SYSCLK_source( SYSCLK source );

void
set_AHB_prescaler( HCLK prescaler );

void
set_PLL_prescaler( PREDIV prescaler );

void
set_PLL_source( PLLSRC source );

void
set_PLL_multiplier( PLLMUL factor );

void
set_APB_prescaler( PCLK prescaler );

void
enable_GPIOx_clock( GPIOx gpio );
