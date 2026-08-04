# i2c_c3

ESP32-C3 I2C0 init (SDA=GPIO8, SCL=GPIO9) via [`machine_esp`](../../README.md)
MMIO `I2c`, then blinks GPIO2.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```
