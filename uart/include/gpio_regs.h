#ifndef GPIO_REGS_H
#define GPIO_REGS_H
#include <stdint.h>
#include "macro.h"

typedef enum GpioXModerPinMode
{
    MODER_PIN_MODE_INPUT = 0b00,
    MODER_PIN_MODE_GPO   = 0b01,
    MODER_PIN_MODE_ALTERNATE = 0b10,
    MODER_PIN_MODE_ANALOG    = 0b11
} GpioXModerPinMode;

typedef enum GpioXPupdrPinMode
{
    PUPDR_PIN_MODE_DISABLED  = 0b00,
    PUPDR_PIN_MODE_PULL_UP   = 0b01,
    PUPDR_PIN_MODE_PULL_DOWN = 0b10,
    PUPDR_PIN_MODE_RESERVED  = 0b11
} GpioXPupdrPinMode;

typedef enum GpioXAfhrAlternateFunction
{
    AF0 = 0b0000,
    AF1 = 0b0001,
    AF2 = 0b0010,
    AF3 = 0b0011,
    AF4 = 0b0100,
    AF5 = 0b0101,
    AF6 = 0b0110,
    AF7 = 0b0111,
} GpioXAfhrAlternateFunction;

typedef enum GpioXOspeedrMode
{
    OSPEEDR_LOW_1 = 0b00,
    OSPEEDR_LOW_2 = 0b10,
    OSPEEDR_MEDIUM = 0b01,
    OSPEEDR_HIGHT = 0b11
} GpioXOspeedMode;

typedef union GpioX4BitFieldRegBits
{
    uint32_t gpio_4bit_fields;
    uint32_t whole_reg;
} GpioX4BitFieldRegBits;

typedef GpioX4BitFieldRegBits GpioXAfhrRegBits;

STATIC_ASSERT( sizeof( GpioX4BitFieldRegBits ) == sizeof( uint32_t ), GpioX4BitFieldRegBitsShouldBe4Bytes );

typedef union GpioX2BitFieldRegBits
{
    uint32_t gpio_2bit_fields; /* sequence of bit fields with 2 bit length */
    uint32_t whole_reg;
} GpioX2BitFieldRegBits;

/* Pupdr and Moder registers has same structure */
typedef GpioX2BitFieldRegBits GpioXPupdrRegBits;
typedef GpioX2BitFieldRegBits GpioXModerRegBits;
typedef GpioX2BitFieldRegBits GpioXOspeedrRegBits;

STATIC_ASSERT( sizeof( GpioX2BitFieldRegBits ) == sizeof( uint32_t ), GpioX2BitFieldRegBitsShouldBe4Bytes );

typedef union __attribute__((packed)) GpioX1BitFieldRegBits
{
    struct __attribute__((packed))
    {
	uint16_t gpio_1bit_fields; // 1 bit field from 0 to 15
	uint16_t reserved;
    };
    
    uint32_t whole_reg;
} GpioX1BitFieldRegBits;

/* Typer Odr Idr registers has same structure */
typedef GpioX1BitFieldRegBits GpioXTyperRegBits;
typedef GpioX1BitFieldRegBits GpioXIdrRegBits;
typedef GpioX1BitFieldRegBits GpioXOdrRegBits;


STATIC_ASSERT( sizeof( GpioX1BitFieldRegBits ) == sizeof( uint32_t ), GpioX1BitFieldRegBitsShouldBe4Bytes );

//----------------
// GPIO Registers
//----------------


static volatile GpioXIdrRegBits* gGpioAIdr = (volatile GpioXIdrRegBits*) 0x48000010U; 
static volatile GpioXOdrRegBits* gGpioAOdr = (volatile GpioXOdrRegBits*) 0x48000014U; 

static volatile GpioXModerRegBits* gGpioAModer = (volatile GpioXModerRegBits*) 0x48000000U; 
static volatile GpioXModerRegBits* gGpioCModer = (volatile GpioXModerRegBits*) 0x48000800U; 

static volatile GpioXTyperRegBits* gGpioATyper = (volatile GpioXTyperRegBits*) 0x48000004U; 
static volatile GpioXTyperRegBits* gGpioCTyper = (volatile GpioXTyperRegBits*) 0x48000804U; 

static volatile GpioXPupdrRegBits* gGpioAPupdr = (volatile GpioXPupdrRegBits*) 0x4800080CU;
static volatile GpioXAfhrRegBits* gGpioAAfhdr = (volatile GpioXAfhrRegBits*) 0x48000024U;

static volatile GpioXOspeedrRegBits* gGpioAOspeedr = (volatile GpioXOspeedrRegBits*)0x48000008U;  

#endif
