# blink_c3

ESP32-C3-DevKitM-1 LED blink via [`machine_esp`](../../README.md) MMIO `Pin`.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF’s `app_main` (in `main/app_main.c`) owns the entry point and calls `klin_app_main`.
