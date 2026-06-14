# rr_statusmask PoC — test evidence (fill on TEST server)

**TEST ONLY.** Production не трогать до анализа этого файла.

## Pre-test checklist

- [ ] Stage 1 = **block mode only**. Client видит `[RR_STATUSMASK] command 'status' blocked (PoC block mode)` — для PoC нормально. Stage 2 = masked output без block message.
- [ ] Сборка на Linux i386: `./fetch_deps.sh && make && make install-test`
- [ ] Строка в test `addons/metamod/plugins.ini`: `linux addons/rr_statusmask/rr_statusmask_mm_i386.so`
- [ ] **Full restart** HLDS (не `changelevel`)
- [ ] После старта: `meta list` → `exec addons/rr_statusmask/rr_statusmask.cfg` → `rr_statusmask_enable 1` → `rr_statusmask_debug 1`
- [ ] Заполнить секции 1–7 ниже
- [ ] Production не менять

Date: ___________
Server: ___________ (TEST only)
ReHLDS build: ___________

## 1. Compile result

```text
(paste output of: cd addons/rr_statusmask && make)
```

```text
(paste: ls -la build/rr_statusmask_mm_i386.so)
```

## 2. meta list

```text
(paste server console: meta list)
```

Expected: `RR_STATUSMASK` status `RUN`.

## 3. Startup / attach logs

```text
(paste lines containing [RR_STATUSMASK] from server log on map load)
```

## 4. ValidateCommand logs (rr_statusmask_debug 1, rr_statusmask_enable 1)

### Server console status (PASS)

```text
(paste: type status in SERVER console + [RR_STATUSMASK] PASS log)
```

### Client status (BLOCK)

```text
(paste: Steam client types status + server log BLOCK line)
```

### Client stat (BLOCK)

```text
(paste: Steam client types stat + server log BLOCK line)
```

## 5. Client status BEFORE (enable 0)

```text
(paste client console output after: rr_statusmask_enable 0)
```

Must show `#N "BotName" BOT` if bots present.

## 6. Client status AFTER (enable 1)

```text
(paste client console output after: rr_statusmask_enable 1)
```

Must show block message, NO `BOT` lines.

## 7. GameCMS / A2S (unchanged)

```text
(paste GameCMS player count OR A2S query result — same as before module)
```

## 8. Notes

- Production touched: NO / YES (must be NO)
- Issues:
