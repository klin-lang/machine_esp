# machine_esp

ESP32 port of a MicroPython-shaped **`machine`** API for [Klin](https://github.com/klin-lang/klin).

Not a MicroPython port. No GC, no hidden heap. **`Pin` / `Pwm` / `Rc` / `Uart` /
`I2c` / `Spi` / `Adc` use explicit MMIO** (ESP32-C3 / ESP32-S3 GPIO + IO_MUX +
LEDC + UART/I2C/SPI2/ADC1). Boot and flash use **minimal ESP-IDF** in the
examples only — IDF is not part of the Klin package API.

**No hardware DAC** on ESP32-C3 — there is no `Dac` API in this package.

Decision / catalog: [Klin issue 061](https://github.com/klin-lang/klin/blob/main/issues/061-micropython-machine-api.md),
targets [062](https://github.com/klin-lang/klin/blob/main/issues/062-targets-esp-rp.md),
S3 port [099](https://github.com/klin-lang/klin/blob/main/issues/099-machine-esp-esp32-s3.md).

## Status

| Chip | Pin | Pwm | Rc | Uart | I2c | Spi | Adc | Dac | Example | Boot |
|---|---|---|---|---|---|---|---|---|---|---|
| **ESP32-C3** | `pin_out` / `pin_in` | `pwm_out` | `rc_out` | `uart_out` | `i2c_out` | `spi_out` | `adc_out` | — (none on C3) | `blink_c3`, `pwm_c3`, `rc_c3`, `uart_c3`, `i2c_c3`, `spi_c3`, `adc_c3` | ESP-IDF v5.x |
| **ESP32-S3** | `pin_out_s3` / `pin_in_s3` | later (`*_s3`) | later | later | later | later | later | — | `blink_s3` | ESP-IDF v5.x |
| Classic ESP32 (Xtensa), C6 | later | — | — | — | — | — | — | — | — | — |
| Freestanding (no IDF) | later | — | — | — | — | — | — | — | — | — |

C3 target board: **ESP32-C3-DevKitM-1** (onboard LED ≈ **GPIO8**).  
S3 examples use **GPIO2** by default (Waveshare ESP32-S3-Pico D10 → GPIO35; WS2812 on GPIO21 is separate).  
`version()` → `5` (`@v0.5.0`).

S3 vs C3 (Pin): same MMIO bases (`GPIO`/`IO_MUX` at `0x60004…` / `0x60009…`), different **GPIO matrix** encoding (`SIG_GPIO_OUT=256`, OEN bit 10, IN_SEL bit 7 / 6-bit gpio), **bank1** for GPIO ≥ 32, and pad list **0..21 + 26..48**. Do not mix C3 `pin_out` with S3 silicon.

## Requirements

- [Klin](https://github.com/klin-lang/klin) compiler
- For board examples: [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/) **v5.x** (`IDF_PATH` exported)

## Layout

```text
machine_esp/
  version.kl
  pin.kl / pin_s3.kl / pwm.kl / rc.kl
  uart.kl / i2c.kl / spi.kl / adc.kl
  *_test.kl              # host math only; skipped on import
examples/blink_c3/       # C3 Pin toggle
examples/blink_s3/       # S3 Pin toggle (pin_out_s3)
examples/pwm_c3/         # LEDC fade on GPIO8
examples/rc_c3/          # LEDC RC/servo sweep on GPIO8
examples/uart_c3/        # UART0 TX21/RX20 hello + echo
examples/i2c_c3/         # I2C0 SDA8/SCL9 init + GPIO2 blink
examples/spi_c3/         # SPI2 SCK6/MOSI7/MISO2 clock out
examples/adc_c3/         # ADC1 CH0 GPIO0 → PWM GPIO8
```

## Usage — Pin / Pwm / Rc (C3)

```klin
import "github/klin-lang/machine_esp" machine

let led = machine.pin_out(8)
let pwm = machine.pwm_out(8, 0, 0, 80000000)
let servo = machine.rc_out(8, 0, 0, 80000000, 50, 1000, 2000)
```

## Usage — Pin (S3)

```klin
import "github/klin-lang/machine_esp" machine

let led = machine.pin_out_s3(2)   // Waveshare S3-Pico D10 → 35
```

Clocks (APB / XTAL) stay **explicit** in factory args for bus APIs — same as C3
(typically APB **80_000_000**, XTAL **40_000_000** under IDF).

## Usage — Uart

Caller passes instance (0/1), TX/RX GPIO, APB clock Hz, and baud. 8N1.

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn uart_main() {
    let u = machine.uart_out(0, 21, 20, 80000000, 115200)
    u.write_u8(65)
    if u.any() {
        let b = u.read_u8()
        u.write_u8(b)
    }
}
```

## Usage — I2c

I2C0 only (`i2c` arg kept for shape parity). SDA/SCL GPIO, source clock
(typically XTAL **40_000_000**), and bus `freq_hz`. Open-drain + pull-up.
7-bit addresses. Blocking transfers.

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn i2c_main() {
    let bus = machine.i2c_out(0, 8, 9, 40000000, 100000)
    let mut w: [1]u8
    w[0] = 0x00
    bus.writeto(0x50, w)
    let mut r: [2]u8
    bus.readfrom_into(0x50, r)
    bus.write_readfrom_into(0x50, 0x00, r)
}
```

## Usage — Spi

SPI2 (GPSPI) only. Soft CS — drive chip-select with a separate `Pin`.
`mode` is 0..=3.

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn spi_main() {
    let s = machine.spi_out(2, 6, 7, 2, 80000000, 1000000, 0)
    let cs = machine.pin_out(10)
    cs.low()
    let v = s.write_read_u8(0x9F)
    cs.high()
}
```

## Usage — Adc

ADC1 only. Channels **0..=4** map to **GPIO0..=4** on C3. Pass GPIO + channel
explicitly. **No DAC** on this chip.

```klin
import "github/klin-lang/machine_esp" machine

@[cexport, codename("klin_app_main")]
fn adc_main() {
    let adc = machine.adc_out(0, 0)
    let raw = adc.read_u12()   // 0..=4095
    let u16 = adc.read_u16()   // 0..=65535
}
```

```sh
klin get github/klin-lang/machine_esp@v0.5.0
```

## Shape (shared with other `machine_*`)

| Piece | Role |
|---|---|
| `*_out(…)` / `*_s3(…)` | factory — chip-specific args OK; clocks explicit |
| `deinit()` | stop peripheral (explicit) |
| `writeto` / `readfrom_into` | I2C into caller buffers (no heap) |
| `write_read_u8` / `readinto` | SPI full-duplex / fill buffer |
| `read_u12` / `read_u16` | ADC raw / MicroPython-scaled |

## Examples

```sh
. $IDF_PATH/export.sh
cd examples/blink_c3   # or blink_s3 / pwm_c3 / …
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

Wi‑Fi, NVS, and IDF `gpio_*` / `ledc_*` / `uart_*` / `i2c_*` / `spi_*` /
`adc_*` drivers are **out of scope** for this MVP.

## Tests

```sh
dart run /path/to/klin/bin/klin.dart test machine_esp/
```

Host tests cover baud/div/base/scale math and S3 pin bank math only (no MMIO).

## License

MIT
