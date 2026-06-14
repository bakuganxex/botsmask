#pragma once

void UTIL_LogPrintf(const char *fmt, ...);
char *UTIL_VarArgs(const char *format, ...);

bool StatusMask_ValidateCommand(IRehldsHook_ValidateCommand *chain, const char *cmd, cmd_source_t src, IGameClient *client);

extern cvar_t g_cv_rr_statusmask_enable;
extern cvar_t g_cv_rr_statusmask_mode;
extern cvar_t g_cv_rr_statusmask_debug;

extern enginefuncs_t g_engfuncs;
extern meta_globals_t *gpMetaGlobals;
extern mutil_funcs_t *gpMetaUtilFuncs;
