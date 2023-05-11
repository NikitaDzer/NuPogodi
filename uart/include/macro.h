#ifndef MACRO_H
#define MACRO_H

#define FIRST(a, ...) a
#define SECOND(a, b, ...) b

#define EMPTY()

/* Macro for additional macro expanding pass */
#define EVAL(...) EVAL1024(__VA_ARGS__)
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))
#define EVAL512(...) EVAL256(EVAL256(__VA_ARGS__))
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

/* macro to defer macro expansion */
#define DEFER1(m) m EMPTY()
#define DEFER2(m) m EMPTY EMPTY()()
#define DEFER3(m) m EMPTY EMPTY EMPTY()()()
#define DEFER4(m) m EMPTY EMPTY EMPTY EMPTY()()()()
#define DEFER5(m) m EMPTY EMPTY EMPTY EMPTY EMPTY()()()()()

/* IS_PROBE = 1 if the argument is macro PROBE() */
#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1
/* check for paretheses */
#define IS_PAREN(x) IS_PROBE( IS_PAREN_PROBE x )
#define IS_PAREN_PROBE(...) PROBE()

#define CAT(a,b) a ## b
#define CAT3(a, b, ...) a ## b ## __VA_ARGS__

#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()

/* macro cast to bool any value ( 1 - for any object, 0 - for 0 ) */
#define BOOL(x) NOT(NOT(x))

/* expand the branch depending on compile time condition */
#define IF_ELSE(condition) _IF_ELSE(BOOL(condition))
#define _IF_ELSE(condition) CAT(_IF_, condition)

#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

/* check if the macro has arguments */
#define HAS_ARGS(...) BOOL(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _END_OF_ARGUMENTS_() 0

/* apply macro to list of arguments */
#define MAP(m, first, ...)           \
  m(first)                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP)()(m, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP() MAP


/* increase number */
#define INC(x) CAT3(INC_, x)
#define INC_0 1
#define INC_1 2
#define INC_2 3
#define INC_3 4
#define INC_4 5
#define INC_5 6
#define INC_6 7
#define INC_7 8
#define INC_8 9
#define INC_9 10
#define INC_10 11
#define INC_11 12
#define INC_12 13

/* decrease number */
#define DEC(x) CAT3(DEC_, x)
#define DEC_0 0
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8
#define DEC_10 9
#define DEC_11 10
#define DEC_12 11
#define DEC_13 12

/* repeat macro count times */
#define REPEAT(count, macro, ...)		 \
    IF_ELSE(count)(				 \
	DEFER2(_REPEAT) ()			 \
        (					 \
	    DEC(count), macro, __VA_ARGS__       \
	)                         	         \
	DEFER2(macro) ( DEC(count), __VA_ARGS__) \
	)( /* do nothing */ )

#define _REPEAT() REPEAT

/* repeat macro count - 1 times, at the end call the end_macro */
#define REPEAT_END(count, macro, end_macro, ...)  \
    REPEAT(DEC(count), macro, __VA_ARGS__)        \
    end_macro(DEC(count), __VA_ARGS__)


#define CMP_0(x) x
#define CMP_1(x) x
#define CMP_2(x) x
#define CMP_3(x) x
#define CMP_4(x) x
#define CMP_5(x) x
#define CMP_6(x) x
#define CMP_7(x) x
#define CMP_8(x) x
#define CMP_9(x) x
#define CMP_10(x) x
#define CMP_11(x) x
#define CMP_12(x) x

/* compare two tokens ( by tokens I mostly mean two strings like foo and bar ) */
#define TOKEN_NOT_EQUAL(x, y) IS_PAREN ( CMP_ ## x ( CMP_ ## y) (()) )
#define TOKEN_EQUAL(x, y) NOT(NOT_EQUAL(x,y))

/* repeat macro end - start times */
#define RANGE_REPEAT( start, end, macro, ... )  \
    IF_ELSE(TOKEN_NOT_EQUAL(end, start))(       \
	DEFER2(_RANGE_REPEAT) ()                \
	(                                       \
	    start, DEC(end), macro, __VA_ARGS__ \
	)                                       \
        DEFER2(macro) ( DEC(end), __VA_ARGS__ ) \
	) ( /* do nothing */ )

#define _RANGE_REPEAT() RANGE_REPEAT

#define RANGE_REPEAT_END( start, end, macro, end_macro, ... ) \
    RANGE_REPEAT( start, DEC( end ), macro, __VA_ARGS__ )     \
    end_macro( DEC( end ), __VA_ARGS__ )

#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]

/* count number of arguments */
#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#endif
