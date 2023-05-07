#include "lib.h"

static uint32_t
create_bitmask( uint32_t shift,
                uint32_t value )
{
    return (value << shift);
}

static void 
zero_reg_bits( volatile uint32_t *reg,
               uint32_t shift,
               uint32_t number_of_bits )
{
    for ( uint32_t i = 0; i < number_of_bits; i++ )
    {
        *reg &= ~(1U << (shift + i));
    }
}

static void 
set_reg_bits( volatile uint32_t *reg,
              uint32_t shift,
              uint32_t number_of_bits,
              uint32_t value )
{
    zero_reg_bits( reg, shift, number_of_bits );

    *reg |= (value << shift);
}

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
enable_HSE_clock()
{
    const uint32_t HSEON_SHIFT = 16U;
    const uint32_t HSEON_SIZE_IN_BITS = 1U;
    const uint32_t HSE_ON_VALUE = 1U;

    const uint32_t HSERDY_SHIFT = 17U;
    const uint32_t HSERDY_SIZE_IN_BITS = 1U;
    const uint32_t HSE_READY_VALUE = 1U;
    const uint32_t HSE_READY_BITMASK = 
        create_bitmask( HSERDY_SHIFT, HSE_READY_VALUE );

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
enable_PLL_clock()
{
    const uint32_t PLLON_SHIFT = 24U;
    const uint32_t PLLON_SIZE_IN_BITS = 1U;
    const uint32_t PLL_ON_VALUE = 1U;

    const uint32_t PLLRDY_SHIFT = 24U;
    const uint32_t PLLRDY_SIZE_IN_BITS = 1U;
    const uint32_t PLL_LOCKED_VALUE = 1U;
    const uint32_t PLL_LOCKED_BITMASK =
        create_bitmask( PLLRDY_SHIFT, PLL_LOCKED_VALUE );

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
        create_bitmask( SWS_value, SWS_SHIFT );

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

/**
 * Only RCC_APB1ENR is supported.
 */
void
enable_APB_peripheral_clock( PERIPHERAL peripheral )
{
    const uint32_t PERIPHERAL_SIZE_IN_BITS = 1U;
    const uint32_t PERIPHERAL_ENABLED_VALUE = 1U;

    set_reg_bits(
        get_RCC_reg_addr( RCC_APB1ENR_OFFSET ),
        (uint32_t)peripheral,
        PERIPHERAL_SIZE_IN_BITS,
        PERIPHERAL_ENABLED_VALUE
    );
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
