# ESP32-P4 ADC1 + regi2c DREF: Klin --emit-c → ESP-IDF build/flash.
#
#   make emit KLIN=/path/to/klin/bin/klin.dart
#   make build
#   make flash

KLIN ?= klin
IDF_PY ?= idf.py

.PHONY: all emit build flash monitor clean

all: build

emit: out/adc.c

out/adc.c out/adc.link: adc.kl $(wildcard ../../machine_esp/*.kl)
	@mkdir -p out
	@case "$(KLIN)" in \
	  *.dart) dart run "$(KLIN)" --emit-c adc.kl ;; \
	  *) "$(KLIN)" --emit-c adc.kl ;; \
	esac
	@sed -i 's/^int main(void)/static int klin_unused_main(void)/' out/adc.c

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
