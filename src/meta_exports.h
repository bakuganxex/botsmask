#pragma once

// Metamod dlsym() requires unmangled C exports.
#ifdef __cplusplus
extern "C" {
#endif

int Meta_Query(char *interfaceVersion, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs);
int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs);
int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
void GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals);

#ifdef __cplusplus
}
#endif
