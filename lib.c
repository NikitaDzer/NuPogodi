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


volatile uint32_t *
get_RCC_reg_addr( uint32_t offset )
{
    return (volatile uint32_t *)(RCC_BASE_ADDR + offset);
}

uint32_t
get_RCC_reg( uint32_t offset )
{
    return *get_RCC_reg_addr( offset );
}

uint32_t
get_reg_bit( uint32_t reg,
             uint32_t shift )
{
    return reg & (1U << shift);
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
select_SYSCLK_source( SYSCLK source )
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
        case HSI:
            SW_value  = SW_HSI_VALUE;
            SWS_value = SWS_HSI_VALUE;
            break;

        case HSE:
            SW_value  = SW_HSE_VALUE;
            SWS_value = SWS_HSE_VALUE;
            break;

        case PLL:
            SW_value  = SW_PLL_VALUE;
            SWS_value = SWS_PLL_VALUE;
            break;

        case HSI48:
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
