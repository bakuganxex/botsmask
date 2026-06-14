#include "precompiled.h"
#include "meta_exports.h"

plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,
	"RR_STATUSMASK",
	"0.1.0-poc",
	__DATE__,
	"RR-GAME",
	"PoC: block client status/stat via ReHLDS ValidateCommand",
	"RR_STATUSMASK PoC",
	PT_STARTUP,
	PT_ANYTIME,
};

meta_globals_t *gpMetaGlobals = nullptr;
gamedll_funcs_t *gpGamedllFuncs = nullptr;
mutil_funcs_t *gpMetaUtilFuncs = nullptr;

META_FUNCTIONS gMetaFunctionTable = { nullptr };

extern bool OnMetaAttach();
extern void OnMetaDetach();

C_DLLEXPORT int Meta_Query(char *interfaceVersion, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs)
{
	if (strcmp(interfaceVersion, META_INTERFACE_VERSION) != 0)
	{
		*pPlugInfo = nullptr;
		return FALSE;
	}

	*pPlugInfo = &Plugin_info;
	gpMetaUtilFuncs = pMetaUtilFuncs;
	return TRUE;
}

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	gpMetaGlobals = pMGlobals;
	gpGamedllFuncs = pGamedllFuncs;

	if (!OnMetaAttach())
		return FALSE;

	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	OnMetaDetach();
	return TRUE;
}
