#pragma once
#define ICONV_LIB_NAME "iconv"

#if defined(ICONV_LIB_DYNAMIC)
#  define ICONV_LIB_PREFIX
#elif defined(_WIN32)
#  define ICONV_LIB_PREFIX "lib"
#endif

#if defined(_WIN64)
#  define ICONV_LIB_PLATFORM "-x64"
#elif defined(_WIN32)
#  define ICONV_LIB_PLATFORM "-x86"
#endif

#ifdef _DLL
#  if defined(_DEBUG)
// MDd
#    define ICONV_LIB_RT_OPT "-gd"
#  else
// MD
#    define ICONV_LIB_RT_OPT
#  endif
#else
#  if defined(_DEBUG)
// MTd
#    define ICONV_LIB_RT_OPT "-sgd"
#  else
// MT
#    define ICONV_LIB_RT_OPT "-s"
#  endif
#endif

#  pragma comment(lib, ICONV_LIB_PREFIX ICONV_LIB_NAME  ICONV_LIB_RT_OPT ICONV_LIB_PLATFORM ".lib")
// static MDd x86 : libiconv-gd-x86.lib
// static MD x86 :  libiconv-x86.lib
// static MTd x86 : libiconv-sgd-x86.lib
// static MT x86 :  libiconv-s-x86.lib
