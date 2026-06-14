#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "basetypes.h"
#include "archtypes.h"

#include <extdll.h>
#include <enginecallback.h>

#include <meta_api.h>
#include <h_export.h>

#include "rehlds_api.h"
#include "engine_rehlds.h"
#include "main.h"

#undef DLLEXPORT

#ifdef _WIN32
	#define DLLEXPORT __declspec(dllexport)
	#define NOINLINE __declspec(noinline)
	#define NORETURN __declspec(noreturn)
#else
	#define DLLEXPORT __attribute__((visibility("default")))
	#define NOINLINE __attribute__((noinline))
	#define NORETURN __attribute__((noreturn))
	#define WINAPI
#endif
