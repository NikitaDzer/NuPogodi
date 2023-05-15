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

#define USART1_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x40013800U

#define USARTx_CR1_OFFSET  (0x00U)
#define USARTx_CR2_OFFSET  (0x04U)
#define USARTx_CR3_OFFSET  (0x08U)
#define USARTx_BRR_OFFSET  (0x0CU)
#define USARTx_GTPR_OFFSET (0x10U)
#define USARTx_RTOR_OFFSET (0X14U)
#define USARTx_RQR_OFFSET  (0x18U)
#define USARTx_ISR_OFFSET  (0x1CU)
#define USARTx_ICR_OFFSET  (0x20U)
#define USARTx_RDR_OFFSET  (0x24U)
#define USARTx_TDR_OFFSET  (0x28U)

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
#define GPIOx_AFRH_OFFSET    (0x24U)


#define PERIPH_BASE_ADDR (volatile uint8_t*)(uintptr_t)0x40000000U
#define APBPERIPH_BASE_ADDR PERIPH_BASE_ADDR
#define TIM_BASE_ADDR APBPERIPH_BASE_ADDR

#define TIM2_PSC_ADDR (volatile uint8_t*)(uintptr_t)0x40000028U
#define TIM2_ARR_ADDR (volatile uint8_t*)(uintptr_t)0x4000002CU
#define TIM2_CR1_ADDR (volatile uint8_t*)(uintptr_t)0x40000000U
#define TIM2_CNT_ADDR (volatile uint8_t*)(uintptr_t)0x40000024U
#define TIM2_CCR1_ADDR (volatile uint8_t*)(uintptr_t)0x40000034U
#define TIM2_CCER_ADDR (volatile uint8_t*)(uintptr_t)0x40000020U
#define TIM2_CCMR1_ADDR (volatile uint8_t*)(uintptr_t)0x40000018U

#define TIM3_PSC_ADDR (volatile uint8_t*)(uintptr_t)0x40000428U
#define TIM3_ARR_ADDR (volatile uint8_t*)(uintptr_t)0x4000042CU
#define TIM3_CR1_ADDR (volatile uint8_t*)(uintptr_t)0x40000400U
#define TIM3_CNT_ADDR (volatile uint8_t*)(uintptr_t)0x40000424U

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

    SYSCFGCOMPEN = 0,
    USART6EN     = 5,
    USART7EN     = 6,
    USART8EN     = 7,
    ADCEN        = 9,
    TIM1EN       = 11,
    SPI1EN       = 12,
    USART1EN     = 14,
    TIM15EN      = 16,
    TIM16EN      = 17,
    TIM17EN      = 18,
    DBGMCUEN     = 22,
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

typedef enum USARTx
{
    USART1,
} USARTx;

typedef enum USARTx_SOURCE
{
    USARTx_SOURCE_PCLK   = 0b00,
    USARTx_SOURCE_SYSCLK = 0b01,
    USARTx_SOURCE_LSE    = 0b10,
    USARTx_SOURCE_HSI    = 0b11,
} USARTx_SOURCE;

typedef enum USARTx_OVERSAMPLAING
{
    USARTx_OVERSAMPLING_BY_16 = 0b0,
    USARTx_OVERSAMPLING_BY_8  = 0b1,
} USARTx_OVERSAMPLING;

typedef enum USARTx_WORD_LENGTH
{
    USARTx_WORD_LENGTH_8BIT,
    USARTx_WORD_LENGTH_9BIT,
    USARTx_WORD_LENGTH_7BIT,
} USARTx_WORD_LENGTH;

typedef enum USARTx_ENDIANNESS
{
    USARTx_ENDIANNESS_LSB = 0b0,
    USARTx_ENDIANNESS_MSB = 0b1,
} USARTx_ENDIANNESS;


typedef enum USARTx_STOP_BIT
{
    USARTx_STOP_1BIT   = 0b00,
    USARTx_STOP_0_5BIT = 0b01,
    USARTx_STOP_2BIT   = 0b10,
    USARTx_STOP_1_5BIT = 0b11,
} USARTx_STOP_BIT;

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
    HCLK_DIVIDED_BY_2  = 0b001,
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

typedef enum POLARITY
{
    POLARITY_LOW = 0b1,
    POLARITY_HIGH = 0b0
} POLARITY;

typedef enum COUNTER_MODE
{
    COUNTER_MODE_UP = 0b0,
    COUNTER_MODE_DOWN = 0b1
} COUNTER_MODE;

typedef enum CAPTURE_COMPARE_MODE
{
    CAPTURE_COMPARE_FROZEN          = 0b000,
    CAPTURE_COMPARE_ACTIVE          = 0b001,
    CAPTURE_COMPARE_iNACTIVE        = 0b010,
    CAPTURE_COMPARE_TOGGLE          = 0b011,
    CAPTURE_COMPARE_FORCED_INACTIVE = 0b100,
    CAPTURE_COMPARE_FORCED_ACTIVE   = 0b101,
    CAPTURE_COMPARE_PWM1            = 0b110,
    CAPTURE_COMPARE_PWM2            = 0b111
} CAPTURE_COMPARE_MODE;

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
set_GPIOx_AFRH( GPIOx gpio,
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
set_range_GPIOx_AFRH( GPIOx gpio,
                      uint32_t pin_start, uint32_t pin_end,
                      GPIOx_AFSEL afsel );


void
set_TIM3_prescaler( uint16_t prescaler );

void
set_TIM3_auto_reload( uint16_t auto_reload );

void
set_TIM3_counter_mode( COUNTER_MODE mode );

void
enable_TIM3_counter();

uint16_t
get_TIM3_counter();

void
set_TIM2_prescaler( uint16_t prescaler );

void
set_TIM2_auto_reload( uint32_t auto_reload );

void
set_TIM2_compare_ch1( uint32_t compare_value );

void
enable_TIM2_ch1();

void
set_TIM2_polarity( POLARITY polarity );

void
set_TIM2_ch1_mode( CAPTURE_COMPARE_MODE mode );

void
set_TIM2_counter_mode( COUNTER_MODE mode );

void
enable_TIM2_counter();

void
enable_TIM2_ch1();

void
enable_HSE_clock();

void
enable_HSI_clock();

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
enable_APB1_peripheral_clock( PERIPHERAL peripheral );

void
enable_APB2_peripheral_clock( PERIPHERAL peripheral );

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

void
select_USART1_source( USARTx_SOURCE source );

void
set_USARTx_peripheral( USARTx usart, uint32_t value );

void
enable_USARTx_peripheral( USARTx usart );

void
disable_USARTx_peripheral( USARTx usart );

void
set_USARTx_receiver( USARTx usart, uint32_t value );

void
enable_USARTx_receiver( USARTx usart );

void
disable_USARTx_receiver( USARTx usart );

void
set_USARTx_transmitter( USARTx usart, uint32_t value );

void
enable_USARTx_transmitter( USARTx usart );

void
disable_USARTx_transmitter( USARTx usart );

void
set_USARTx_parity( USARTx usart, uint32_t value );

void
enable_USARTx_parity( USARTx usart );

void
disable_USARTx_parity( USARTx usart );

void
set_USARTx_oversampling( USARTx usart,
                         USARTx_OVERSAMPLING oversampling );

void
set_USARTx_word_length( USARTx usart,
                        USARTx_WORD_LENGTH length );

void
set_USARTx_endianness( USARTx usart,
                       USARTx_ENDIANNESS endianness );

void
set_USARTx_stop_bit( USARTx usart,
                     USARTx_STOP_BIT stop_bit );

void
set_USARTx_baudrate( USARTx usart,
                     uint32_t baudrate,
                     uint32_t frequency );

void
wait_USARTx_transmit_enabled( USARTx usart );

void
wait_USARTx_receive_enabled( USARTx usart );
