#include "precompiled.h"

IRehldsApi *g_RehldsApi = nullptr;
const RehldsFuncs_t *g_RehldsFuncs = nullptr;
IRehldsHookchains *g_RehldsHookchains = nullptr;

rehlds_ret RehldsApi_Init()
{
	CSysModule *engineModule = Sys_LoadModule(ENGINE_LIB);

	if (!engineModule)
		return RETURN_NOT_FOUND;

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
	if (!ifaceFactory)
		return RETURN_NOT_FOUND;

	int retCode = 0;
	g_RehldsApi = (IRehldsApi *)ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode);

	if (!g_RehldsApi)
	{
		UTIL_LogPrintf("[RR_STATUSMASK] RehldsApi_Init: REHLDS interface not found\n");
		return RETURN_NOT_FOUND;
	}

	const int majorVersion = g_RehldsApi->GetMajorVersion();
	const int minorVersion = g_RehldsApi->GetMinorVersion();

	if (majorVersion != REHLDS_API_VERSION_MAJOR)
	{
		UTIL_LogPrintf("[RR_STATUSMASK] RehldsApi_Init: major mismatch (need %d, got %d)\n",
			REHLDS_API_VERSION_MAJOR, majorVersion);
		return RETURN_MAJOR_MISMATCH;
	}

	if (minorVersion < REHLDS_API_VERSION_MINOR)
	{
		UTIL_LogPrintf("[RR_STATUSMASK] RehldsApi_Init: minor mismatch (need >= %d, got %d)\n",
			REHLDS_API_VERSION_MINOR, minorVersion);
		return RETURN_MINOR_MISMATCH;
	}

	g_RehldsFuncs = g_RehldsApi->GetFuncs();
	g_RehldsHookchains = g_RehldsApi->GetHookchains();

	if (!g_RehldsFuncs || !g_RehldsHookchains)
		return RETURN_NOT_FOUND;

	return RETURN_LOAD;
}
