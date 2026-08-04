# spi_c3

ESP32-C3 SPI2 master clock-out via [`machine_esp`](../../README.md) MMIO `Spi`
(SCK=GPIO6, MOSI=GPIO7, MISO=GPIO2). Soft CS.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```
