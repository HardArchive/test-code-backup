///
/// @file kl_gen_container.h
/// @author Kevin Lynx
///
#ifndef ___KL_GEN_CONTAINER_H_
#define ___KL_GEN_CONTAINER_H_

#include "kl_macro_params.h"

namespace kl_common
{
	///
	/// Param(n) class can store any type data in it.
	/// It's a generic container to contain any type of values.
	/// i.e:
	/// Param2<int, char>( a, b ); which a is a int value and b is
	/// a char value.
	///
	struct Param0
	{
	};

#define CREATE_PARAM( n ) \
	template <DEF_PARAM( n )> \
	struct Param##n \
	{ \
		DEF_PARAM_TYPE( n ); \
		Param##n( DEF_FUNC_PARAM( n ) ) \
		{ \
			DEF_MEM_VAR_ASSIGN( n ); \
		} \
		DEF_VAR_DEF( n ); \
	}

	CREATE_PARAM( 1 );
	CREATE_PARAM( 2 );
	CREATE_PARAM( 3 );
	CREATE_PARAM( 4 );
	CREATE_PARAM( 5 );
	CREATE_PARAM( 6 );
	CREATE_PARAM( 7 );
	CREATE_PARAM( 8 );
	CREATE_PARAM( 9 );
	CREATE_PARAM( 10 );
	CREATE_PARAM( 11 );
	CREATE_PARAM( 12 );
	CREATE_PARAM( 13 );
	CREATE_PARAM( 14 );
	CREATE_PARAM( 15 );


	///
	/// A helper generic function to create a Param(n) object.
	/// Usage :
	/// int a; char b;
	/// Param<int, char> ret = create_param( a, b );
	/// and you can pass ret to any place you want, then :
	/// ret.p1, ret.p2 to query the values.
	/// 
	inline Param0 create_param()
	{
		return Param0();
	}

#define CREATE_PARAM_CREATOR( n ) \
	template <DEF_PARAM( n )> \
	Param##n<DEF_ARG( n )> create_param( DEF_FUNC_PARAM_P( n ) ) \
	{ \
		return Param##n<DEF_ARG( n )>( DEF_FUNC_ARG( n ) ); \
	} 

	CREATE_PARAM_CREATOR( 1 );
	CREATE_PARAM_CREATOR( 2 );
	CREATE_PARAM_CREATOR( 3 );
	CREATE_PARAM_CREATOR( 4 );
	CREATE_PARAM_CREATOR( 5 );
	CREATE_PARAM_CREATOR( 6 );
	CREATE_PARAM_CREATOR( 7 );
	CREATE_PARAM_CREATOR( 8 );
	CREATE_PARAM_CREATOR( 9 );
	CREATE_PARAM_CREATOR( 10 );
	CREATE_PARAM_CREATOR( 11 );
	CREATE_PARAM_CREATOR( 12 );
	CREATE_PARAM_CREATOR( 13 );
	CREATE_PARAM_CREATOR( 14 );
	CREATE_PARAM_CREATOR( 15 );
}

#endif
