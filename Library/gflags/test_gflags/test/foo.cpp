#include "stdafx.h"
#include "foo.h"

DEFINE_int32(port, 0, "What port to listen on");

bool ValidatePort(const char* flagname, int32_t value) {
    if (value > 0 && value<32768)   // value is ok
        return true;
    printf("Invalid value for --%s: %d\n", flagname, (int)value);
    return false;
}