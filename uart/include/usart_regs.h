#ifndef USART_REG_H
#define USART_REG_H

#include <stdint.h>
#include "macro.h"

typedef union UsartCr1RegBits
{
    struct __attribute__((packed))
    {
	uint8_t ue : 1;
	uint8_t uesm : 1;
	uint8_t re : 1;
	uint8_t te : 1;
	uint8_t idleie : 1;
	uint8_t rxneie : 1;
	uint8_t tcie : 1;
	uint8_t txeie : 1;
	uint8_t peie : 1;
	uint8_t ps : 1;
	uint8_t pce : 1;
	uint8_t wake : 1;
	uint8_t m0 : 1;
	uint8_t mme : 1;
	uint8_t cmie : 1;
	uint8_t over8 : 1;
	uint8_t dedt : 4;
	uint8_t deat : 4;
	uint8_t rtoie : 1;
	uint8_t eobie : 1;
	uint8_t m1 : 1;
	uint8_t reserved : 3;
    };

    uint32_t whole_reg;
    
} UsartCr1RegBits;

STATIC_ASSERT( sizeof( UsartCr1RegBits ) == sizeof( uint32_t ), UsartCr1RegBitsMustBe4Bytes );

typedef union UsartCr2RegBits
{
    struct __attribute__((__packed__))
    {
	uint8_t reserved1 : 4;
	uint8_t addm7 : 1;
	uint8_t lbdl : 1;
	uint8_t lbdie : 1;
	uint8_t reserved2 : 1;
	uint8_t lbcl : 1;
	uint8_t cpha : 1;
	uint8_t cpol : 1;
	uint8_t cplken : 1;
	uint8_t stop : 2;
	uint8_t linen : 1;
	uint8_t swap : 1;
	uint8_t rxinv : 1;
	uint8_t txinv : 1;
	uint8_t datainv : 1;
	uint8_t msbfirst : 1;
	uint8_t abren : 1;
	uint8_t abrmod : 2;
	uint8_t rtoen : 1;
	uint8_t add : 8;
    };

    uint32_t whole_reg;
} UsartCr2RegBits;

STATIC_ASSERT( sizeof( UsartCr2RegBits ) == sizeof( uint32_t ), UsartCr2RegBitsMustBe4Bytes );


typedef union UsartBrrRegBits
{
    struct __attribute__((__packed__))
    {
	uint16_t brr;
	uint16_t reserved;
    };

    uint32_t whole_reg;
} UsartBrrRegBits;

STATIC_ASSERT( sizeof( UsartBrrRegBits ) == sizeof( uint32_t ), UsartBrrRegBitsMustBe4Bytes );


typedef union UsartTdrRegBits
{
    struct __attribute__((__packed__))
    {
	uint8_t tdr;
	uint32_t reserved : 24;
    };

    uint32_t whole_reg;
} UsartTdrRegBits;

STATIC_ASSERT( sizeof( UsartTdrRegBits ) == sizeof( uint32_t ), UsartTdrRegBitsMustBe4Bytes );


typedef union UsartRdrRegBits
{
    struct __attribute__((__packed__))
    {
	uint8_t rdr;
	uint32_t reserved : 24;
    };

    uint32_t whole_reg;
} UsartRdrRegBits;

STATIC_ASSERT( sizeof( UsartRdrRegBits ) == sizeof( uint32_t ), UsartRdrRegBitsMustBe4Bytes );

typedef union UsartIsrRegBits
{
    struct __attribute__((__packed__))
    {
	uint8_t pe : 1;
	uint8_t fe : 1;
	uint8_t nf : 1;
	uint8_t ore : 1;
	uint8_t idle : 1;
	uint8_t rxne : 1;
	uint8_t tc : 1;
	uint8_t txe : 1;
	uint8_t lbdf : 1;
	uint8_t ctsif : 1;
	uint8_t cts : 1;
	uint8_t rtof : 1;
	uint8_t eobf : 1;
	uint8_t reserved1 : 1;
	uint8_t abre : 1;
	uint8_t abrf : 1;
	uint8_t busy : 1;
	uint8_t cmf : 1;
	uint8_t sbkf : 1;
	uint8_t rwu : 1;
	uint8_t wuf : 1;
	uint8_t teack : 1;
	uint8_t reack : 1;
	uint16_t reserved2 : 9;
    };

    uint32_t whole_reg;
} UsartIsrRegBits;

STATIC_ASSERT( sizeof( UsartIsrRegBits ) == sizeof( uint32_t ), UsartIsrRegBitsMustBe4Bytes );

static volatile UsartCr1RegBits* gUsart1Cr1 = (volatile UsartCr1RegBits*) 0x40013800U; 
static volatile UsartCr2RegBits* gUsart1Cr2 = (volatile UsartCr2RegBits*) 0x40013804U; 
static volatile UsartBrrRegBits* gUsart1Brr = (volatile UsartBrrRegBits*) 0x4001380CU; 
static volatile UsartIsrRegBits* gUsart1Isr = (volatile UsartIsrRegBits*) 0x4001381CU; 
static volatile UsartRdrRegBits* gUsart1Rdr = (volatile UsartRdrRegBits*) 0x40013824U; 
static volatile UsartTdrRegBits* gUsart1Tdr = (volatile UsartTdrRegBits*) 0x40013828U; 

#endif
