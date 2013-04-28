#pragma once

#ifdef SYNCHRONIZERLIB_EXPORTS
#define SYNCHRONIZERLIB_API __declspec(dllexport)
#else
#define SYNCHRONIZERLIB_API __declspec(dllimport)
#endif
