#include "stdafx.h"
#include "win32_crtdbg.h"

#if defined _DEBUG && defined _DETECT_MEMORY_LEAK

__dbg_impl::CDebugEnv __dbgEnv;

#endif // _DEBUG && defined _DETECT_MEMORY_LEAK
