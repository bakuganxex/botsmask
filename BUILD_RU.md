# Как собрать rr_statusmask_mm_i386.so (с нуля)

`.so` **не лежит в zip** — его нужно один раз собрать на **Linux-сервере**, где уже крутится ваш **test** CS 1.6 + ReHLDS.

С Windows на сервер только **заливаете папку** `rr_statusmask`, дальше — команды в SSH/консоли хостинга.

---

## Шаг 0. Что должно быть

- **Test-сервер** CS 1.6 с **ReHLDS** (не vanilla HLDS).
- Доступ по **SSH** или **веб-консоль** хостинга (терминал Linux).
- Папка `addons/rr_statusmask/` уже на сервере, например:
  ```
  /home/user/server/cstrike/addons/rr_statusmask/
  ```
  (путь у каждого свой — подставьте свой.)

---

## Шаг 1. Залить файлы на test-сервер (если ещё не залили)

С **Windows**:

1. Откройте FTP / файловый менеджер хостинга (FileZilla, панель и т.д.).
2. Зайдите в папку мода сервера, обычно `cstrike/addons/`.
3. Создайте папку `rr_statusmask`.
4. Залейте **всё** из вашего `rr_statusmask.zip` / репозитория:
   - `src/`, `Makefile`, `CMakeLists.txt`, `fetch_deps.sh`, `compile.sh`, `rr_statusmask.cfg`, …

`.so` заливать не нужно — он появится после сборки.

---

## Шаг 2. Открыть терминал на сервере

- **SSH** (PuTTY, Windows Terminal): `ssh user@IP_сервера`
- или **консоль** в панели хостинга (VDSina, Timeweb, и т.д.)

Перейдите в папку модуля (путь замените на свой):

```bash
cd /home/ВАШ_ЮЗЕР/server/cstrike/addons/rr_statusmask
pwd
```

Должны увидеть файлы `Makefile`, `fetch_deps.sh`, папку `src/`.

---

## Шаг 3. Установить инструменты сборки (один раз)

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y build-essential g++-multilib gcc-multilib cmake curl unzip make
```

### CentOS / Alma / Rocky

```bash
sudo yum groupinstall -y "Development Tools"
sudo yum install -y gcc-c++ glibc-devel.i686 libstdc++-devel.i686 cmake curl unzip make
```

Если `sudo` нет — попросите поддержку хостинга установить эти пакеты или соберите на другой Linux-машине с `-m32` и скопируйте только `.so`.

---

## Шаг 4. Сборка (копируйте по очереди)

```bash
cd /home/ВАШ_ЮЗЕР/server/cstrike/addons/rr_statusmask

chmod +x fetch_deps.sh compile.sh

./fetch_deps.sh
```

Должно закончиться строкой вроде:
```text
[fetch_deps] done -> vendor/{metamod,public,engine,...}
```

Дальше:

```bash
make
```

Успех — в конце что-то вроде:
```text
Built: .../build/rr_statusmask_mm_i386.so
-rwxr-xr-x ... rr_statusmask_mm_i386.so
```

Скопировать `.so` рядом с cfg:

```bash
make install-test
ls -la rr_statusmask_mm_i386.so
```

Файл **`rr_statusmask_mm_i386.so`** должен лежать в `addons/rr_statusmask/`.

---

## Шаг 5. Подключить на test (не production!)

1. Откройте **test** `cstrike/addons/metamod/plugins.ini`.
2. Добавьте строку (можно после reunion):
   ```ini
   linux addons/rr_statusmask/rr_statusmask_mm_i386.so
   ```
3. **Полный restart** сервера (не `changelevel`).
4. В консоли сервера:
   ```text
   meta list
   exec addons/rr_statusmask/rr_statusmask.cfg
   rr_statusmask_enable 1
   rr_statusmask_debug 1
   ```

В `meta list` должно быть: `RR_STATUSMASK` → `RUN`.

---

## Частые ошибки

| Ошибка | Что делать |
|--------|------------|
| `g++: error: unrecognized command line option '-m32'` | Не установлен multilib: `apt install g++-multilib` |
| `cmake: command not found` | `apt install cmake` |
| `vendor/ missing` | Сначала `./fetch_deps.sh` |
| `meta list` → `fail load` | Нет ReHLDS или `.so` не i386 / не в той папке |
| `curl: (6) Could not resolve host` | Нет интернета на сервере — vendor можно залить с ПК (см. ниже) |

---

## Если на сервере нет интернета

На **Windows** с интернетом:

1. Скачайте https://github.com/rehlds/rechecker/archive/refs/heads/master.zip
2. Распакуйте, скопируйте папки `metamod`, `public`, `engine`, `common`, `dlls`, `pm_shared` в `rr_statusmask/vendor/` на сервере.
3. Скопируйте `rechecker-master/public/interface.cpp` → `vendor/public/interface.cpp`
4. На сервере только: `make && make install-test`

---

## Если совсем не получается

1. Скиньте **полный текст ошибки** из терминала после `make`.
2. Или попросите того, кто админит test Linux/VDS, выполнить 4 команды из шага 4 — это 2–3 минуты.

После успешной сборки заполните `TEST_EVIDENCE.md`.
