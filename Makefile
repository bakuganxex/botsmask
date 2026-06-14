# rr_statusmask — thin wrapper over compile.sh (Linux i386)
.PHONY: all deps clean

all: deps
	./compile.sh

deps:
	@test -d vendor/metamod || ./fetch_deps.sh

clean:
	rm -rf build .deps_tmp

install-test: all
	cp -f build/rr_statusmask_mm_i386.so ./rr_statusmask_mm_i386.so
	@echo "Copied .so to addons/rr_statusmask/ — add plugins.ini.test_snippet to TEST metamod/plugins.ini"
