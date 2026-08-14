# uart_lp_p4

ESP32-P4 LP UART via [`machine_esp`](../../README.md) MMIO `uart_out_lp_p4` (see `uart.kl`).

Default pins: **LP GPIO14** (TX) / **LP GPIO15** (RX) — IDF `LP_U0TXD` / `LP_U0RXD`.
Sclk is LPPERI **XTAL_D2** (pass `20000000`). Returns `UartLp`, not HP `Uart`.
Do not also call `uart_out_p4` or `pin_out_lp_p4` on those pads.

## Build

```sh
. $IDF_PATH/export.sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

`make emit` runs Klin `--emit-c` and rewrites `main` to a static stub so ESP-IDF's `app_main` owns the entry point and calls `klin_app_main`. Target: `esp32p4`.
