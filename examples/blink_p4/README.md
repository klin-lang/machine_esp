# blink_p4

ESP32-P4 GPIO blink via [`machine_esp`](../../README.md) MMIO `pin_out_p4`.

Default pin: **GPIO2**. Edit `blink.kl` for your board — skip flash/PSRAM pads.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF’s `app_main` (in `main/app_main.c`) owns the entry point and calls `klin_app_main`.
