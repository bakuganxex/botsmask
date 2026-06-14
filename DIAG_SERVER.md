# Диагностика badf load на test-сервере

`badf load` = Metamod **не открыл** `.so` или **не нашёл Meta_Query** (часто: залит ZIP вместо .so, битый FTP, неверный путь).

## 0. Частая ошибка: залит ZIP, а не .so

GitHub Artifacts — это **ZIP-архив**. Сначала распакуй, потом заливай **только** файл `rr_statusmask_mm_i386.so`.

Проверка на сервере:
```bash
head -c 4 addons/rr_statusmask/rr_statusmask_mm_i386.so | od -An -tx1
```
Должно быть: `7f 45 4c 46` (ELF).  
Если `50 4b` — это ZIP, Metamod даст **badf**.

## 1. Проверь файл на сервере

```bash
cd /путь/к/cstrike
ls -la addons/rr_statusmask/rr_statusmask_mm_i386.so
file addons/rr_statusmask/rr_statusmask_mm_i386.so
```

**Должно быть:**
- размер **~16560** байт (не 13816 — это старая битая сборка)
- `ELF 32-bit LSB shared object, Intel 80386`

## 2. Проверь экспорты (главное!)

```bash
nm -D addons/rr_statusmask/rr_statusmask_mm_i386.so | grep Meta_
```

**Должно быть (без `@@` в конце!):**
```text
Meta_Query
Meta_Attach
Meta_Detach
```

Если видишь `Meta_Query@@RR_STATUSMASK_ABI_1.0` — залей **новый** .so с GitHub Actions.

Если `nm: no symbols` или пусто — файл **повреждён при FTP** (заливай Binary, не Auto).

## 3. Проверь plugins.ini

```ini
linux addons/rr_statusmask/rr_statusmask_mm_i386.so
```

Путь относительно `cstrike/`, как у reunion/rechecker.

## 4. Metamod info

```text
meta info 12
```

Пришли полный вывод.

## 5. После замены .so

- **Full restart** HLDS (не changelevel)
- `meta list`

Ожидание: `RR_STATUSMASK RUN` (не badf).

## Скачать правильный .so

https://github.com/bakuganxex/botsmask/actions → последний зелёный run → Artifacts → `rr_statusmask_mm_i386`
