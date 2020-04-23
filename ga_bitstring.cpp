#include "ga_bitstring.h"

byte *ga_bit_new( int length )
  {
  byte *ptr;

  if ( !(ptr = (byte *) s_malloc( ga_bit_sizeof( length ) )) )
    die("Unable to allocate bitstring.");

  return ptr;
  }

void ga_bit_free( byte *bstr )
  {
  s_free( bstr );

  return;
  }

void ga_bit_set( byte *bstr, int n )
  {
  bstr[n/BYTEBITS] |= 1 << ( n%BYTEBITS );

  return;
  }

void ga_bit_clear( byte *bstr, int n )
  {
  bstr[n/BYTEBITS] &= ~(1 << ( n%BYTEBITS ));

  return;
  }

void ga_bit_invert( byte *bstr, int n )
  {
  bstr[n/BYTEBITS] ^= 1 << (n % BYTEBITS);

  return;
  }




boolean ga_bit_get( byte *bstr, int n )
  {
  return (boolean) ( (bstr[n/BYTEBITS] & (1 << (n % BYTEBITS))) != 0 );
  }

void ga_bit_randomize( byte *bstr, int n )
  {
  if ( random_boolean() )
    ga_bit_set( bstr, n );
  else
    ga_bit_clear( bstr, n );

  return;
  }

void ga_bit_copy( byte *dest, byte *src, int ndest, int nsrc, int length )
  {
  int i;

  if (dest != src || ndest < nsrc)
    {
    for ( i=0; i < length; ++i )
      {
      if ( ga_bit_get(src, nsrc+i) )
        ga_bit_set( dest, ndest+i );
      else
        ga_bit_clear( dest, ndest+i );
      }
    }
  else
    {
    for ( i = length-1 ; i >= 0; --i )
      {
      if ( ga_bit_get(src, nsrc+i) )
        ga_bit_set( dest, ndest+i );
      else
        ga_bit_clear( dest, ndest+i );
      }
    }

  return;
  }

size_t ga_bit_sizeof( int length )
  {
/* Note that sizeof(byte) should always be 1. */
  return sizeof(byte) * (length+BYTEBITS-1) / BYTEBITS;
  }

byte *ga_bit_clone( byte *dest, byte *src, int length )
  {
  if (!dest) dest=ga_bit_new( length );

  memcpy( dest, src, ga_bit_sizeof( length ) );

  return dest;
  }


unsigned int ga_bit_decode_binary_uint( byte *bstr, int n, int length )
  {
  int		i;
  unsigned int	value=0;	/* Decoded value. */

  for ( i=n; i < n+length; i++ )
    {
    value <<= 1;
    value |= ga_bit_get(bstr, i);
    }

  return value;
  }

void ga_bit_encode_binary_uint( byte *bstr, int n, int length, unsigned int value )
  {
  int i;

  /* Set bits in _reverse_ order. */
  for ( i = n+length-1; i >= n; i-- )
    {
    if ( value & 1 )
      ga_bit_set( bstr, i );
    else
      ga_bit_clear( bstr, i );

    value >>= 1;
    }

  return;
  }

int ga_bit_decode_binary_int( byte *bstr, int n, int length )
  {
  if ( ga_bit_get( bstr, n ) )
    return (int) -ga_bit_decode_binary_uint( bstr, n+1, length-1 );
  else
    return (int) ga_bit_decode_binary_uint( bstr, n+1, length-1 );
  }

void ga_bit_encode_binary_int( byte *bstr, int n, int length, int value )
  {
  if ( value < 0 )
    {
    ga_bit_set( bstr, n );
    value = -value;
    }
  else
    {
    ga_bit_clear( bstr, n );
    }

  ga_bit_encode_binary_uint( bstr, n+1, length-1, (unsigned int) value );

  return;
  }

static void gray_to_binary( byte *gray_bstr, int n, byte *int_bstr, int length )
  {
  boolean	bit;
  int		i;

  bit = ga_bit_get( gray_bstr, n );
  if (bit)
    ga_bit_set( int_bstr, 0 );
  else
    ga_bit_clear( int_bstr, 0 );

  for ( i=1; i<length; i++ )
    {
    if (bit)
      bit = !ga_bit_get( gray_bstr, n+i );
    else
      bit = ga_bit_get( gray_bstr, n+i );

    if (bit)
      ga_bit_set( int_bstr, i );
    else
      ga_bit_clear( int_bstr, i );
    }  

  return;
  }

static void binary_to_gray( byte *gray_bstr, int n, byte *int_bstr, int length )
  {
  boolean	bit;
  int		i;

  bit = ga_bit_get( int_bstr, 0 );
  if (bit)
    ga_bit_set( gray_bstr, n );
  else
    ga_bit_clear( gray_bstr, n );

  for ( i=1; i < length; i++ )
    {

    if (bit)
      {
      bit = ga_bit_get( int_bstr, i );
      if (bit)
        ga_bit_clear( gray_bstr, n+i );
      else
        ga_bit_set( gray_bstr, n+i );
      }
    else
      {
      bit = ga_bit_get( int_bstr, i );
      if (bit)
        ga_bit_set( gray_bstr, n+i );
      else
        ga_bit_clear( gray_bstr, n+i );
      }
    }

  return;
  }


int ga_bit_decode_gray_int( byte *bstr, int n, int length )
  {
  byte		*int_bstr;
  int		val;

  if ( !(int_bstr = (byte *) s_malloc( ga_bit_sizeof( length ) )) )
    die("Unable to allocate bitstring.");

  gray_to_binary( bstr, n, int_bstr, length );

  val = ga_bit_decode_binary_int( int_bstr, 0, length );

  s_free(int_bstr);

  return val;
  }

unsigned int ga_bit_decode_gray_uint( byte *bstr, int n, int length )
  {
  byte		*int_bstr;
  unsigned int	val;

  if ( !(int_bstr = (byte *) s_malloc( ga_bit_sizeof( length ) )) )
    die("Unable to allocate bitstring.");

  gray_to_binary( bstr, n, int_bstr, length );

  val = ga_bit_decode_binary_uint( int_bstr, 0, length );

  s_free(int_bstr);

  return val;
  }

void ga_bit_encode_gray_uint( byte *bstr, int n, int length, unsigned int value )
  {
  byte	*int_bstr;

  if ( !(int_bstr = (byte *) s_malloc( ga_bit_sizeof( length ) )) )
    die("Unable to allocate bitstring.");

  ga_bit_encode_binary_uint( int_bstr, 0, length, value );
  binary_to_gray( bstr, n, int_bstr, length );

  s_free(int_bstr);

  return;
  }

void ga_bit_encode_gray_int( byte *bstr, int n, int length, int value )
  {
  byte	*int_bstr;

  if ( !(int_bstr = (byte *) s_malloc( ga_bit_sizeof( length ) )) )
    die("Unable to allocate bitstring.");

  ga_bit_encode_binary_int( int_bstr, 0, length, value );
  binary_to_gray( bstr, n, int_bstr, length );

  s_free(int_bstr);

  return;
  }

double ga_bit_decode_binary_real( byte *bstr, int n, int mantissa, int exponent )
  {
  double	value;
  int		int_mantissa, int_exponent;

  int_mantissa = ga_bit_decode_binary_int( bstr, n, mantissa );
  int_exponent = ga_bit_decode_binary_int( bstr, n+mantissa, exponent );

  value = ((double)int_mantissa) / ((double)(1<<(mantissa-1)))
          * pow( 2.0, (double) int_exponent );

  return value;        
  }


void ga_bit_encode_binary_real( byte *bstr, int n, int mantissa, int exponent, double value )
  {
  int int_mantissa, int_exponent;

  int_mantissa = (int) floor(frexp( value, &int_exponent ) * (double)(1<<(mantissa-1)));
  ga_bit_encode_binary_int( bstr, n, mantissa, int_mantissa );
  ga_bit_encode_binary_int( bstr, n+mantissa, exponent, int_exponent );

  return;
  }

double ga_bit_decode_gray_real( byte *bstr, int n, int mantissa, int exponent )
  {
  double	value;
  int		int_mantissa, int_exponent;

  int_mantissa = ga_bit_decode_gray_int( bstr, n, mantissa );
  int_exponent = ga_bit_decode_gray_int( bstr, n+mantissa, exponent );

  value = pow( 2.0, (double) int_exponent ) *
          ((double)int_mantissa) / ((double)(1<<(mantissa-1)));

  return value;        
  }

void ga_bit_encode_gray_real( byte *bstr, int n, int mantissa, int exponent, double value )
  {
  int int_mantissa, int_exponent;

  int_mantissa = (int) floor(frexp( value, &int_exponent ) * (double)(1<<(mantissa-1)));

  ga_bit_encode_gray_int( bstr, n, mantissa, int_mantissa );
  ga_bit_encode_gray_int( bstr, n+mantissa, exponent, int_exponent );

  return;
  }

boolean ga_bit_test( void )
  {
  int		i;			/* Loop variable. */
  double	origval, newval;	/* Value before and after encoding+decoding. */
  int		origint, newint;	/* Value before and after encoding+decoding. */
  byte		*bstr;

  if ( !(bstr = (byte *) s_malloc( ga_bit_sizeof( 128 ) )) )
    die("Unable to allocate bitstring.");

  printf("Binary encoding of integers:\n");

  for (i=0; i<10; i++)
    {
    origint = 23*i-30;
    ga_bit_encode_binary_int( bstr, 0, 64, origint );
    newint = ga_bit_decode_binary_int( bstr, 0, 64 );
    printf("Orig val = %d new val = %d %s\n",
           origint, newint, origint==newint?"PASSED":"FAILED");
    }

  printf("Binary encoding of reals:\n");

  for (i=0; i<10; i++)
    {
    origval = -0.3+0.16*i;
    ga_bit_encode_binary_real( bstr, 0, 64, 64, origval );
    newval = ga_bit_decode_binary_real( bstr, 0, 64, 64 );
    printf("Orig val = %f new val = %f %s\n",
           origval, newval,
           (origval>newval-TINY&&origval<newval+TINY)?"PASSED":"FAILED");
    }

  printf("Gray encoding of integers:\n");

  for (i=0; i<10; i++)
    {
    origint = 23*i-30;
    ga_bit_encode_gray_int( bstr, 0, 64, origint );
    newint = ga_bit_decode_gray_int( bstr, 0, 64 );
    printf("Orig val = %d new val = %d %s\n",
           origint, newint, origint==newint?"PASSED":"FAILED");
    }

  printf("Gray encoding of reals:\n");

  for (i=0; i<10; i++)
    {
    origval = -0.3+0.16*i;
    ga_bit_encode_gray_real( bstr, 0, 64, 64, origval );
    newval = ga_bit_decode_gray_real( bstr, 0, 64, 64 );
    printf("Orig val = %f new val = %f %s\n",
           origval, newval,
           (origval>newval-TINY&&origval<newval+TINY)?"PASSED":"FAILED");
    }

  s_free(bstr);

  return TRUE;
  }


