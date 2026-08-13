# blink_s3

ESP32-S3 GPIO blink via [`machine_esp`](../../README.md) MMIO `pin_out_s3`.

Default pin: **GPIO2**. Waveshare ESP32-S3-Pico: use **GPIO35** (silkscreen D10)
for a wired LED; onboard WS2812 is **GPIO21** (not this example).

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF’s `app_main` (in `main/app_main.c`) owns the entry point and calls `klin_app_main`.
