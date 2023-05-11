#ifndef BITMASK_H
#define BITMASK_H

#include "macro.h"


#define SET_4BIT( mask_num, bitmask ) ( bitmask << ( mask_num * 4 ) )
#define IN_RANGE_SET_4BIT( mask_num, bitmask ) SET_4BIT( mask_num, bitmask ) |
#define IN_RANGE_SET_4BIT_END( mask_num, bitmask ) SET_4BIT( mask_num, bitmask )

/* set range of bitmask in range [from; until] */
#define RANGE_SET_4BIT( from, until, bitmask ) \
    EVAL( RANGE_REPEAT_END( from, INC( until ), IN_RANGE_SET_4BIT, IN_RANGE_SET_4BIT_END, bitmask ) )

/* set range of bitmask from range [0; until] */
#define Z_RANGE_SET_4BIT( until, bimask ) EVAL( REPEAT_END( INC( until ), SET_4BIT, SET_4BIT_END, bimask ) )

#define SET_2BIT( mask_num, bitmask ) ( bitmask << ( mask_num * 2 ) )
/* set range of bitmask in range [from; until] */
#define IN_RANGE_SET_2BIT( mask_num, bitmask ) SET_2BIT( mask_num, bitmask ) |
#define IN_RANGE_SET_2BIT_END( mask_num, bitmask ) SET_2BIT( mask_num, bitmask )
#define RANGE_SET_2BIT( from, until, bitmask ) \
    EVAL( RANGE_REPEAT_END( from, INC( until ), IN_RANGE_SET_2BIT, IN_RANGE_SET_2BIT_END, bitmask ) )
/* set range of bitmask from range [0; until] */
#define Z_RANGE_SET_2BIT( until, bimask ) EVAL( REPEAT_END( INC( until ), SET_2BIT, SET_2BIT_END, bimask ) )


#define SET_1BIT( mask_num ) ( 1 << ( mask_num ) )
#define IN_RANGE_SET_1BIT( mask_num, _ ) SET_1BIT( mask_num )|
#define IN_RANGE_SET_1BIT_END( mask_num, _ ) SET_1BIT( mask_num )

#define RANGE_SET_1BIT( from, until ) \
    EVAL( REPEAT_END( from, INC( until ), IN_RANGE_SET_1BIT, IN_RANGE_SET_1BIT_END, ~ ) )
#define Z_RANGE_SET_1BIT( until ) \
    EVAL( REPEAT_END( INC( until ), IN_RANGE_SET_1BIT, IN_RANGE_SET_1BIT_END, ~ ) )

#define IN_RANGE_CLEAR_1BIT( mask_num, _ ) ~( 1 << ( mask_num ) ) &
#define IN_RANGE_CLEAR_1BIT_END( mask_num, _ ) ~( 1 << ( mask_num ) )

#define RANGE_CLEAR_1BIT( from, until ) \
    EVAL( RANGE_REPEAT_END( from, INC( until ), IN_RANGE_CLEAR_1BIT, IN_RANGE_CLEAR_1BIT_END, ~ ) )
#define Z_RANGE_CLEAR_1BIT( until ) \
    EVAL( REPEAT_END( INC( until ), IN_RANGE_CLEAR_1BIT, IN_RANGE_CLEAR_1BIT_END, ~ ) )

#define CLEAR_1BIT( mask_num ) ~( 1 << ( mask_num ) )

#endif
