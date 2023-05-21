#include <limits.h>
#include "../include/lib.h"

#define DEFINE_FIELD( field_name, shift, size_in_bits )         \
    const uint32_t field_name##_SHIFT = (shift);                \
    const uint32_t field_name##_SIZE_IN_BITS = (size_in_bits);

#define SET_REG_FIELD( reg_addr, field_name, value )    \
    set_reg_bits(                                       \
        (reg_addr),                                     \
        field_name##_SHIFT,                             \
        field_name##_SIZE_IN_BITS,                      \
        (value)                                         \
    )

#define WAIT_REG_FIELD( reg_addr, field_name, bitmask ) \
    while ( get_reg_bits(                               \
                (reg_addr),                             \
                field_name##_SHIFT,                     \
                field_name##_SIZE_IN_BITS               \
            ) != (bitmask) )

static uint32_t
get_reg_bits( const volatile uint32_t *reg,
              uint32_t shift,
              uint32_t number_of_bits )
{
    uint32_t result = 0;

    for ( uint32_t i = 0; i < number_of_bits; i++ )
    {
        result |= *reg & (1U << (shift + i));
    }

    return result;
}

static uint32_t
get_reg_bit( uint32_t reg,
             uint32_t shift )
{
    return reg & (1U << shift);
}

static void
set_reg_mask( volatile uint32_t *reg_addr,
              uint32_t access_mask,
              uint32_t value_mask )
{
    uint32_t reg = get_reg_bits( reg_addr, 0, 32 );

    *reg_addr = (reg & ~access_mask) | (value_mask & access_mask);
}

static void
set_reg_bits( volatile uint32_t *reg_addr,
              uint32_t shift,
              uint32_t size_in_bits,
              uint32_t value )
{
    uint32_t access_mask
        = create_bitmask( shift, size_in_bits, UINT32_MAX );
    uint32_t value_mask
        = create_bitmask( shift, size_in_bits, value );

    set_reg_mask( reg_addr, access_mask, value_mask );
}

static volatile uint32_t *
get_RCC_reg_addr( uint32_t offset )
{
    return (volatile uint32_t *)( RCC_BASE_ADDR + offset );
}

static uint32_t
get_RCC_reg( uint32_t offset )
{
    return *get_RCC_reg_addr( offset );
}

static volatile uint8_t *
get_I2Cx_base_addr( I2Cx i2c )
{
    switch ( i2c )
    {
        case I2C1:
            return I2C1_BASE_ADDR;

        case I2C2:
            return I2C2_BASE_ADDR;

        default:
            // Unreachable.
            return 0U;
    }
}

static volatile uint32_t *
get_I2Cx_reg_addr( I2Cx i2c, uint32_t offset )
{
    return (volatile uint32_t *)( get_I2Cx_base_addr( i2c ) + offset );
}

static uint32_t
get_I2Cx_reg( I2Cx i2c, uint32_t offset )
{
    return *get_I2Cx_reg_addr( i2c, offset );
}

static volatile uint8_t *
get_USARTx_base_addr( USARTx usart )
{
    switch ( usart )
    {
        case USART1:
            return USART1_BASE_ADDR;

        default:
            // Unreachable.
            return 0U;
    }
}

static volatile uint32_t *
get_USARTx_reg_addr( USARTx usart, uint32_t offset )
{
    return (volatile uint32_t *)( get_USARTx_base_addr( usart) + offset );
}

static uint32_t
get_USARTx_reg( USARTx usart, uint32_t offset )
{
    return *get_USARTx_reg_addr( usart, offset );
}

uint32_t
create_bitmask( uint32_t shift,
                uint32_t size_in_bits,
                uint32_t value )
{
    const uint32_t UINT32_BITS = sizeof( uint32_t ) * CHAR_BIT;

    uint32_t clear_mask = UINT32_MAX >> ( UINT32_BITS - size_in_bits );
    uint32_t prepared_value = value & clear_mask;

    return (prepared_value << shift);
}

volatile uint8_t *
get_GPIOx_base_addr( GPIOx gpio )
{
    switch ( gpio )
    {
        case GPIOA:
            return GPIOA_BASE_ADDR;

        case GPIOB:
            return GPIOB_BASE_ADDR;

        case GPIOC:
            return GPIOC_BASE_ADDR;

        case GPIOD:
            return GPIOD_BASE_ADDR;

        case GPIOE:
            return GPIOE_BASE_ADDR;

        case GPIOF:
            return GPIOF_BASE_ADDR;

        default:
            // Unreachable.
            return 0U;
    }
}

volatile uint32_t *
get_GPIOx_reg_addr( GPIOx gpio,
                    uint32_t offset )
{
    return (volatile uint32_t *)(get_GPIOx_base_addr( gpio ) + offset);
}

uint32_t
get_GPIOx_reg( GPIOx gpio,
               uint32_t offset )
{
    return *get_GPIOx_reg_addr( gpio, offset );
}

void
set_GPIOx_MODER( GPIOx gpio,
                 uint32_t pin,
                 GPIOx_MODER moder)
{
    const uint32_t MODER_SIZE_IN_BITS = 2U;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_MODER_OFFSET ),
        pin * MODER_SIZE_IN_BITS,
        MODER_SIZE_IN_BITS,
        (uint32_t)moder
    );
}

void
set_GPIOx_OTYPER( GPIOx gpio,
                  uint32_t pin,
                  GPIOx_OTYPER typer )
{
    const uint32_t OTYPER_SIZE_IN_BITS = 1U;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_OTYPER_OFFSET ),
        pin * OTYPER_SIZE_IN_BITS,
        OTYPER_SIZE_IN_BITS,
        (uint32_t)typer
    );
}

void
set_GPIOx_OSPEEDR( GPIOx gpio,
                   uint32_t pin,
                   GPIOx_OSPEEDR speed )
{
    const uint32_t OSPEEDR_SIZE_IN_BITS = 2U;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_OSPEEDR_OFFSET ),
        pin * OSPEEDR_SIZE_IN_BITS,
        OSPEEDR_SIZE_IN_BITS,
        (uint32_t)speed
    );
}

void
set_GPIOx_PUPDR( GPIOx gpio,
                 uint32_t pin,
                 GPIOx_PUPDR pupdr )
{
    const uint32_t PUPDR_SIZE_IN_BITS = 2U;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_PUPDR_OFFSET ),
        pin * PUPDR_SIZE_IN_BITS,
        PUPDR_SIZE_IN_BITS,
        (uint32_t)pupdr
    );
}

void
set_GPIOx_AFRL( GPIOx gpio,
                uint32_t pin,
                GPIOx_AFSEL afsel )
{
    const uint32_t AFSEL_SIZE_IN_BITS = 4U;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_AFRL_OFFSET ),
        pin * AFSEL_SIZE_IN_BITS,
        AFSEL_SIZE_IN_BITS,
        (uint32_t)afsel
    );
}

void
set_GPIOx_AFRH( GPIOx gpio,
                uint32_t pin,
                GPIOx_AFSEL afsel )
{
    const uint32_t AFSEL_SIZE_IN_BITS = 4U;
    const uint32_t PIN_SHIFT = 8U;

    uint32_t pin_position = pin - PIN_SHIFT;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_AFRH_OFFSET ),
        pin_position * AFSEL_SIZE_IN_BITS,
        AFSEL_SIZE_IN_BITS,
        (uint32_t)afsel
    );
}

void
set_GPIOx_ODR_whole( GPIOx gpio,
                     uint32_t value )
{
    *get_GPIOx_reg_addr( gpio, GPIOx_ODR_OFFSET ) = value;
}

void
set_GPIOx_ODR( GPIOx gpio,
               uint32_t pin,
               uint32_t value )
{
    const uint32_t ODR_SIZE_IN_BITS = 1U;

    set_reg_bits(
        get_GPIOx_reg_addr( gpio, GPIOx_ODR_OFFSET ),
        pin * ODR_SIZE_IN_BITS,
        ODR_SIZE_IN_BITS,
        (uint32_t)value
    );
}

uint32_t
get_GPIOx_ODR_whole( GPIOx gpio )
{
    return get_GPIOx_reg( gpio, GPIOx_ODR_OFFSET );
}

uint32_t
get_GPIOx_ODR( GPIOx gpio,
               uint32_t pin )
{
    return get_GPIOx_ODR_whole( gpio ) & ( 1U << pin );
}

uint32_t
get_GPIOx_IDR_whole( GPIOx gpio )
{
    return get_GPIOx_reg( gpio, GPIOx_IDR_OFFSET );
}

uint32_t
get_GPIOx_IDR( GPIOx gpio,
               uint32_t pin )
{
    return get_GPIOx_IDR_whole( gpio ) & ( 1U << pin );
}

void
set_range_GPIOx_MODER( GPIOx gpio,
                       uint32_t pin_start,
                       uint32_t pin_end,
                       GPIOx_MODER moder )
{
    for ( uint32_t pin = pin_start; pin <= pin_end; pin++ )
    {
        set_GPIOx_MODER( gpio, pin, moder );
    }
}

void
set_range_GPIOx_OTYPER( GPIOx gpio,
                        uint32_t pin_start,
                        uint32_t pin_end,
                        GPIOx_OTYPER typer )
{
    for ( uint32_t pin = pin_start; pin <= pin_end; pin++ )
    {
        set_GPIOx_OTYPER( gpio, pin, typer );
    }
}

void
set_range_GPIOx_OSPEEDR( GPIOx gpio,
                         uint32_t pin_start,
                         uint32_t pin_end,
                         GPIOx_OSPEEDR speed )
{
    for ( uint32_t pin = pin_start; pin <= pin_end; pin++ )
    {
        set_GPIOx_OSPEEDR( gpio, pin, speed );
    }
}

void
set_range_GPIOx_PUPDR( GPIOx gpio,
                       uint32_t pin_start,
                       uint32_t pin_end,
                       GPIOx_PUPDR pupdr )
{
    for ( uint32_t pin = pin_start; pin <= pin_end; pin++ )
    {
        set_GPIOx_PUPDR( gpio, pin, pupdr );
    }
}

void
set_range_GPIOx_AFRL( GPIOx gpio,
                      uint32_t pin_start,
                      uint32_t pin_end,
                      GPIOx_AFSEL afsel )
{
    for ( uint32_t pin = pin_start; pin <= pin_end; pin++ )
    {
        set_GPIOx_AFRL( gpio, pin, afsel );
    }
}

void
set_range_GPIOx_AFRH( GPIOx gpio,
                      uint32_t pin_start,
                      uint32_t pin_end,
                      GPIOx_AFSEL afsel )
{
    for ( uint32_t pin = pin_start; pin <= pin_end; pin++ )
    {
        set_GPIOx_AFRH( gpio, pin, afsel );
    }
}

void
enable_HSE_clock()
{
    const uint32_t HSEON_SHIFT = 16U;
    const uint32_t HSEON_SIZE_IN_BITS = 1U;
    const uint32_t HSE_ON_VALUE = 1U;

    const uint32_t HSERDY_SHIFT = 17U;
    const uint32_t HSERDY_SIZE_IN_BITS = 1U;
    const uint32_t HSE_READY_VALUE = 1U;
    const uint32_t HSE_READY_BITMASK =
        create_bitmask(
            HSERDY_SHIFT,
            HSERDY_SIZE_IN_BITS,
            HSE_READY_VALUE
        );

    set_reg_bits(
            get_RCC_reg_addr( RCC_CR_OFFSET ),
            HSEON_SHIFT,
            HSEON_SIZE_IN_BITS,
            HSE_ON_VALUE
    );

    while ( get_reg_bits(
                get_RCC_reg_addr( RCC_CR_OFFSET ),
                HSERDY_SHIFT,
                HSERDY_SIZE_IN_BITS
            ) != HSE_READY_BITMASK )
    {
    }
}

void
enable_HSI_clock()
{
    const uint32_t HSION_SHIFT = 0U;
    const uint32_t HSION_SIZE_IN_BITS = 1U;
    const uint32_t HSI_ON_VALUE = 1U;

    const uint32_t HSIRDY_SHIFT = 1U;
    const uint32_t HSIRDY_SIZE_IN_BITS = 1U;
    const uint32_t HSI_READY_VALUE = 1U;
    const uint32_t HSI_READY_BITMASK =
	create_bitmask(
	    HSIRDY_SHIFT,
	    HSION_SIZE_IN_BITS,
	    HSI_ON_VALUE
        );
    set_reg_bits(
	get_RCC_reg_addr( RCC_CR_OFFSET ),
	HSION_SHIFT,
	HSION_SIZE_IN_BITS,
	HSI_ON_VALUE
    );

    while ( get_reg_bits(
		get_RCC_reg_addr( RCC_CR_OFFSET ),
		HSIRDY_SHIFT,
		HSIRDY_SIZE_IN_BITS
	    ) != HSI_READY_BITMASK )
    {
    }
}

void
enable_PLL_clock()
{
    const uint32_t PLLON_SHIFT = 24U;
    const uint32_t PLLON_SIZE_IN_BITS = 1U;
    const uint32_t PLL_ON_VALUE = 1U;

    const uint32_t PLLRDY_SHIFT = 24U;
    const uint32_t PLLRDY_SIZE_IN_BITS = 1U;
    const uint32_t PLL_LOCKED_VALUE = 1U;
    const uint32_t PLL_LOCKED_BITMASK =
        create_bitmask(
            PLLRDY_SHIFT,
            PLLRDY_SIZE_IN_BITS,
            PLL_LOCKED_VALUE
        );

    set_reg_bits(
            get_RCC_reg_addr( RCC_CR_OFFSET ),
            PLLON_SHIFT,
            PLLON_SIZE_IN_BITS,
            PLL_ON_VALUE
    );

    while ( get_reg_bits(
                get_RCC_reg_addr( RCC_CR_OFFSET ),
                PLLRDY_SHIFT,
                PLLRDY_SIZE_IN_BITS
            ) != PLL_LOCKED_BITMASK )
    {
    }
}

void
select_SYSCLK_source( SYSCLK_SOURCE source )
{
    const uint32_t SW_SHIFT = 0U;
    const uint32_t SW_SIZE_IN_BITS = 2U;

    const uint32_t SW_HSI_VALUE   = 0b00U;
    const uint32_t SW_HSE_VALUE   = 0b01U;
    const uint32_t SW_PLL_VALUE   = 0b10U;
    const uint32_t SW_HSI48_VALUE = 0b11U;

    const uint32_t SWS_SHIFT = 2U;
    const uint32_t SWS_SIZE_IN_BITS = 2U;

    const uint32_t SWS_HSI_VALUE   = 0b00U;
    const uint32_t SWS_HSE_VALUE   = 0b01U;
    const uint32_t SWS_PLL_VALUE   = 0b10U;
    const uint32_t SWS_HSI48_VALUE = 0b11U;

    uint32_t SW_value  = 0;
    uint32_t SWS_value = 0;

    switch ( source )
    {
        case SYSCLK_SOURCE_HSI:
            SW_value  = SW_HSI_VALUE;
            SWS_value = SWS_HSI_VALUE;
            break;

        case SYSCLK_SOURCE_HSE:
            SW_value  = SW_HSE_VALUE;
            SWS_value = SWS_HSE_VALUE;
            break;

        case SYSCLK_SOURCE_PLL:
            SW_value  = SW_PLL_VALUE;
            SWS_value = SWS_PLL_VALUE;
            break;

        case SYSCLK_SOURCE_HSI48:
            SW_value  = SW_HSI48_VALUE;
            SWS_value = SWS_HSI48_VALUE;
            break;

        default:
            // Unreachable.
            return;
    }

    uint32_t SWS_bitmask =
        create_bitmask( SWS_value, SWS_SIZE_IN_BITS, SWS_SHIFT );

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR_OFFSET ),
        SW_SHIFT,
        SW_SIZE_IN_BITS,
        SW_value
    );

    while ( get_reg_bits(
                get_RCC_reg_addr( RCC_CFGR_OFFSET ),
                SWS_SHIFT,
                SWS_SIZE_IN_BITS
            ) != SWS_bitmask )
    {
    }
}

void
set_AHB_prescaler( HCLK prescaler )
{
    const uint32_t HRPE_SHIFT = 4U;
    const uint32_t HRPE_SIZE_IN_BITS = 4U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR_OFFSET ),
        HRPE_SHIFT,
        HRPE_SIZE_IN_BITS,
        (uint32_t)prescaler
    );
}

void
set_PLL_prescaler( PREDIV prescaler )
{
    const uint32_t PREDIV_SHIFT = 0U;
    const uint32_t PREDIV_SIZE_IN_BITS = 4U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR2_OFFSET ),
        PREDIV_SHIFT,
        PREDIV_SIZE_IN_BITS,
        (uint32_t)prescaler
    );
}

void
set_PLL_source( PLLSRC source )
{
    const uint32_t PLLSRC_SHIFT = 15U;
    const uint32_t PLLSRC_SIZE_IN_BITS = 2U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR_OFFSET ),
        PLLSRC_SHIFT,
        PLLSRC_SIZE_IN_BITS,
        (uint32_t)source
    );
}

void
set_PLL_multiplier( PLLMUL factor )
{
    const uint32_t PLLMUL_SHIFT = 18U;
    const uint32_t PLLMUL_SIZE_IN_BITS = 4U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR_OFFSET ),
        PLLMUL_SHIFT,
        PLLMUL_SIZE_IN_BITS,
        (uint32_t)factor
    );
}

void
set_APB_prescaler( PCLK prescaler )
{
    const uint32_t PCLK_PRESCALER_SHIFT = 8U;
    const uint32_t PCLK_PRESCALER_SIZE_IN_BITS = 3U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR_OFFSET ),
        PCLK_PRESCALER_SHIFT,
        PCLK_PRESCALER_SIZE_IN_BITS,
        (uint32_t)prescaler
    );
}

void
enable_GPIOx_clock( GPIOx gpio )
{
    const uint32_t IOPAEN_SHIFT = 17U;
    const uint32_t IOPBEN_SHIFT = 18U;
    const uint32_t IOPCEN_SHIFT = 19U;
    const uint32_t IOPDEN_SHIFT = 20U;
    const uint32_t IOPEEN_SHIFT = 21U;
    const uint32_t IOPFEN_SHIFT = 22U;

    const uint32_t IOPEN_SIZE_IN_BITS = 1U;
    const uint32_t IOPEN_ENABLED_VALUE = 0b1U;

    uint32_t IOPEN_shift = 0U;

    switch ( gpio )
    {
        case GPIOA:
            IOPEN_shift = IOPAEN_SHIFT;
            break;

        case GPIOB:
            IOPEN_shift = IOPBEN_SHIFT;
            break;

        case GPIOC:
            IOPEN_shift = IOPCEN_SHIFT;
            break;

        case GPIOD:
            IOPEN_shift = IOPDEN_SHIFT;
            break;

        case GPIOE:
            IOPEN_shift = IOPEEN_SHIFT;
            break;

        case GPIOF:
            IOPEN_shift = IOPFEN_SHIFT;
            break;

        default:
            // Unreachable.
            return;

    }

    set_reg_bits(
        get_RCC_reg_addr( RCC_AHBENR_OFFSET ),
        IOPEN_shift,
        IOPEN_SIZE_IN_BITS,
        IOPEN_ENABLED_VALUE
    );
}

static void
enable_APBx_peripheral_clock( PERIPHERAL peripheral,
                              uint32_t APBx_offset )
{
    const uint32_t PERIPHERAL_SIZE_IN_BITS = 1U;
    const uint32_t PERIPHERAL_ENABLED_VALUE = 1U;

    set_reg_bits(
        get_RCC_reg_addr( APBx_offset ),
        (uint32_t)peripheral,
        PERIPHERAL_SIZE_IN_BITS,
        PERIPHERAL_ENABLED_VALUE
    );
}


void
set_TIM3_prescaler( uint16_t prescaler )
{
    *(volatile uint16_t*)TIM3_PSC_ADDR = prescaler;
}

void
set_TIM3_auto_reload( uint16_t auto_reload )
{
    *(volatile uint16_t*)TIM3_ARR_ADDR = auto_reload;
}

void
set_TIM3_counter_mode( COUNTER_MODE mode )
{
    DEFINE_FIELD( DIR, 4U, 1U );
    SET_REG_FIELD( (volatile uint32_t*)TIM3_CR1_ADDR,
		   DIR,
		   mode );
}


void
enable_TIM3_counter()
{
    DEFINE_FIELD( CR1_CEN, 0U, 1U );
    SET_REG_FIELD( (volatile uint32_t*)TIM3_CR1_ADDR,
		   CR1_CEN,
		   1U );
}

uint16_t
get_TIM3_counter()
{
    return *(uint16_t*)TIM3_CNT_ADDR;
}

void
set_TIM2_prescaler( uint16_t prescaler )
{
    *(volatile uint16_t*)TIM2_PSC_ADDR = prescaler;
}

void
set_TIM2_auto_reload( uint32_t auto_reload )
{
    *(volatile uint32_t*)TIM2_ARR_ADDR = auto_reload;
}

void
set_TIM2_compare_ch1( uint32_t compare_value )
{
    *(volatile uint32_t*)TIM2_CCR1_ADDR = compare_value;
}

void
enable_TIM2_ch1()
{
    DEFINE_FIELD( CC1E, 0U, 1U );
    SET_REG_FIELD( (volatile uint32_t*)TIM2_CCER_ADDR,
		   CC1E,
		   1U );
}

void
set_TIM2_polarity( POLARITY polarity )
{
    DEFINE_FIELD( CC1P, 1U, 1U );

    SET_REG_FIELD( (volatile uint32_t*)TIM2_CCER_ADDR,
		   CC1P,
		   polarity );

}

void
set_TIM2_ch1_mode( CAPTURE_COMPARE_MODE mode )
{
    DEFINE_FIELD( OC1M, 4U, 3U );
    SET_REG_FIELD( (volatile uint32_t*)TIM2_CCMR1_ADDR,
		   OC1M,
		   mode );
}

void
set_TIM2_counter_mode( COUNTER_MODE mode )
{
    DEFINE_FIELD( DIR, 4U, 1U );
    SET_REG_FIELD( (volatile uint32_t*)TIM2_CR1_ADDR,
		   DIR,
		   mode );
}

void
enable_TIM2_counter()
{
    DEFINE_FIELD( CR1_CEN, 0U, 1U );
    SET_REG_FIELD( (volatile uint32_t*)TIM2_CR1_ADDR,
		   CR1_CEN,
		   1U );
}

void
enable_APB1_peripheral_clock( PERIPHERAL peripheral )
{
    enable_APBx_peripheral_clock( peripheral, RCC_APB1ENR_OFFSET );
}

void
enable_APB2_peripheral_clock( PERIPHERAL peripheral )
{
    enable_APBx_peripheral_clock( peripheral, RCC_APB2ENR_OFFSET );
}


void
select_I2Cx_source( I2Cx i2c, I2Cx_SOURCE source )
{
    const uint32_t I2Cx_SOURCE_SIZE_IN_BITS = 1U;

    const uint32_t I2C1_SOURCE_SHIFT = 4U;
    const uint32_t I2C2_SOURCE_SHIFT = 5U;

    uint32_t I2Cx_SOURCE_shift = 0;

    switch ( i2c )
    {
        case I2C1:
            I2Cx_SOURCE_shift = I2C1_SOURCE_SHIFT;
            break;

        case I2C2:
            I2Cx_SOURCE_shift = I2C2_SOURCE_SHIFT;
            break;

        default:
            // Unreachable.
            break;
    }

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR3_OFFSET ),
        I2Cx_SOURCE_shift,
        I2Cx_SOURCE_SIZE_IN_BITS,
        (uint32_t)source
    );
}

/**
 * Can be set when I2C is disabled.
 */
void
set_I2Cx_analog_filter( I2Cx i2c, uint32_t value )
{
    const uint32_t ANFOFF_SHIFT = 12U;
    const uint32_t ANFOFF_SIZE_IN_BITS = 1U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        ANFOFF_SHIFT,
        ANFOFF_SIZE_IN_BITS,
        value
    );
}

void
enable_I2Cx_analog_filter( I2Cx i2c )
{
    const uint32_t ANF_ENABLED = 0U;
    set_I2Cx_analog_filter( i2c, ANF_ENABLED );
}

void
disable_I2Cx_analog_filter( I2Cx i2c )
{
    const uint32_t ANF_DISABLED = 1U;
    set_I2Cx_analog_filter( i2c, ANF_DISABLED );
}

void
set_I2Cx_digital_filter( I2Cx i2c, uint32_t value )
{
    const uint32_t DNF_SHIFT = 11U;
    const uint32_t DNF_SIZE_IN_BITS = 4U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        DNF_SHIFT,
        DNF_SIZE_IN_BITS,
        value
    );
}

void
disable_I2Cx_digital_filter( I2Cx i2c )
{
    const uint32_t DNF_DISABLED = 0U;
    set_I2Cx_digital_filter( i2c, DNF_DISABLED );
}

void
set_I2Cx_peripheral( I2Cx i2c, uint32_t value )
{
    const uint32_t PE_SHIFT = 0U;
    const uint32_t PE_SIZE_IN_BITS = 1U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        PE_SHIFT,
        PE_SIZE_IN_BITS,
        value
    );
}

void
enable_I2Cx_peripheral( I2Cx i2c )
{
    const uint32_t PERIPHERAL_ENABLED = 1U;
    set_I2Cx_peripheral( i2c, PERIPHERAL_ENABLED );
}

void
disable_I2Cx_peripheral( I2Cx i2c )
{
    const uint32_t PERIPHERAL_DISABLED = 0U;
    set_I2Cx_peripheral( i2c, PERIPHERAL_DISABLED );
}

/**
 * Assumed, that I2C source frequency is 16MHz.
 */
void
set_I2Cx_400khz( I2Cx i2c )
{
    const uint32_t SCLL_SHIFT = 0U;
    const uint32_t SCLL_SIZE_IN_BITS = 8U;
    const uint32_t SCLL_VALUE = 0x9U;

    const uint32_t SCLH_SHIFT = 8U;
    const uint32_t SCLH_SIZE_IN_BITS = 8U;
    const uint32_t SCLH_VALUE = 0x3U;

    const uint32_t SDADEL_SHIFT = 16U;
    const uint32_t SDADEL_SIZE_IN_BITS = 4U;
    const uint32_t SDADEL_VALUE = 0x2U;

    const uint32_t SCLDEL_SHIFT = 20U;
    const uint32_t SCLDEL_SIZE_IN_BITS = 4U;
    const uint32_t SCLDEL_VALUE = 0x3U;

    const uint32_t PRESC_SHIFT = 28U;
    const uint32_t PRESC_SIZE_IN_BITS = 4U;
    const uint32_t PRESC_VALUE = 1U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_TIMINGR_OFFSET ),
        SCLL_SHIFT,
        SCLL_SIZE_IN_BITS,
        SCLL_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_TIMINGR_OFFSET ),
        SCLH_SHIFT,
        SCLH_SIZE_IN_BITS,
        SCLH_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_TIMINGR_OFFSET ),
        SDADEL_SHIFT,
        SDADEL_SIZE_IN_BITS,
        SDADEL_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_TIMINGR_OFFSET ),
        SCLDEL_SHIFT,
        SCLDEL_SIZE_IN_BITS,
        SCLDEL_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_TIMINGR_OFFSET ),
        PRESC_SHIFT,
        PRESC_SIZE_IN_BITS,
        PRESC_VALUE
    );
}

void
set_I2Cx_clock_stretching( I2Cx i2c, uint32_t value )
{
    const uint32_t NOSTRETCH_SHIFT = 17U;
    const uint32_t NOSTRETCH_SIZE_IN_BITS = 1U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        NOSTRETCH_SHIFT,
        NOSTRETCH_SIZE_IN_BITS,
        value
    );
}

void
enable_I2Cx_clock_stretching( I2Cx i2c )
{
    const uint32_t CLOCK_STRETCHING_ENABLED = 0U;
    set_I2Cx_clock_stretching( i2c, CLOCK_STRETCHING_ENABLED );
}

void
disable_I2Cx_clock_stretching( I2Cx i2c )
{
    const uint32_t CLOCK_STRETCHING_DISABLED = 1U;
    set_I2Cx_clock_stretching( i2c, CLOCK_STRETCHING_DISABLED );
}

void
set_I2Cx_addressing_mode( I2Cx i2c, I2Cx_ADDR_MODE mode )
{
    const uint32_t ADD10_SHIFT = 11U;
    const uint32_t ADD10_SIZE_IN_BITS = 1U;

    const uint32_t ADD10_7BIT_VALUE = 0U;
    const uint32_t ADD10_10BIT_VALUE = 1U;

    uint32_t ADD10_value = 0U;

    switch ( mode )
    {
        case I2Cx_7BIT:
            ADD10_value = ADD10_7BIT_VALUE;
            break;

        case I2Cx_10BIT:
            ADD10_value = ADD10_10BIT_VALUE;
            break;

        default:
            // Unreachable.
            break;
    }

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR2_OFFSET ),
        ADD10_SHIFT,
        ADD10_SIZE_IN_BITS,
        ADD10_value
    );
}

void
set_I2Cx_own_address( I2Cx i2c, uint32_t value )
{
    const uint32_t OA1EN_SHIFT = 15U;
    const uint32_t OA1EN_SIZE_IN_BITS = 1U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_OAR1_OFFSET ),
        OA1EN_SHIFT,
        OA1EN_SIZE_IN_BITS,
        value
    );
}

void
enable_I2Cx_own_address( I2Cx i2c )
{
    const uint32_t OWN_ADDRESS_ENABLED = 1U;
    set_I2Cx_own_address( i2c, OWN_ADDRESS_ENABLED );
}

void
disable_I2Cx_own_address( I2Cx i2c )
{
    const uint32_t OWN_ADDRESS_DISABLED = 0U;
    set_I2Cx_own_address( i2c, OWN_ADDRESS_DISABLED );
}

/**
 * Must be set when OA1EN=0 ( before enable_I2Cx_own_address ).
 */
void
set_I2Cx_own_address_mode( I2Cx i2c, I2Cx_ADDR_MODE mode )
{
    const uint32_t OA1MODE_SHIFT = 10U;
    const uint32_t OA1MODE_SIZE_IN_BITS = 1U;

    const uint32_t OA1MODE_7BIT_VALUE = 0U;
    const uint32_t OA1MODE_10BIT_VALUE = 1U;

    uint32_t OA1MODE_value = 0u;

    switch ( mode )
    {
        case I2Cx_7BIT:
            OA1MODE_value = OA1MODE_7BIT_VALUE;
            break;

        case I2Cx_10BIT:
            OA1MODE_value = OA1MODE_10BIT_VALUE;
            break;

        default:
            // Unreachable.
            break;
    }

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_OAR1_OFFSET ),
        OA1MODE_SHIFT,
        OA1MODE_SIZE_IN_BITS,
        OA1MODE_value
    );
}

void
set_I2Cx_mode_I2C( I2Cx i2c )
{
    const uint32_t TXIE_SHIFT = 1U;
    const uint32_t TXIE_SIZE_IN_BITS = 1U;
    const uint32_t TXIE_ENABLED_VALUE = 1U;

    const uint32_t RXIE_SHIFT = 2U;
    const uint32_t RXIE_SIZE_IN_BITS = 1U;
    const uint32_t RXIE_ENABLED_VALUE = 1U;

    const uint32_t NACKIE_SHIFT = 4U;
    const uint32_t NACKIE_SIZE_IN_BITS = 1U;
    const uint32_t NACKIE_ENABLED_VALUE = 1U;

    const uint32_t TCIE_SHIFT = 6U;
    const uint32_t TCIE_SIZE_IN_BITS = 1U;
    const uint32_t TCIE_ENABLED_VALUE = 1U;

    const uint32_t ERRIE_SHIFT = 7U;
    const uint32_t ERRIE_SIZE_IN_BITS = 1U;
    const uint32_t ERRIE_ENABLED_VALUE = 1U;

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        TXIE_SHIFT,
        TXIE_SIZE_IN_BITS,
        TXIE_ENABLED_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        RXIE_SHIFT,
        RXIE_SIZE_IN_BITS,
        RXIE_ENABLED_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        NACKIE_SHIFT,
        NACKIE_SIZE_IN_BITS,
        NACKIE_ENABLED_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        TCIE_SHIFT,
        TCIE_SIZE_IN_BITS,
        TCIE_ENABLED_VALUE
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR1_OFFSET ),
        ERRIE_SHIFT,
        ERRIE_SIZE_IN_BITS,
        ERRIE_ENABLED_VALUE
    );
}

void
start_I2Cx_transfer( I2Cx i2c,
                     I2Cx_ADDR_MODE mode,
                     uint32_t slave_addr,
                     uint8_t nbytes )
{
    const uint32_t START_SHIFT = 13U;
    const uint32_t START_SIZE_IN_BITS = 1U;
    const uint32_t START_GENERATE_VALUE = 1U;

    const uint32_t AUTOEND_SHIFT = 25U;
    const uint32_t AUTOEND_SIZE_IN_BITS = 1U;
    const uint32_t AUTOEND_AUTOMATIC_STOP_VALUE = 1U;

    const uint32_t NBYTES_SHIFT = 16U;
    const uint32_t NBYTES_SIZE_IN_BITS = 8U;

    const uint32_t SADD7_SHIFT = 1U;
    const uint32_t SADD7_SIZE_IN_BITS = 7U;
    const uint32_t SADD10_SHIFT = 0U;
    const uint32_t SADD10_SIZE_IN_BITS = 10U;

    uint32_t SADD_shift = 0U;
    uint32_t SADD_size_in_bits = 0U;
    uint32_t SADD_value = 0U;

    switch ( mode )
    {
        case I2Cx_7BIT:
            SADD_shift = SADD7_SHIFT;
            SADD_size_in_bits = SADD7_SIZE_IN_BITS;
            SADD_value = slave_addr >> 1U;
            break;

        case I2Cx_10BIT:
            SADD_shift = SADD10_SHIFT;
            SADD_size_in_bits = SADD10_SIZE_IN_BITS;
            SADD_value = slave_addr;
            break;

        default:
            // Unreachable.
            break;
    }

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR2_OFFSET ),
        SADD_shift,
        SADD_size_in_bits,
        SADD_value
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR2_OFFSET ),
        NBYTES_SHIFT,
        NBYTES_SIZE_IN_BITS,
        nbytes
    );

    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR2_OFFSET ),
        AUTOEND_SHIFT,
        AUTOEND_SIZE_IN_BITS,
        AUTOEND_AUTOMATIC_STOP_VALUE
    );

    /**
     * It is crucial to set START after others.
     */
    set_reg_bits(
        get_I2Cx_reg_addr( i2c, I2Cx_CR2_OFFSET ),
        START_SHIFT,
        START_SIZE_IN_BITS,
        START_GENERATE_VALUE
    );
}

void
wait_I2Cx_transmition( I2Cx i2c )
{
    DEFINE_FIELD( TXIS, 1U, 1U );
    const uint32_t TXIS_READY_VALUE = 1U;
    const uint32_t TXIS_READY_BITMASK =
        create_bitmask( TXIS_SHIFT, TXIS_SIZE_IN_BITS, TXIS_READY_VALUE );

    WAIT_REG_FIELD(
        get_I2Cx_reg_addr( i2c, I2Cx_ISR_OFFSET ),
        TXIS,
        TXIS_READY_BITMASK
    );
}

void
end_I2Cx_transfer( I2Cx i2c )
{
    DEFINE_FIELD( TC, 6U, 1U );
    const uint32_t TC_COMPLETE_VALUE = 0U;
    const uint32_t TC_COMPLETE_BITMASK =
        create_bitmask( TC_SHIFT, TC_SIZE_IN_BITS, TC_COMPLETE_VALUE );

    WAIT_REG_FIELD(
        get_I2Cx_reg_addr( i2c, I2Cx_ISR_OFFSET ),
        TC,
        TC_COMPLETE_BITMASK
    );
}

void
transmit_I2Cx( I2Cx i2c, uint8_t data )
{
    wait_I2Cx_transmition( i2c );

    DEFINE_FIELD( TXDATA, 0U, 8U );

    SET_REG_FIELD(
        get_I2Cx_reg_addr( i2c, I2Cx_TXDR_OFFSET ),
        TXDATA,
        data
    );
}

void
select_USART1_source( USARTx_SOURCE source )
{
    const uint32_t USARTx_SOURCE_SIZE_IN_BITS = 2U;
    const uint32_t USART1_SOURCE_SHIFT = 0U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_CFGR3_OFFSET ),
        USART1_SOURCE_SHIFT,
        USARTx_SOURCE_SIZE_IN_BITS,
        (uint32_t)source
    );
}

void
set_USARTx_peripheral( USARTx usart, uint32_t value )
{
    DEFINE_FIELD( UE, 0, 1 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR1_OFFSET ),
        UE,
        value
    );
}

void
enable_USARTx_peripheral( USARTx usart )
{
    const uint32_t USART_ENABLED_VALUE = 1U;
    set_USARTx_peripheral( usart, USART_ENABLED_VALUE );
}

void
disable_USARTx_peripheral( USARTx usart )
{
    const uint32_t USART_DISABLED_VALUE = 0U;
    set_USARTx_peripheral( usart, USART_DISABLED_VALUE );
}

void
set_USARTx_receiver( USARTx usart, uint32_t value )
{
    DEFINE_FIELD( RE, 2, 1 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR1_OFFSET ),
        RE,
        value
    );
}

void
enable_USARTx_receiver( USARTx usart )
{
    const uint32_t RECEIVER_ENABLED_VALUE = 1U;
    set_USARTx_receiver( usart, RECEIVER_ENABLED_VALUE );
}

void
disable_USARTx_receiver( USARTx usart )
{
    const uint32_t RECEIVER_DISABLED_VALUE = 0U;
    set_USARTx_receiver( usart, RECEIVER_DISABLED_VALUE );
}

void
set_USARTx_transmitter( USARTx usart, uint32_t value )
{
    DEFINE_FIELD( TE, 2, 1 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR1_OFFSET ),
        TE,
        value
    );
}

void
enable_USARTx_transmitter( USARTx usart )
{
    const uint32_t TRANSMITTER_ENABLED_VALUE = 1U;
    set_USARTx_transmitter( usart, TRANSMITTER_ENABLED_VALUE );
}

void
disable_USARTx_transmitter( USARTx usart )
{
    const uint32_t TRANSMITTER_DISABLED_VALUE = 0U;
    set_USARTx_transmitter( usart, TRANSMITTER_DISABLED_VALUE );
}

void
set_USARTx_parity( USARTx usart, uint32_t value )
{
    DEFINE_FIELD( PCE, 10, 1 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR1_OFFSET ),
        PCE,
        value
    );
}

void
enable_USARTx_parity( USARTx usart )
{
    const uint32_t PARITY_ENABLED_VALUE = 1U;
    set_USARTx_parity( usart, PARITY_ENABLED_VALUE );
}

void
disable_USARTx_parity( USARTx usart )
{
    const uint32_t PARITY_DISABLED_VALUE = 0U;
    set_USARTx_parity( usart, PARITY_DISABLED_VALUE );
}

void
set_USARTx_oversampling( USARTx usart,
                         USARTx_OVERSAMPLING oversampling )
{
    DEFINE_FIELD( OVER8, 15, 1 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR1_OFFSET ),
        OVER8,
        (uint32_t)oversampling
    );
}

/**
 * TODO: check correctness of fields set.
 * Attention: may work incorrect.
 */
void
set_USARTx_word_length( USARTx usart,
                        USARTx_WORD_LENGTH length )
{
    DEFINE_FIELD( M0, 12, 1 );
    DEFINE_FIELD( M1, 28, 1 );

    const uint32_t ACCESS_MASK = ( 1U << M0_SHIFT ) | ( 1U << M1_SHIFT );

    uint32_t length_mask = 0;

    switch ( length )
    {
        case USARTx_WORD_LENGTH_8BIT:
            length_mask = 0U;
            break;

        case USARTx_WORD_LENGTH_9BIT:
            length_mask =
                create_bitmask( M0_SHIFT, M0_SIZE_IN_BITS, 1U );
            break;

        case USARTx_WORD_LENGTH_7BIT:
            length_mask =
                create_bitmask( M1_SHIFT, M1_SIZE_IN_BITS, 1U );
            break;

        default:
            // Unreachable.
            break;
    }

    set_reg_mask(
        get_USARTx_reg_addr( usart, USARTx_CR1_OFFSET ),
        ACCESS_MASK,
        length_mask
    );
}

void
set_USARTx_endianness( USARTx usart,
                       USARTx_ENDIANNESS endianness )
{
    DEFINE_FIELD( MSBFIRST, 19, 1 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR2_OFFSET ),
        MSBFIRST,
        (uint32_t)endianness
    );
}

void
set_USARTx_stop_bit( USARTx usart,
                     USARTx_STOP_BIT stop_bit )
{
    DEFINE_FIELD( STOP, 12, 2 );
    SET_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_CR2_OFFSET ),
        STOP,
        (uint32_t)stop_bit
    );
}

/**
 * TODO: understand meaning of usartdev.
 * Attention: may work incorrect.
 */
void
set_USARTx_baudrate( USARTx usart,
                     uint32_t baudrate,
                     uint32_t frequency )
{
    uint32_t usartdiv = ( frequency + baudrate / 2 ) / baudrate;
    *get_USARTx_reg_addr( usart, USARTx_BRR_OFFSET ) = usartdiv;
}

void
wait_USARTx_transmit_enabled( USARTx usart )
{
    DEFINE_FIELD( TEACK, 21U, 1U );
    const uint32_t TEACK_READY_VALUE = 1U;
    const uint32_t TEACK_READY_BITMASK =
        create_bitmask( TEACK_SHIFT, TEACK_SIZE_IN_BITS, TEACK_READY_VALUE );

    WAIT_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_ISR_OFFSET ),
        TEACK,
        TEACK_READY_BITMASK
    );
}

void
wait_USARTx_receive_enabled( USARTx usart )
{
    DEFINE_FIELD( REACK, 22U, 1U );
    const uint32_t REACK_READY_VALUE = 1U;
    const uint32_t REACK_READY_BITMASK =
        create_bitmask( REACK_SHIFT, REACK_SIZE_IN_BITS, REACK_READY_VALUE );

    WAIT_REG_FIELD(
        get_USARTx_reg_addr( usart, USARTx_ISR_OFFSET ),
        REACK,
        REACK_READY_BITMASK
    );
}
