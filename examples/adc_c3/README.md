# adc_c3

ESP32-C3 ADC1 CH0 (GPIO0) → LEDC PWM on GPIO8 via [`machine_esp`](../../README.md).

**No hardware DAC** on ESP32-C3.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```
