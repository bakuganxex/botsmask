# Собрать .so онлайн (GitHub Actions)

## Быстрый путь

1. Залей содержимое `rr_statusmask/` в репозиторий GitHub (как `bakuganxxx/botsmask`).
2. Файл **`.github/workflows/main.yml`** — из этого репозитория (уже исправлен).
3. **Actions** → **Build rr_statusmask** → **Run workflow**
4. Скачай **Artifacts** → `rr_statusmask_mm_i386.so`

---

## Ошибка `exit code 127`

**Причина:** скрипты `fetch_deps.sh` / `compile.sh` загружены с Windows — в конце строк **CRLF** (`\r\n`). Linux не находит интерпретатор `/bin/bash\r`.

**Исправление в workflow** (уже в `main.yml`):

```yaml
sed -i 's/\r$//' fetch_deps.sh compile.sh
bash ./fetch_deps.sh
bash ./compile.sh
```

Обнови `main.yml` на GitHub этим содержимым и запусти workflow снова.

---

## Предупреждение Node.js 20

Жёлтое предупреждение про `actions/checkout@v4` — **не причина падения**. Игнорируй до июня 2026.

---

## После успешной сборки

1. Скачай artifact (zip) → `rr_statusmask_mm_i386.so`
2. Залей на **test** в `cstrike/addons/rr_statusmask/`
3. test `plugins.ini`: `linux addons/rr_statusmask/rr_statusmask_mm_i386.so`
4. Full restart → `meta list` → `rr_statusmask_enable 1`

---

## Если снова красный крестик

Открой failed run → шаг **Build .so** → скопируй **весь лог** (последние 30 строк).
