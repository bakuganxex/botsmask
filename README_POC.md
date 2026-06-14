# rr_statusmask — PoC stage 1 (block mode)

ReHLDS API Metamod module. Blocks **client** `status` and `stat` via `ValidateCommand` hook so `Host_Status_f` / `Host_Status_Formatted_f` never run for connected players.

**NOT AMXX.** Does not touch A2S/GameCMS/YaPB query hook.

**TEST SERVER ONLY.** Do not deploy to production `plugins.ini` without explicit approval.

---

## What stage 1 does

| Check | Behavior |
|-------|----------|
| Metamod load | `meta list` shows `RR_STATUSMASK RUN` |
| ReHLDS API | Log: `ReHLDS API OK: major=3 minor=...` |
| Client `status` | Blocked — no `#N "Bot" BOT` lines |
| Client `stat` | Blocked (formatted status bypass) |
| Server console `status` | Allowed (`src_command`) |
| RCON `status` | Allowed (`src_command`) |
| `stats` | **Not** blocked (different command — server CPU table) |
| A2S / GameCMS | Unchanged (no query hooks) |

## Cvars

```
rr_statusmask_enable "0"   // 0=off, 1=on
rr_statusmask_mode "1"     // 1=block only (PoC); 2=mask — not implemented
rr_statusmask_debug "1"    // log ValidateCommand to server console + rehlds log
```

## Source layout

```
addons/rr_statusmask/
  src/
    main.cpp           — ValidateCommand hook, cvars, attach/detach
    meta_api.cpp       — Metamod Meta_Query/Attach/Detach
    engine_rehlds.cpp  — ReHLDS API init (Sys_LoadModule engine_i486)
    h_export.cpp       — GiveFnptrsToDll
    sdk_util.cpp       — logging helpers
  fetch_deps.sh        — downloads rechecker vendor headers (one-time)
  compile.sh / Makefile
  CMakeLists.txt
  rr_statusmask.cfg
  plugins.ini.test_snippet
```

## Build (Linux i386, on test server or build host)

**Пошагово на русском (для новичков):** см. [`BUILD_RU.md`](BUILD_RU.md)

`.so` в репозитории/zip **нет** — появляется только после сборки на Linux.

Requirements: `g++-multilib` (or `g++` with `-m32`), `cmake`, `curl`, `unzip`, `make`.

```bash
cd addons/rr_statusmask
chmod +x fetch_deps.sh compile.sh
./fetch_deps.sh          # vendor/metamod, engine/rehlds_api.h, etc.
make                     # -> build/rr_statusmask_mm_i386.so
make install-test        # copies .so next to cfg
```

Expected compile output ends with:

```
Built: .../build/rr_statusmask_mm_i386.so
-rwxr-xr-x 1 user user XXXXX ... rr_statusmask_mm_i386.so
```

## Test deploy (test server only)

1. Copy folder `addons/rr_statusmask/` to test server `cstrike/addons/rr_statusmask/`.
2. Ensure `rr_statusmask_mm_i386.so` is present (from build).
3. Add line from `plugins.ini.test_snippet` to **test** `addons/metamod/plugins.ini`:
   ```
   linux addons/rr_statusmask/rr_statusmask_mm_i386.so
   ```
4. Full restart test HLDS (not `changelevel`).
5. In server console:
   ```
   meta list
   exec addons/rr_statusmask/rr_statusmask.cfg
   rr_statusmask_enable 1
   ```

## Verification procedure

### 1. meta list

Expect row like:

```
[ N] RR_STATUSMASK   RUN   -   rr_statusmask_mm_i386.so   0.1.0-poc   ini   Start ANY
```

If `fail load` — check ReHLDS engine (`engine_i486.so`), missing `.so`, or wrong arch (need i386).

### 2. Startup logs

```
[RR_STATUSMASK] PoC v0.1.0-poc attach: looking for ReHLDS API...
[RR_STATUSMASK] ReHLDS API OK: major=3 minor=15
[RR_STATUSMASK] ValidateCommand hook registered (block mode only)
```

### 3. Server console status (must work)

Type `status` in **server** console. Full player list including bots must appear.

With `rr_statusmask_debug 1`:

```
[RR_STATUSMASK] ValidateCommand PASS: cmd=status src=src_command (server console)
```

### 4. Client status BEFORE enable

`rr_statusmask_enable 0` — Steam client console `status` shows engine output:

```
# 1 "PlayerName" STEAM_0:x:xxxxx ...
# 2 "BotName" BOT
```

### 5. Client status AFTER enable

`rr_statusmask_enable 1` — Steam client console `status`:

```
[RR_STATUSMASK] command 'status' blocked (PoC block mode)
```

No `#N "BotName" BOT` lines.

Server log:

```
[RR_STATUSMASK] ValidateCommand BLOCK: cmd=status src=src_client client=PlayerName
```

### 6. Client `stat` (bypass check)

Same block message; log shows `cmd=stat`.

### 7. GameCMS / A2S

Query test server via GameCMS monitor or `A2S_PLAYER` — player count and bot masking unchanged (YaPB hook only).

---

## Architecture

```
Client types "status"
  Cmd_ExecuteString(text, src_client)
    ValidateCommand hook  <-- rr_statusmask returns false
    (Host_Status_f NOT called)

Server console types "status"
  Cmd_ExecuteString(text, src_command)
    ValidateCommand hook  <-- rr_statusmask returns callNext(true)
    Host_Status_f() -> Con_Printf
```

## Stage 2 (not in this PoC)

- `rr_statusmask_mode 2` — fake masked status output
- admin bypass, fake SteamID/IP, AMXX bridge

## Files to fill after test

See `TEST_EVIDENCE.md` — paste real `meta list`, logs, and client before/after screenshots.
