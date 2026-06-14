#!/bin/bash
# Fetch build dependencies (ReHLDS/Metamod headers) from rechecker vendor tree.
# Run once on Linux before compile: ./fetch_deps.sh

set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
VENDOR="$ROOT/vendor"
TMP="$ROOT/.deps_tmp"
ZIP_URL="https://github.com/rehlds/rechecker/archive/refs/heads/master.zip"

echo "[fetch_deps] root=$ROOT"

rm -rf "$TMP"
mkdir -p "$TMP" "$VENDOR"

if ! command -v unzip >/dev/null 2>&1; then
	echo "[fetch_deps] ERROR: unzip not found"
	exit 1
fi

echo "[fetch_deps] downloading rechecker headers..."
curl -fsSL "$ZIP_URL" -o "$TMP/rechecker.zip"
unzip -q "$TMP/rechecker.zip" -d "$TMP"

SRC="$TMP/rechecker-master"
for dir in metamod public engine common dlls pm_shared; do
	echo "[fetch_deps] copy $dir/"
	rm -rf "$VENDOR/$dir"
	cp -a "$SRC/$dir" "$VENDOR/$dir"
done

cp -a "$SRC/public/interface.cpp" "$VENDOR/public/interface.cpp"

rm -rf "$TMP"
echo "[fetch_deps] done -> vendor/{metamod,public,engine,common,dlls,pm_shared}"
