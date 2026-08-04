# uart_c3

ESP32-C3 UART0 hello + echo via [`machine_esp`](../../README.md) MMIO `Uart`
(TX=GPIO21, RX=GPIO20).

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```
