# blink_lp_p4

ESP32-P4 LP GPIO blink via [`machine_esp`](../../README.md) MMIO `pin_out_lp_p4`.

Default pin: **LP GPIO2** (same pad as HP GPIO2). Do not also call `pin_out_p4` on that pad. Edit for your board.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF's `app_main` owns the entry point and calls `klin_app_main`. Target: `esp32p4`.
