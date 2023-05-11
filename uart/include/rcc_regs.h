#ifndef RCC_REGS_H
#define RCC_REGS_H

#include <stdint.h>
#include "macro.h"

typedef enum PllSrc
{
    PLL_SRC_HSI_2        = 0b00,
    PLL_SRC_HSI_PREDIV   = 0b01,
    PLL_SRC_HSE_PREDIV   = 0b10,
    PLL_SRC_HSI48_PREDIV = 0b11,
} PllSrc;

typedef enum MCClockInput
{
   MC_CLOCK_INPUT_DISABLED     = 0b0000,
   MC_CLOCK_INPUT_HSI14        = 0b0001,
   MC_CLOCK_INPUT_LSI          = 0b0010,
   MC_CLOCK_INPUT_LSE          = 0b0011,
   MC_CLOCK_INPUT_SYSTEM_CLOCK = 0b0100,
   MC_CLOCK_INPUT_HSI8         = 0b0101,
   MC_CLOCK_INPUT_HSE          = 0b0110,
   MC_CLOCK_INPUT_PLL_CLOCK    = 0b0111,
   MC_CLOCK_INPUT_HSI48        = 0b1000
} MCClockInput;

typedef enum SysClockSource
{
    SYSCLOCK_SOURCE_HSI = 0b00,
    SYSCLOCK_SOURCE_HSE = 0b01,
    SYSCLOCK_SOURCE_PLL = 0b10,
    SYSCLOCK_SOURCE_HSI48 = 0b11
} SysClockSource;

typedef enum UsartClockSource
{
    USART_SOURCE_PCLK = 0b00,
    USART_SOURCE_SYSCLK = 0b01,
    USART_SOURCE_LSE = 0b10,
    USART_SOURCE_HSE = 0b11
} UsartClockSource;

typedef enum AhbDivFactor
{
    AHB_DIV_FACTOR_DIV_NO  = 0b0000,
    AHB_DIV_FACTOR_DIV_2   = 0b1000,
    AHB_DIV_FACTOR_DIV_4   = 0b1001,
    AHB_DIV_FACTOR_DIV_8   = 0b1010,
    AHB_DIV_FACTOR_DIV_16  = 0b1011,
    AHB_DIV_FACTOR_DIV_64  = 0b1100,
    AHB_DIV_FACTOR_DIV_128 = 0b1101,
    AHB_DIV_FACTOR_DIV_256 = 0b1110,
    AHB_DIV_FACTOR_DIV_512 = 0b1111
} AhbDivFactor;

typedef enum ApbDivFactor
{
    APB_DIV_FACTOR_DIV_NO = 0b000,
    APB_DIV_FACTOR_DIV_2  = 0b100,
    APB_DIV_FACTOR_DIV_4  = 0b101,
    APB_DIV_FACTOR_DIV_8  = 0b110,
    APB_DIV_FACTOR_DIV_16 = 0b111
} ApbDivFactor;


typedef union RccCrRegBits
{
    struct __attribute__((packed))
    {
	uint8_t hsion : 1;
	uint8_t hsirdy : 1; 
	uint8_t reserved1 : 1; 
	uint8_t hsitrim : 5; 
	uint8_t hsical : 8; 
	uint8_t hseon : 1; 
	uint8_t hserdy : 1; 
	uint8_t hsebyp: 1; 
	uint8_t csson : 1; 
	uint8_t reserved2 : 4; 
	uint8_t pllon : 1; 
	uint8_t pllrdy : 1; 
	uint8_t reserved3 : 6;
    };

    uint32_t whole_reg;
} RccCrRegBits;

STATIC_ASSERT( sizeof(RccCrRegBits) == sizeof(uint32_t), RccCrRegBitsShouldBe4Byte );

typedef union RccCfgrRegBits
{
    struct __attribute__((packed))
    {
	SysClockSource sw : 2; 
	SysClockSource sws : 2;
	AhbDivFactor hpre : 4;
	ApbDivFactor ppre : 3;
	uint8_t reserved1 : 3;
	uint8_t adcpre : 1;
	PllSrc pllsrc : 2;
	uint8_t pllxtpre : 1;
	uint8_t pllmul : 4;
	uint8_t reserved2 : 2;
	MCClockInput mco : 4;
	uint8_t mcopre : 3;
	uint8_t pllnodiv : 1;
    };

    uint32_t whole_reg;
} RccCfgrRegBits;

int
toPllMul( int multiplier)
{
    return multiplier - 2;
}

STATIC_ASSERT( sizeof(RccCfgrRegBits) == sizeof(uint32_t), RccCfgrRegBitsShouldBe4Byte );


typedef union RccCfgr2RegBits
{
    struct __attribute__((packed))
    {
	uint8_t  prediv : 4;
	uint32_t reserved : 28;
    };
    
    uint32_t whole_reg;
} RccCfgr2RegBits;

int
toPreDiv( int div_factor)
{
    return div_factor - 1;
}

STATIC_ASSERT( sizeof(RccCfgr2RegBits) == sizeof(uint32_t), RccCfgr2RegBitsShouldBe4Byte );


typedef union RccAhbenrRegBits
{
    struct __attribute__((packed))
    {
	uint8_t dmaen : 1;
	uint8_t dma2en : 1;
	uint8_t sramen : 1;
	uint8_t reserved1 : 1;   
	uint8_t flitfen : 1; 
	uint8_t reserved2 : 1;  
	uint8_t crcen : 1;
	uint16_t reserved3 : 10;
	uint8_t iopaen : 1;
	uint8_t iopben : 1;
	uint8_t iopcen : 1;
	uint8_t iopden : 1;
	uint8_t iopeen : 1;
	uint8_t iopfen : 1;
	uint8_t reserved4 : 1; 
	uint8_t tsce : 1;
	uint8_t reserved5 : 7;
    };

    uint32_t whole_reg;
} RccAhbenrRegBits;

STATIC_ASSERT( sizeof(RccAhbenrRegBits) == sizeof(uint32_t), RccAhbenrRegBitsShouldBe4Byte );

typedef union RccApb2enrRegBits
{
    struct __attribute__((packed))
    {
	uint8_t syscfgcompen : 1;
	uint8_t reserved1 : 4;
	uint8_t usart6en : 1;
	uint8_t usart7en : 1;
	uint8_t usart8en : 1;
	uint8_t reserved2 : 1;
	uint8_t adcen : 1;
	uint8_t reserved3 : 1;
	uint8_t tim1en : 1;
	uint8_t spi1en : 1;
	uint8_t reserved4 : 1;
	uint8_t usart1en : 1;
	uint8_t reserved5 : 1;
	uint8_t tim15en : 1;
	uint8_t tim16en : 1;
	uint8_t tim17en : 1;
	uint8_t reserved6 : 3;
	uint8_t dbgmcuen : 1;
	uint16_t reserved7 : 9;
    };

    uint32_t whole_reg;
} RccApb2enrRegBits;


STATIC_ASSERT( sizeof(RccApb2enrRegBits) == sizeof(uint32_t), RccApb2enrRegBitsShouldBe4Byte );

typedef union RccCfgr3RegBits
{
    struct __attribute__((packed))
    {
	uint8_t usart1sw : 2;
	uint8_t reserved1 : 2;
	uint8_t i2c1sw : 1;
	uint8_t reserved2 : 1;
	uint8_t cecsw : 1;
	uint8_t usbsw : 1;
	uint8_t adcsw : 1;
	uint8_t reserved3 : 7;
	uint8_t usart2sw : 2;
	uint8_t usart3sw : 2;
	uint16_t reserved4 : 12;
    };

    uint32_t whole_reg;
} RccCfgr3RegBits;

STATIC_ASSERT( sizeof(RccCfgr3RegBits) == sizeof(uint32_t), RccCfgr3RegBitsShouldBe4Byte );

static volatile RccCrRegBits* gRccCr = (volatile RccCrRegBits*)0x40021000U;
static volatile RccCfgrRegBits* gRccCfgr = (volatile RccCfgrRegBits*)0x40021004U;
static volatile RccCfgr2RegBits* gRccCfgr2 = (volatile RccCfgr2RegBits*)0x4002102CU;
static volatile RccCfgr3RegBits* gRccCfgr3 = (volatile RccCfgr3RegBits*)0x40021030U;
static volatile RccAhbenrRegBits* gRccAhbenr = (volatile RccAhbenrRegBits*)0x40021014U;
static volatile RccApb2enrRegBits* gRccApb2enr = (volatile RccApb2enrRegBits*)0x40021018U;


#endif
