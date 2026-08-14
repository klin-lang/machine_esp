# pwm_p4

ESP32-P4 pwm via [`machine_esp`](../../README.md) MMIO `pwm_out_p4` (see `pwm.kl`).

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF's `app_main` owns the entry point and calls `klin_app_main`. Target: `esp32p4`.
