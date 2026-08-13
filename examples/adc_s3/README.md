# adc_s3

ESP32-S3 adc via [`machine_esp`](../../README.md) MMIO `adc_out_s3` (see `adc.kl`).

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF's `app_main` owns the entry point and calls `klin_app_main`. Target: `esp32s3`.
