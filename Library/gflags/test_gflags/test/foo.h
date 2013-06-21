#include <iostream>
#include <gflags/gflags.h>

typedef __int32           int32_t; 

//DEFINE宏支持的类型包括：
//o DEFINE_bool: boolean
//o DEFINE_int32: 32-bit integer
//o DEFINE_int64: 64-bit integer
//o DEFINE_uint64: unsigned 64-bit integer
//o DEFINE_double: double
//o DEFINE_string: C++ string

DECLARE_int32(port);
//DEFINE_int32(port);
//DECLARE_string(tryfromenv);   // in gflags.cc

//DEFINE_bool(test_bool, false, "tests bool-ness");
//DEFINE_int32(test_int32, -1, "");
//DEFINE_int64(test_int64, -2, "");
//DEFINE_uint64(test_uint64, 2, "");
//DEFINE_double(test_double, -1.0, "");
//DEFINE_string(test_string, "initial", "");

bool ValidatePort(const char* flagname, int32_t value);