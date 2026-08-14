# adc_cal_p4

ESP32-P4 ADC1 oneshot + SAR **regi2c** DREF via [`machine_esp`](../../README.md) `adc_cal_init_p4`.

Call `adc_cal_init_p4` **before** `adc_out_p4` (not done inside the factory). Analog I2C is **LP_I2C_ANA_MST** `0x50124000`, slave `0x69`. No eFuse / IDF `adc_cali_*`.

ADC1 CH0 = **GPIO16**. PWM LED default **GPIO2**.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF's `app_main` owns the entry point and calls `klin_app_main`. Target: `esp32p4`.
