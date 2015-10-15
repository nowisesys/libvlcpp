#pragma once

#include "targetver.h"

// 
// Disable warnigs about using unsafe C-function like getenv():
// 
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// 
// Exclude rarely used functions from windows.h:
// 
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// 
// Have setlocale() function:
// 
#define HAVE_SETLOCALE 1

// 
// C++ standard library headers:
// 
#define HAVE_CSTRING 1
#define HAVE_CSTDLIB 1
