#pragma message ("WHILE_INDEX = " TO_STRING(WHILE_INDEX))

#ifndef WHILE_INDEX_NEW
#define WHILE_INDEX_NEW WHILE_INDEX
#endif

#if WHILE_INDEX_NEW < WHILE_INDEX_END
#pragma message (TO_STRING(WHILE_INDEX < WHILE_INDEX_END))
#define WHILE_INDEX_NEW (WHILE_INDEX + 1)
#pragma message ("XXXX = " TO_STRING(WHILE_INDEX_NEW))
#include "macro_while.h"
#endif
