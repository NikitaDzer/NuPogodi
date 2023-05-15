#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SET_BIT(REG, BIT)   (*(REG) |= 1u << (BIT))
#define CLEAR_BIT(REG, BIT) (*(REG) &= (~(1u << (BIT))))
#define READ_BIT(REG, BIT)  ((*(REG) >> (BIT)) & 1u)


#define REG_RCC_CR     (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR   (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2  (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2

#define SYSCFG_EXTICR1 (volatile uint32_t*)(uintptr_t)0x40010008U // Register for interruptions
#define SYSCFG_EXTICR2 (volatile uint32_t*)(uintptr_t)0x4001000CU
#define SYSCFG_EXTICR3 (volatile uint32_t*)(uintptr_t)0x40010010U
#define SYSCFG_EXTICR4 (volatile uint32_t*)(uintptr_t)0x40010014U

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

#define EXTI_IMR    (volatile uint32_t*)(uintptr_t)0x40010400U
#define EXTI_EMR    (volatile uint32_t*)(uintptr_t)0x40010404U
#define EXTI_RTSR   (volatile uint32_t*)(uintptr_t)0x40010408U
#define EXTI_FTSR   (volatile uint32_t*)(uintptr_t)0x4001040CU
#define EXTI_SWIER  (volatile uint32_t*)(uintptr_t)0x40010410U
#define EXTI_PR     (volatile uint32_t*)(uintptr_t)0x40010414U

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

#define I2C1_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x40005400U
#define I2C2_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x40005800U

#define I2Cx_CR1_OFFSET      (0x00U)
#define I2Cx_CR2_OFFSET      (0x04U)
#define I2Cx_OAR1_OFFSET     (0x08U)
#define I2Cx_OAR2_OFFSET     (0x0CU)
#define I2Cx_TIMINGR_OFFSET  (0x10U)
#define I2Cx_TIMOUTR_OFFSET  (0x14U)
#define I2Cx_ISR_OFFSET      (0x18U)
#define I2Cx_ICR_OFFSET      (0x1CU)
#define I2Cx_PECR_OFFSET     (0x20U)
#define I2Cx_RXDR_OFFSET     (0x24U)
#define I2Cx_TXDR_OFFSET     (0x28U)

#define GPIOA_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000000U
#define GPIOB_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000400U
#define GPIOC_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000800U
#define GPIOD_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48000C00U
#define GPIOE_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48001000U
#define GPIOF_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x48001400U

#define GPIOx_MODER_OFFSET   (0x00U)
#define GPIOx_OTYPER_OFFSET  (0x04U)
#define GPIOx_OSPEEDR_OFFSET (0x08U)
#define GPIOx_PUPDR_OFFSET   (0x0CU)
#define GPIOx_IDR_OFFSET     (0x10U)
#define GPIOx_ODR_OFFSET     (0x14U)
#define GPIOx_AFRL_OFFSET    (0x20U)

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

typedef enum I2Cx
{
    I2C1,
    I2C2,
} I2Cx;

typedef enum PERIPHERAL
{
    TIM2EN   = 0,
    TIM3EN   = 1,
    TIM6EN   = 4,
    TIM7EN   = 5,
    TIM14EN  = 8,
    WWDGEN   = 11,
    SPI2EN   = 14,
    USART2EN = 17,
    USART3EN = 18,
    USART4EN = 19,
    USART5EN = 20,
    I2C1EN   = 21,
    I2C2EN   = 22,
    USBEN    = 23,
    CANEN    = 25,
    CRSEN    = 27,
    PWREN    = 28,
    DACEN    = 29,
    CECEN    = 30,
} PERIPHERAL;

typedef enum I2Cx_SOURCE
{
    I2Cx_SOURCE_HSI    = 0b0,
    I2Cx_SOURCE_SYSCLK = 0b1,
} I2Cx_SOURCE;

typedef enum I2Cx_ADDR_MODE
{
    I2Cx_7BIT,
    I2Cx_10BIT,
} I2Cx_ADDR_MODE;

typedef enum SYSCLK_SOURCE
{
    SYSCLK_SOURCE_HSI,
    SYSCLK_SOURCE_HSE,
    SYSCLK_SOURCE_PLL,
    SYSCLK_SOURCE_HSI48,
} SYSCLK_SOURCE;

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

typedef enum GPIOx_OSPEEDR
{
    LOW_SPEED    = 0b00,
    MEDIUM_SPEED = 0b01,
    HIGH_SPEED   = 0b11,
} GPIOx_OSPEEDR;

typedef enum GPIOx_PUPDR
{
    NO_PUPDR        = 0b00,
    PULLUP_PUPDR    = 0b01,
    PULL_DOWN_PUPDR = 0b10,
} GPIOx_PUPDR;

typedef enum GPIOx_AFSEL
{
    AF0 = 0b0000,
    AF1 = 0b0001,
    AF2 = 0b0010,
    AF3 = 0b0011,
    AF4 = 0b0100,
    AF5 = 0b0101,
    AF6 = 0b0110,
    AF7 = 0b0111,
} GPIOx_AFSEL;

typedef struct GPin
{
    GPIOx gpio;
    uint8_t pin;
} GPin;

uint32_t
create_bitmask( uint32_t shift,
                uint32_t size_in_bits,
                uint32_t value );

volatile uint8_t *
get_GPIOx_base_addr( GPIOx gpio );

volatile uint32_t *
get_GPIOx_reg_addr( GPIOx gpio, 
                    uint32_t offset );

void 
set_GPIOx_MODER( GPIOx gpio, 
                 uint32_t pin, 
                 GPIOx_MODER moder );

void 
set_GPIOx_OTYPER( GPIOx gpio, 
                  uint32_t pin, 
                  GPIOx_OTYPER typer );

void
set_GPIOx_OSPEEDR( GPIOx gpio,
                   uint32_t pin,
                   GPIOx_OSPEEDR speed );

void 
set_GPIOx_PUPDR( GPIOx gpio, 
                 uint32_t pin, 
                 GPIOx_PUPDR pupdr );

void
set_GPIOx_AFRL( GPIOx gpio,
                uint32_t pin,
                GPIOx_AFSEL afsel );

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
set_range_GPIOx_OSPEEDR( GPIOx gpio,
                         uint32_t pin_start, uint32_t pin_end,
                         GPIOx_OSPEEDR speed );

void 
set_range_GPIOx_PUPDR( GPIOx gpio, 
                       uint32_t pin_start, uint32_t pin_end, 
                       GPIOx_PUPDR pupdr );

void
set_range_GPIOx_AFRL( GPIOx gpio,
                      uint32_t pin_start, uint32_t pin_end,
                      GPIOx_AFSEL afsel );

void
enable_HSE_clock();

void
enable_PLL_clock();

void
select_SYSCLK_source( SYSCLK_SOURCE source );

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

void
select_I2Cx_source( I2Cx i2c, 
                    I2Cx_SOURCE source );

void
enable_APB_peripheral_clock( PERIPHERAL peripheral );

void
set_I2Cx_analog_filter( I2Cx i2c, 
                        uint32_t value );

void
enable_I2Cx_analog_filter( I2Cx i2c );

void
disable_I2Cx_analog_filter( I2Cx i2c );

void
set_I2Cx_digital_filter( I2Cx i2c, 
                         uint32_t value );

void
disable_I2Cx_digital_filter( I2Cx i2c );

void
set_I2Cx_peripheral( I2Cx i2c, 
                     uint32_t value );

void
enable_I2Cx_peripheral( I2Cx i2c );

void
disable_I2Cx_peripheral( I2Cx i2c );

void
set_I2Cx_400khz( I2Cx i2c );

void
set_I2Cx_clock_stretching( I2Cx i2c, 
                           uint32_t value );

void
enable_I2Cx_clock_stretching( I2Cx i2c );

void
disable_I2Cx_clock_stretching( I2Cx i2c );

void
set_I2Cx_addressing_mode( I2Cx i2c, 
                          I2Cx_ADDR_MODE mode );

void
set_I2Cx_own_address( I2Cx i2c, 
                      uint32_t value );

void
enable_I2Cx_own_address( I2Cx i2c );

void
disable_I2Cx_own_address( I2Cx i2c );

void
set_I2Cx_own_address_mode( I2Cx i2c, 
                           I2Cx_ADDR_MODE mode );

void
set_I2Cx_mode_I2C( I2Cx i2c );

void
start_I2Cx_transfer( I2Cx i2c, 
                     I2Cx_ADDR_MODE mode,
                     uint32_t slave_addr,
                     uint8_t nbytes );

void
wait_I2Cx_transmition( I2Cx i2c );

void
end_I2Cx_transfer( I2Cx i2c );

void
transmit_I2Cx( I2Cx i2c, 
               uint8_t data );
