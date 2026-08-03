# pwm_c3

ESP32-C3-DevKitM-1 LED fade via [`machine_esp`](../../README.md) MMIO `Pwm` (LEDC).

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`pwm_out(8, 0, 0, 80000000)` — GPIO8, LEDC channel 0, timer 0, APB 80 MHz.
