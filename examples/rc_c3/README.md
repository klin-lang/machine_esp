# rc_c3 — ESP32-C3 RC/servo sweep (GPIO8)

Uses `machine_esp.rc_out` (LEDC). Boot via minimal ESP-IDF; Klin emits `out/rc.c`.

```sh
make emit KLIN=/path/to/klin/bin/klin.dart
make build   # requires IDF_PATH
make flash
```
