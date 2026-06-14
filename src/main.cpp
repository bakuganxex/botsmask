#include "precompiled.h"

cvar_t g_cv_rr_statusmask_enable = { "rr_statusmask_enable", "0", FCVAR_SERVER };
cvar_t g_cv_rr_statusmask_mode = { "rr_statusmask_mode", "1", FCVAR_SERVER };
cvar_t g_cv_rr_statusmask_debug = { "rr_statusmask_debug", "1", FCVAR_SERVER };

static bool IsStatusLikeCommand(const char *cmd)
{
	if (!cmd || !cmd[0])
		return false;

	return _stricmp(cmd, "status") == 0 || _stricmp(cmd, "stat") == 0;
}

bool StatusMask_ValidateCommand(IRehldsHook_ValidateCommand *chain, const char *cmd, cmd_source_t src, IGameClient *client)
{
	if (!g_cv_rr_statusmask_enable.value)
		return chain->callNext(cmd, src, client);

	// Stage 1: only block mode (1). Mask mode reserved for stage 2.
	if ((int)g_cv_rr_statusmask_mode.value != 1)
		return chain->callNext(cmd, src, client);

	if (!IsStatusLikeCommand(cmd))
		return chain->callNext(cmd, src, client);

	// Server console / RCON / cfg exec — src_command must pass through.
	if (src == src_command)
	{
		if (g_cv_rr_statusmask_debug.value)
		{
			UTIL_LogPrintf("[RR_STATUSMASK] ValidateCommand PASS: cmd=%s src=src_command (server console)\n", cmd);
		}
		return chain->callNext(cmd, src, client);
	}

	// Client path — block before Host_Status_f / Host_Status_Formatted_f.
	if (g_cv_rr_statusmask_debug.value)
	{
		const char *clientName = client ? client->GetName() : "<null>";
		UTIL_LogPrintf("[RR_STATUSMASK] ValidateCommand BLOCK: cmd=%s src=src_client client=%s\n", cmd, clientName);

		if (g_RehldsFuncs)
		{
			g_RehldsFuncs->Log("RR_STATUSMASK", UTIL_VarArgs("BLOCK cmd=%s client=%s", cmd, clientName));
		}
	}

	if (client)
	{
		edict_t *ed = client->GetEdict();
		if (ed)
		{
			static char blockMsg[256];
			snprintf(blockMsg, sizeof(blockMsg),
				"[RR_STATUSMASK] command '%s' blocked (PoC block mode)\n", cmd);
			g_engfuncs.pfnClientPrintf(ed, print_console, blockMsg);
		}
	}

	return false;
}

static void RegisterCvars()
{
	g_engfuncs.pfnCvar_RegisterVariable(&g_cv_rr_statusmask_enable);
	g_engfuncs.pfnCvar_RegisterVariable(&g_cv_rr_statusmask_mode);
	g_engfuncs.pfnCvar_RegisterVariable(&g_cv_rr_statusmask_debug);
}

bool OnMetaAttach()
{
	UTIL_LogPrintf("[RR_STATUSMASK] PoC v0.1.0-poc attach: looking for ReHLDS API...\n");

	const rehlds_ret ret = RehldsApi_Init();
	if (ret != RETURN_LOAD)
	{
		UTIL_LogPrintf("[RR_STATUSMASK] FAILED: ReHLDS API init error code %d (need ReHLDS, not vanilla HLDS)\n", ret);
		return false;
	}

	UTIL_LogPrintf("[RR_STATUSMASK] ReHLDS API OK: major=%d minor=%d\n",
		g_RehldsApi->GetMajorVersion(), g_RehldsApi->GetMinorVersion());

	RegisterCvars();

	g_RehldsHookchains->ValidateCommand()->registerHook(&StatusMask_ValidateCommand);

	UTIL_LogPrintf("[RR_STATUSMASK] ValidateCommand hook registered (block mode only)\n");
	UTIL_LogPrintf("[RR_STATUSMASK] exec addons/rr_statusmask/rr_statusmask.cfg then set rr_statusmask_enable 1\n");

	return true;
}

void OnMetaDetach()
{
	if (g_RehldsHookchains)
	{
		g_RehldsHookchains->ValidateCommand()->unregisterHook(&StatusMask_ValidateCommand);
	}

	UTIL_LogPrintf("[RR_STATUSMASK] detached\n");
}
