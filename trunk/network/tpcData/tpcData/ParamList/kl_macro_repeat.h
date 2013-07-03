///
/// @file kl_macro_repeat.h
/// @author Kevin Lynx
/// @date 8.20.2008
///
#ifndef ___KL_MACRO_REPEAT_H_
#define ___KL_MACRO_REPEAT_H_

/**
  The macro recursion techinque is very interesting, but to understand it is not easy as the 
  c language books told you, but i can provide your supports. :D
*/

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
#define DEC_14 13
#define DEC_15 14

#define DEC( n ) DEC_##n

#define CHR( x, y ) CHR1( x, y )
#define CHR1( x, y ) x##y

/**
  As you see, REPEAT macro needs arguments :
  @n repeat count
  @f a callback macro can take one parameter : current repeat count
  @e the end symbol, take no parameter.
*/
#define REPEAT_0( n, f, e ) 
#define REPEAT_1( n, f, e ) e
#define REPEAT_2( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_3( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_4( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_5( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_6( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_7( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_8( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_9( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_10( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_11( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_12( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_13( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_14( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )
#define REPEAT_15( n, f, e ) CHR( CHR( REPEAT_, DEC( n ) )( DEC( n ), f, e ), f( n ) )


#endif // ___KL_MACRO_REPEAT_H_