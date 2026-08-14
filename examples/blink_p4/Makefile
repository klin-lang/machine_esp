# ESP32-P4 blink: Klin --emit-c → ESP-IDF build/flash.
#
# Requires:
#   - Klin (dart run path/to/bin/klin.dart or `klin` on PATH)
#   - ESP-IDF v5.x with esp32p4 support (`export.sh`)
#
#   make emit KLIN=/path/to/klin/bin/klin.dart
#   make build
#   make flash

KLIN ?= klin
IDF_PY ?= idf.py

.PHONY: all emit build flash monitor clean

all: build

emit: out/blink.c

out/blink.c out/blink.link: blink.kl $(wildcard ../../machine_esp/*.kl)
	@mkdir -p out
	@case "$(KLIN)" in \
	  *.dart) dart run "$(KLIN)" --emit-c blink.kl ;; \
	  *) "$(KLIN)" --emit-c blink.kl ;; \
	esac
	# Avoid clashing with IDF entry (app_main); keep Klin main as unused static.
	@sed -i 's/^int main(void)/static int klin_unused_main(void)/' out/blink.c

build: emit
	@test -n "$$IDF_PATH" || (echo "IDF_PATH not set — source ESP-IDF export.sh" >&2; exit 1)
	$(IDF_PY) set-target esp32p4
	$(IDF_PY) build

flash: build
	$(IDF_PY) flash

monitor:
	$(IDF_PY) monitor

clean:
	rm -rf out build
	-$(IDF_PY) fullclean 2>/dev/null || true
