# adc2_p4

ESP32-P4 ADC2 via [`machine_esp`](../../README.md) MMIO `adc2_out_p4` (see `adc.kl`).

ADC2 CH0 = **GPIO49**. PWM LED default **GPIO2**. Edit for your board — skip flash/PSRAM pads.

Oneshot is **LP_ADC** (`0x50127000`) MEAS2. Call `read_u16_p4` / `read_u12_p4`, not C3/S3 `read_u16`. IDF pad bit is `channel+2`.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF's `app_main` owns the entry point and calls `klin_app_main`. Target: `esp32p4`.
