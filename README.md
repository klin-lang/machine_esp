# machine_esp

ESP32 port of a MicroPython-shaped **`machine`** API for [Klin](https://github.com/klin-lang/klin).

Not a MicroPython port. No GC, no hidden heap. **`Pin` uses explicit MMIO** (ESP32-C3
GPIO + IO_MUX). Boot and flash use **minimal ESP-IDF** in the example only — IDF is
not part of the Klin package.

Decision / catalog: [Klin issue 061](https://github.com/klin-lang/klin/blob/main/issues/061-micropython-machine-api.md),
targets [062](https://github.com/klin-lang/klin/blob/main/issues/062-targets-esp-rp.md).

## Status

| Chip | Pin API | Example | Boot |
|---|---|---|---|
| **ESP32-C3** | `pin_out` / `pin_in` | `examples/blink_c3` | ESP-IDF v5.x (`idf.py`) |
| Classic ESP32 (Xtensa), C6, S3 | later | — | — |
| Freestanding (no IDF) | later | — | — |

Target board for the blink example: **ESP32-C3-DevKitM-1** (onboard LED ≈ **GPIO8**).

## Requirements

- [Klin](https://github.com/klin-lang/klin) compiler (`klin` or `dart run path/to/bin/klin.dart`)
- For the blink example: [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/) **v5.x** with `esp32c3` target (`IDF_PATH` exported)

## Layout

```text
machine_esp/             # module machine_esp (directory package)
  version.kl
  pin.kl                 # ESP32-C3 MMIO Pin
  pin_test.kl            # skipped on import
examples/blink_c3/       # DevKitM-1 GPIO8 + minimal IDF app
```

## Usage

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn blink_main() {
    let led = machine.pin_out(8)
    led.toggle()
}

fn main() {
    blink_main()
}
```

```sh
klin get github/klin-lang/machine_esp@v0.1.0
```

Local / in-repo example:

```klin
import "../../machine_esp" machine
```

## Blink example (ESP-IDF)

```sh
# once per shell
. $IDF_PATH/export.sh

cd examples/blink_c3
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
# make monitor
```

Flow:

1. `make emit` — Klin `--emit-c` → `out/blink.c` (exports `klin_app_main`)
2. `idf.py` links that C with `main/app_main.c` (calls `klin_app_main`)
3. Flash via IDF / `esptool` under the hood

Wi‑Fi, NVS, and IDF `gpio_*` drivers are **out of scope** for this MVP.

## Tests

Host-safe package tests (no MMIO):

```sh
dart run /path/to/klin/bin/klin.dart test machine_esp/
```

## License

MIT
