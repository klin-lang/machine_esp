# machine_esp

ESP32 port of a MicroPython-shaped **`machine`** API for [Klin](https://github.com/klin-lang/klin).

Not a MicroPython port. No GC, no hidden heap. **`Pin` / `Pwm` / `Rc` / `Uart` /
`I2c` / `Spi` / `Adc` / S3 `Rmt` use explicit MMIO** (ESP32-C3 / ESP32-S3 GPIO +
IO_MUX + LEDC + UART/I2C/SPI2/ADC1 + RMT). Boot and flash use **minimal ESP-IDF**
in the examples only — IDF is not part of the Klin package API.

**No hardware DAC** on ESP32-C3 / ESP32-S3 in this package (S3 has DAC pads but
no `Dac` API here).

Decision / catalog: [Klin issue 061](https://github.com/klin-lang/klin/blob/main/issues/061-micropython-machine-api.md),
targets [062](https://github.com/klin-lang/klin/blob/main/issues/062-targets-esp-rp.md),
S3 port [099](https://github.com/klin-lang/klin/blob/main/issues/099-machine-esp-esp32-s3.md).

## Status

| Chip | Pin | Pwm | Rc | Uart | I2c | Spi | Adc | Rmt | Dac | Example | Boot |
|---|---|---|---|---|---|---|---|---|---|---|---|
| **ESP32-C3** | `pin_out` / `pin_in` | `pwm_out` | `rc_out` | `uart_out` | `i2c_out` | `spi_out` | `adc_out` | — | — | `*_c3` | ESP-IDF v5.x |
| **ESP32-S3** | `pin_out_s3` / `pin_in_s3` | `pwm_out_s3` | `rc_out_s3` | `uart_out_s3` | `i2c_out_s3` | `spi_out_s3` | `adc_out_s3` | `rmt_tx_s3` | — | `*_s3` | ESP-IDF v5.x |
| Classic ESP32 (Xtensa), C6 | later | — | — | — | — | — | — | — | — | — |
| Freestanding (no IDF) | later | — | — | — | — | — | — | — | — | — |

C3 target board: **ESP32-C3-DevKitM-1** (onboard LED ≈ **GPIO8**).  
S3 examples use **GPIO2** by default (Waveshare ESP32-S3-Pico D10 → GPIO35; WS2812 on GPIO21 is separate).  
`version()` → `7` (`@v0.7.0`).

### C3 vs S3 (do not mix in one binary)

| Item | C3 | S3 |
|---|---|---|
| Factories | `pin_out`, `pwm_out`, … | `pin_out_s3`, `pwm_out_s3`, … |
| GPIO pads | 0..21 | 0..21, 26..48 (22..25 none) |
| Matrix simple-GPIO | `SIG_GPIO_OUT=128`, OEN bit 9 | `SIG_GPIO_OUT=256`, OEN bit 10 |
| Matrix IN_SEL | bit 6, 5-bit gpio | bit 7, 6-bit gpio |
| GPIO ≥ 32 | n/a | `OUT1` / `ENABLE1` / `IN1` |
| SYSTEM CLK/RST | `+0x10` / `+0x18` | `+0x18` / `+0x20` |
| UART signal | UART0=6, UART1=9 | UART0=12, UART1=15, UART2=18 |
| LEDC out | `45+ch` (ch 0..5) | `73+ch` (ch 0..7) |
| I2C0 SCL/SDA | 53 / 54 | 89 / 90 |
| SPI2 FSPI | 63 / 64 / 65 | 101 / 102 / 103 |
| ADC1 CH→GPIO | CH0..4 → GPIO0..4 | CH0..9 → GPIO1..10 |
| RMT TX | — | `rmt_tx_s3` ch 0..3, SIG_OUT 81+ch, RAM @ `0x60016800` |

Clocks stay **explicit** in factory args (typically APB **80_000_000**, XTAL
**40_000_000** under IDF).

## Requirements

- [Klin](https://github.com/klin-lang/klin) compiler
- For board examples: [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/) **v5.x** (`IDF_PATH` exported)

## Layout

```text
machine_esp/
  version.kl
  pin.kl / pin_s3.kl
  pwm.kl / pwm_s3.kl / rc.kl / rc_s3.kl
  uart.kl / uart_s3.kl / i2c.kl / i2c_s3.kl
  spi.kl / spi_s3.kl / adc.kl / adc_s3.kl
  rmt_s3.kl               # S3 TX only (WS2812-style bit streams)
  *_test.kl
examples/*_c3/           # esp32c3
examples/*_s3/           # esp32s3
```

## Usage — C3

```klin
import "github/klin-lang/machine_esp" machine

let led = machine.pin_out(8)
let pwm = machine.pwm_out(8, 0, 0, 80000000)
let u = machine.uart_out(0, 21, 20, 80000000, 115200)
```

## Usage — S3

```klin
import "github/klin-lang/machine_esp" machine

let led = machine.pin_out_s3(2)          // Waveshare S3-Pico D10 → 35
let pwm = machine.pwm_out_s3(2, 0, 0, 80000000)
let u = machine.uart_out_s3(0, 17, 18, 80000000, 115200)
let bus = machine.i2c_out_s3(0, 8, 9, 40000000, 100000)
let s = machine.spi_out_s3(2, 12, 11, 13, 80000000, 1000000, 0)
let adc = machine.adc_out_s3(1, 0)       // CH0 → GPIO1
let servo = machine.rc_out_s3(2, 0, 0, 80000000, 50, 1000, 2000)
let rmt = machine.rmt_tx_s3(21, 0, 80000000)  // tick = APB/8; put/start/wait_done
```

`uart_out_s3` accepts instance **0 / 1 / 2**. Soft SPI CS via a separate `Pin`.
S3 `rmt_tx_s3` is TX channels **0..=3** only (no DMA / carrier).

## Examples

```sh
. $IDF_PATH/export.sh
cd examples/blink_s3   # or pwm_s3 / uart_s3 / … / blink_c3 / …
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

```sh
klin get github/klin-lang/machine_esp@v0.7.0
```

Wi‑Fi, NVS, and IDF peripheral drivers are **out of scope**.

## Tests

```sh
dart run /path/to/klin/bin/klin.dart test machine_esp/
```

Host tests cover baud/div/base/scale math, S3 pin/UART/ADC maps, and RMT
symbol/mem helpers (no MMIO).

## License

MIT
