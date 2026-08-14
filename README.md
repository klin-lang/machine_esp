# machine_esp

ESP32 port of a MicroPython-shaped **`machine`** API for [Klin](https://github.com/klin-lang/klin).

Not a MicroPython port. No GC, no hidden heap. **`Pin` / `Pwm` / `Rc` / `Uart` /
`I2c` / `Spi` / `Adc` / S3 `Rmt` use explicit MMIO** (ESP32-C3 / ESP32-S3 /
ESP32-P4 GPIO + IO_MUX + LEDC + UART/I2C/SPI2/ADC1 + RMT). Boot and flash use
**minimal ESP-IDF** in the examples only — IDF is not part of the Klin package API.

**No hardware DAC** on ESP32-C3 / ESP32-S3 / ESP32-P4 in this package.

Decision / catalog: [Klin issue 061](https://github.com/klin-lang/klin/blob/main/issues/061-micropython-machine-api.md),
targets [062](https://github.com/klin-lang/klin/blob/main/issues/062-targets-esp-rp.md),
S3 port [099](https://github.com/klin-lang/klin/blob/main/issues/099-machine-esp-esp32-s3.md),
P4 Pin…Adc+Rmt [114](https://github.com/klin-lang/klin/blob/main/issues/114-machine-esp-esp32-p4.md).

## Status

| Chip | Pin | Pwm | Rc | Uart | I2c | Spi | Adc | Rmt | Dac | Example | Boot |
|---|---|---|---|---|---|---|---|---|---|---|---|
| **ESP32-C3** | `pin_out` / `pin_in` | `pwm_out` | `rc_out` | `uart_out` | `i2c_out` | `spi_out` | `adc_out` | — | — | `*_c3` | ESP-IDF v5.x |
| **ESP32-S3** | `pin_out_s3` / `pin_in_s3` | `pwm_out_s3` | `rc_out_s3` | `uart_out_s3` | `i2c_out_s3` | `spi_out_s3` | `adc_out_s3` | `rmt_tx_s3` | — | `*_s3` | ESP-IDF v5.x |
| **ESP32-P4** | `pin_out_p4` / `pin_in_p4` | `pwm_out_p4` | `rc_out_p4` | `uart_out_p4` | `i2c_out_p4` | `spi_out_p4` | `adc_out_p4` / `adc2_out_p4` | `rmt_tx_p4` | — | `*_p4` | ESP-IDF v5.x |
| Classic ESP32 (Xtensa), C6 | later | — | — | — | — | — | — | — | — | — |
| Freestanding (no IDF) | later | — | — | — | — | — | — | — | — | — |

C3 target board: **ESP32-C3-DevKitM-1** (onboard LED ≈ **GPIO8**).  
S3 examples use **GPIO2** by default (Waveshare ESP32-S3-Pico D10 → GPIO35; WS2812 on GPIO21 is separate).  
P4 examples use **GPIO2** by default (edit for your LED; do not reuse flash/PSRAM pads).  
`version()` → `12` (`@v0.12.0`). P4 ADC oneshot is **LP_ADC** `0x50127000` — call **`read_u12_p4`**, not C3/S3 `read_u12`. ADC2: `adc2_out_p4` (CH0→GPIO49).

### C3 vs S3 vs P4 (do not mix families in one binary)

| Item | C3 | S3 | P4 |
|---|---|---|---|
| Factories | `pin_out`, `pwm_out`, … | `pin_out_s3`, … | `pin_out_p4`, `pwm_out_p4`, … |
| GPIO pads | 0..21 | 0..21, 26..48 (22..25 none) | HP 0..54 (LP GPIO later) |
| GPIO / IO_MUX base | `0x60004000` / `0x60009000` | same band | `0x500E0000` / `0x500E1000` |
| Peri clocks | SYSTEM `0x600C…` | SYSTEM `0x600C…` | **HP_SYS_CLKRST** `0x500E6000` (ADC oneshot: **LPPERI** `0x50120000`) |
| LEDC timer | `freq` (C3/S3 layout) | `freq` | **`freq_p4`** (RST bit 24, CONF `@ +0x170`) |
| ADC oneshot | APB_SARADC + `read_u12` | APB_SARADC + `read_u12` | **LP_ADC** + **`read_u12_p4`** (ADC1 CH0→GPIO16; ADC2 `adc2_out_p4` CH0→GPIO49) |
| Matrix simple-GPIO | `SIG_GPIO_OUT=128`, OEN bit 9 | `SIG_GPIO_OUT=256`, OEN bit 10 | `SIG_GPIO_OUT=256`, OEN bit 10 |
| GPIO ≥ 32 | n/a | `OUT1` / `ENABLE1` / `IN1` | `OUT1` / `ENABLE1` / `IN1` |
| On-die Wi‑Fi/BLE | yes | yes | **no** (companion / other host) |
| On-chip EMAC | no | no | yes — RMII via [`esp_eth`](https://github.com/klin-lang/esp_eth) later [104] |

Clocks stay **explicit** in factory args (typically APB **80_000_000**, XTAL
**40_000_000** under IDF).

## Requirements

- [Klin](https://github.com/klin-lang/klin) compiler
- For board examples: [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/) **v5.x** (`IDF_PATH` exported)

## Layout

```text
machine_esp/
  version.kl
  pin.kl / pin_s3.kl / pin_p4.kl
  pwm.kl / pwm_s3.kl / pwm_p4.kl / rc.kl / rc_s3.kl / rc_p4.kl
  uart.kl / uart_s3.kl / uart_p4.kl / i2c.kl / i2c_s3.kl / i2c_p4.kl
  spi.kl / spi_s3.kl / spi_p4.kl / adc.kl / adc_s3.kl / adc_p4.kl
  rmt_s3.kl / rmt_p4.kl   # TX only (WS2812-style bit streams)
  *_test.kl
examples/*_c3/           # esp32c3
examples/*_s3/           # esp32s3
examples/*_p4/           # esp32p4 (Pin…Adc+Rmt)
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

## Usage — P4

```klin
import "github/klin-lang/machine_esp" machine

let led = machine.pin_out_p4(2)
let pwm = machine.pwm_out_p4(2, 0, 0, 80000000)
pwm.freq_p4(1000)                          // P4 timer layout ≠ C3/S3 `freq`
let u = machine.uart_out_p4(0, 17, 18, 80000000, 115200)
let bus = machine.i2c_out_p4(0, 8, 9, 40000000, 100000)
let s = machine.spi_out_p4(2, 12, 11, 13, 80000000, 1000000, 0)
let servo = machine.rc_out_p4(2, 0, 0, 80000000, 50, 1000, 2000)
let rmt = machine.rmt_tx_p4(2, 0, 80000000)  // tick = PLL_F80M/8; put/start/wait_done
let adc = machine.adc_out_p4(16, 0)          // ADC1 CH0 → GPIO16; read_u12_p4
let adc2 = machine.adc2_out_p4(49, 0)        // ADC2 CH0 → GPIO49
```

`uart_out_p4` accepts instance **0..=4**. I2C0 only. SPI2 only. Soft SPI CS via a separate `Pin`.
P4 LEDC timer bits differ from C3/S3 — call **`freq_p4`**, not `freq`.
P4 `rmt_tx_p4` is TX channels **0..=3** only (no DMA / carrier).
P4 ADC is LP_ADC — call **`read_u12_p4`**, not `read_u12`. ADC1 CH0→GPIO16 … CH7→GPIO23; ADC2 CH0→GPIO49 … CH5→GPIO54.
RMII Ethernet → [`esp_eth`](https://github.com/klin-lang/esp_eth) [104] E1 (not this package).

## Examples

```sh
. $IDF_PATH/export.sh
cd examples/blink_p4   # or blink_s3 / pwm_s3 / … / blink_c3 / …
make emit KLIN=/path/to/klin/bin/klin.dart
make build
make flash
```

```sh
klin get github/klin-lang/machine_esp@v0.12.0
```

Wi‑Fi, NVS, and IDF peripheral drivers are **out of scope**.

## Tests

```sh
dart run /path/to/klin/bin/klin.dart test machine_esp/
```

Host tests cover baud/div/base/scale math, S3/P4 pin maps, UART/I2C/SPI/ADC maps,
and RMT symbol/mem helpers (no MMIO).

## License

MIT
