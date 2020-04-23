#ifndef GA_BITSTRING_H_INCLUDED
#define GA_BITSTRING_H_INCLUDED


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#define FUNCPROTO

#if !defined(TRUE)
#define TRUE	(0==0)
#define FALSE	(0!=0)
#endif

/*
 * Additional types.
 */
#ifndef SUN_FORTE_C
# define SUN_FORTE_C 0
#endif
#if SUN_FORTE_C==0
typedef void* vpointer;
#else
#define vpointer void*
#endif

typedef const void *constvpointer;
/* byte is already defined on win32 systems. */
#ifdef USE_WINDOWS_H
# if USE_WINDOWS_H == 0
typedef unsigned char byte;
# endif
#else
typedef unsigned char byte;
#endif

#ifdef BITSPERBYTE
# define BYTEBITS	BITSPERBYTE
#else
# ifdef CHARBITS
#  define BYTEBITS	CHARBITS
# else
/* Guess! */
#  define BYTEBITS	8
# endif
#endif

#define boolean bool

/* Use standard system calls with no wrappers. */

#define s_malloc(X)		malloc((X))
#define s_calloc(X,Y)		calloc((X),(Y))
#define s_realloc(X,Y)		realloc((X),(Y))
#define s_malloc_labelled(X, Y)	malloc((X))
#define s_strdup(X)		strdup((X))
/* Many systems don't have a strndup() function, so we just avoid the problem completely! */
#define s_strndup(X, Y)		s_strndup_safe((X), (Y),		\
				__PRETTY_FUNCTION__, __FILE__, __LINE__)
#define s_free(X)		free((X))

#define MWRAP_BEGIN
#define MWRAP_END

#define TINY            (1.0e-8)

#define s_breakpoint	abort()
/*
 * die() macro, inspired by perl!
 */
#define die(X)	MWRAP_BEGIN {						\
                printf("FATAL ERROR: %s\nin %s at \"%s\" line %d\n",	\
		(X),					\
                __PRETTY_FUNCTION__,			\
                __FILE__,				\
                __LINE__);				\
		fflush(NULL);				\
                s_breakpoint;				\
		} MWRAP_END


/*
 * Prototypes.
 */
FUNCPROTO byte *ga_bit_new( int length );
FUNCPROTO void ga_bit_free( byte *bstr );
FUNCPROTO void ga_bit_set( byte *bstr, int n );
FUNCPROTO void ga_bit_clear( byte *bstr, int n );
FUNCPROTO void ga_bit_invert( byte *bstr, int n );
FUNCPROTO boolean ga_bit_get( byte *bstr, int n );
FUNCPROTO void ga_bit_randomize( byte *bstr, int n );
FUNCPROTO void ga_bit_copy( byte *dest, byte *src, int ndest, int nsrc, int length );
FUNCPROTO size_t ga_bit_sizeof( int length );
FUNCPROTO byte *ga_bit_clone( byte *dest, byte *src, int length );

/* Integer conversion. */
FUNCPROTO unsigned int ga_bit_decode_binary_uint( byte *bstr, int n, int length );
FUNCPROTO void ga_bit_encode_binary_uint( byte *bstr, int n, int length, unsigned int value );
FUNCPROTO int ga_bit_decode_binary_int( byte *bstr, int n, int length );
FUNCPROTO void ga_bit_encode_binary_int( byte *bstr, int n, int length, int value );
FUNCPROTO int ga_bit_decode_gray_int( byte *bstr, int n, int length );
FUNCPROTO unsigned int ga_bit_decode_gray_uint( byte *bstr, int n, int length );
FUNCPROTO void ga_bit_encode_gray_uint( byte *bstr, int n, int length, unsigned int value );

/* Real conversion. */
FUNCPROTO double ga_bit_decode_binary_real( byte *bstr, int n, int mantissa, int exponent );
FUNCPROTO void ga_bit_encode_binary_real( byte *bstr, int n, int mantissa, int exponent, double value );
FUNCPROTO double ga_bit_decode_gray_real( byte *bstr, int n, int mantissa, int exponent );
FUNCPROTO void ga_bit_encode_grayy_real( byte *bstr, int n, int mantissa, int exponent, double value );

/* Test. */
FUNCPROTO boolean ga_bit_test( void );

FUNCPROTO boolean	random_boolean(void);
#endif	/* GA_BITSTRING_H_INCLUDED */

