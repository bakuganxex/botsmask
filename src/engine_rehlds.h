#pragma once

enum rehlds_ret
{
	RETURN_LOAD,
	RETURN_MINOR_MISMATCH,
	RETURN_MAJOR_MISMATCH,
	RETURN_NOT_FOUND,
};

extern IRehldsApi *g_RehldsApi;
extern const RehldsFuncs_t *g_RehldsFuncs;
extern IRehldsHookchains *g_RehldsHookchains;

rehlds_ret RehldsApi_Init();

#ifndef ENGINE_LIB
	#ifdef _WIN32
		#define ENGINE_LIB "swds"
	#else
		#define ENGINE_LIB "engine_i486"
	#endif
#endif
