# machine_esp

ESP32 port of a MicroPython-shaped **`machine`** API for [Klin](https://github.com/klin-lang/klin).

Not a MicroPython port. No GC, no hidden heap. **`Pin` / `Pwm` / `Rc` use explicit MMIO**
(ESP32-C3 GPIO + IO_MUX + LEDC). Boot and flash use **minimal ESP-IDF** in the
examples only — IDF is not part of the Klin package API.

Decision / catalog: [Klin issue 061](https://github.com/klin-lang/klin/blob/main/issues/061-micropython-machine-api.md),
targets [062](https://github.com/klin-lang/klin/blob/main/issues/062-targets-esp-rp.md).

## Status

| Chip | Pin | Pwm | Rc | Example | Boot |
|---|---|---|---|---|---|
| **ESP32-C3** | `pin_out` / `pin_in` | `pwm_out` | `rc_out` | `blink_c3`, `pwm_c3`, `rc_c3` | ESP-IDF v5.x |
| Classic ESP32 (Xtensa), C6, S3 | later | — | — | — | — |
| Freestanding (no IDF) | later | — | — | — | — |

Target board: **ESP32-C3-DevKitM-1** (onboard LED ≈ **GPIO8**).  
`version()` → `3` (`@v0.3.0`).

## Requirements

- [Klin](https://github.com/klin-lang/klin) compiler
- For board examples: [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/) **v5.x** (`IDF_PATH` exported)

## Layout

```text
machine_esp/
  version.kl
  pin.kl
  pwm.kl                 # LEDC MMIO
  rc.kl
  pin_test.kl / rc_test.kl
examples/blink_c3/       # Pin toggle
examples/pwm_c3/         # LEDC fade on GPIO8
examples/rc_c3/          # LEDC RC/servo sweep on GPIO8
```

## Usage — Pin

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn blink_main() {
    let led = machine.pin_out(8)
    led.toggle()
}
```

## Usage — Pwm

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn pwm_main() {
    let led = machine.pwm_out(8, 0, 0, 80000000)
    led.freq(1000)
    led.duty_u16(32768)
}
```

## Usage — Rc (servo / RC pulse)

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn rc_main() {
    let servo = machine.rc_out(8, 0, 0, 80000000, 50, 1000, 2000)
    servo.out(50000, 0)
    servo.out_f32(0.25, 0)
    servo.pulse_us(1500)
}
```

```sh
klin get github/klin-lang/machine_esp@v0.3.0
```

## Pwm / Rc shape (shared with other `machine_*`)

| Piece | Role |
|---|---|
| `pwm_out(gpio, ch, tim, ledc_clk_hz)` | factory — LEDC channel/timer explicit |
| `freq` / `duty_u16` / `deinit` | PWM |
| `rc_out(…, freq_hz, us_min, us_max)` | servo/RC |
| `out` / `out_f32` / `pulse_us` / `deinit` | position + trim / raw µs |

## Examples

```sh
. $IDF_PATH/export.sh
cd examples/blink_c3   # or examples/pwm_c3 / examples/rc_c3
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

Wi‑Fi, NVS, and IDF `gpio_*` / `ledc_*` drivers are **out of scope** for this MVP.

## Tests

```sh
dart run /path/to/klin/bin/klin.dart test machine_esp/
```

## License

MIT
