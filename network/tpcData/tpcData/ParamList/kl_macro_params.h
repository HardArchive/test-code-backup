///
/// @file kl_macro_params.h
/// @author Kevin Lynx
/// @date 8.20.2008
///
#ifndef ___KL_MACRO_PARAMS_H_
#define ___KL_MACRO_PARAMS_H_

#include "kl_macro_repeat.h"

//参数n
#define PARAM( n ) ,typename P##n
//结束参数
#define PARAM_END typename P1

///
/// to expand a string like : typename P1, typename P2, typename P3
///
#define DEF_PARAM( n ) REPEAT_##n( n, PARAM, PARAM_END )

#define ARG( n ) ,P##n
#define ARG_END P1

///
/// to expand a string like : P1, P2, P3
///
#define DEF_ARG( n ) REPEAT_##n( n, ARG, ARG_END )

#define PARAM_TYPE( n ) ;typedef P##n p##n##_type
#define PARAM_TYPE_END typedef P1 p1_type

///
/// to expand a string like : typedef P1 p1_type; typedef P2 p2_type; 
///
#define DEF_PARAM_TYPE( n ) REPEAT_##n( n, PARAM_TYPE, PARAM_TYPE_END )

#define FUNC_PARAM_REF( n ) ,p##n##_type &p##n
#define FUNC_PARAM_REF_END p1_type &p1

///
/// to expand a string like : p1_type &p1, p2_type &p2
///
#define DEF_FUNC_PARAM_REF( n ) REPEAT_##n( n, FUNC_PARAM_REF, FUNC_PARAM_REF_END )

#define FUNC_PARAM( n ) ,p##n##_type p##n
#define FUNC_PARAM_END p1_type p1

///
/// to expand a string like : p1_type p1, p2_type p2
///
#define DEF_FUNC_PARAM( n ) REPEAT_##n( n, FUNC_PARAM, FUNC_PARAM_END )

#define FUNC_PARAM_P( n ) ,P##n p##n
#define FUNC_PARAM_P_END P1 p1

///
/// to expand a string like : P1 p1, P2 p2
///
#define DEF_FUNC_PARAM_P( n ) REPEAT_##n( n, FUNC_PARAM_P, FUNC_PARAM_P_END )

#define FUNC_ARG( n ) ,p##n
#define FUNC_ARG_END p1

///
/// to expand a string like : p1, p2, p3
///
#define DEF_FUNC_ARG( n ) REPEAT_##n( n, FUNC_ARG, FUNC_ARG_END )

#define VAR_DEF( n ) ;p##n##_type p##n
#define VAR_DEF_END p1_type p1

///
/// to expand a string like : p1_type p1; p2_type p2
///
#define DEF_VAR_DEF( n ) REPEAT_##n( n, VAR_DEF, VAR_DEF_END )

#define MEM_VAR_ASSIGN( n ) ;this->p##n##=p##n
#define MEM_VAR_ASSIGN_END this->p1=p1

///
/// to expand a string like : this->p1=p1; this->p2=p2
///
#define DEF_MEM_VAR_ASSIGN( n ) REPEAT_##n( n, MEM_VAR_ASSIGN, MEM_VAR_ASSIGN_END )

#endif // ___KL_MACRO_PARAMS_H_