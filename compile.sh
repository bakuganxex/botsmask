#!/bin/bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

if [ ! -d vendor/metamod ]; then
	echo "vendor/ missing — run ./fetch_deps.sh first"
	exit 1
fi

rm -rf build
mkdir build
cd build
cmake .. "$@"
make -j"$(nproc 2>/dev/null || echo 2)"
echo
echo "Built: $ROOT/build/rr_statusmask_mm_i386.so"
ls -la rr_statusmask_mm_i386.so
