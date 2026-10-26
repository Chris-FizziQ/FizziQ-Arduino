# FizziQ for Arduino

Send measurements from an Arduino board to [FizziQ](https://www.fizziq.org):
the FizziQ app on a phone or tablet (Bluetooth) or
[FizziQ Web](https://fizziqweb.web.app) on a computer (USB).

FizziQ sees the board as an external FizziQ sensor: the data can be displayed,
plotted, recorded and analysed like any other FizziQ measurement.

```cpp
#include <FizziQ.h>

FizziQ fizziq("LAB1");          // Bluetooth name: "FizziQ Arduino -LAB1"

void setup() {
  fizziq.begin();
}

void loop() {
  fizziq.send(FQ_VOLTAGE, analogRead(A0) * 5.0 / 1023);
  delay(100);
}
```

## Installation

In the Arduino IDE, open the **Library Manager**, search for **FizziQ** and
click **Install**. The ArduinoBLE library is installed with it.

Or download this repository as a ZIP file (**Code > Download ZIP**) and add it
with **Sketch > Include Library > Add .ZIP Library**.

The examples are in **File > Examples > FizziQ**.

## Boards

| Board | USB (FizziQ Web) | Bluetooth (FizziQ app) |
|---|---|---|
| Uno, Nano, Mega, Leonardo… | yes | no |
| ESP32 (with Bluetooth), Arduino Nano ESP32 | yes | yes |
| Uno R4 WiFi | yes | yes |
| Nano 33 BLE, Nano 33 IoT, Nano RP2040 Connect, MKR WiFi 1010 | yes | yes |

Bluetooth is enabled automatically on boards that have it. On other boards,
or if Bluetooth fails to start, the library works over USB only.

## Connecting to FizziQ

- **FizziQ Web (computer, USB)**: open <https://fizziqweb.web.app> in Chrome
  or Edge, choose the USB connection and select the board's serial port.
- **FizziQ app (phone or tablet, Bluetooth)**: open the Bluetooth connection
  screen; the board appears in the list of FizziQ devices as
  `FizziQ Arduino -<name>`.

The quantities appear in FizziQ as soon as the first measurement is received.

## Quantities

| Constant | Quantity | Unit | Decimals (USB) |
|---|---|---|---|
| `FQ_VOLTAGE` | Voltage | V | 2 |
| `FQ_CURRENT` | Current | A | 3 |
| `FQ_POWER` | Power | W | 1 |
| `FQ_TEMPERATURE` | Temperature | °C | 1 |
| `FQ_HUMIDITY` | Relative humidity | % | 1 |
| `FQ_PRESSURE` | Pressure | hPa | 1 |
| `FQ_LIGHT` | Illuminance | lux | 0 |
| `FQ_DISTANCE` | Distance | mm | 0 |
| `FQ_ACCELERATION` | Acceleration | m/s² | 1 |
| `FQ_SOUND_LEVEL` | Sound level | dB | 1 |
| `FQ_ANALOG` | Analog value | — | 1 |

Values are sent in these units: convert your readings before calling
`send()` (for example, mA must be divided by 1000 to be sent as A).
Over Bluetooth the value is sent as a full 32-bit float, without rounding.

Other quantities already known by FizziQ (CO2, magnetic field, pH, UV,
weight…) can be added: open an issue on this repository.

## Channels

Each measurement goes to a channel from 0 to 5 (0 by default). The same
quantity sent on several channels gives several instruments in FizziQ, for
example two voltages in the same circuit:

```cpp
fizziq.send(FQ_VOLTAGE, u1, 0);   // across R1
fizziq.send(FQ_VOLTAGE, u2, 1);   // across R2
fizziq.send(FQ_CURRENT, i);       // channel 0
```

## Reference

```cpp
FizziQ fizziq(name);                       // name: up to 13 characters
fizziq.begin();                            // starts Serial at 115200 baud and Bluetooth
fizziq.send(quantity, value);              // channel 0
fizziq.send(quantity, value, channel);     // channel 0 to 5
```

USB only on a board that has Bluetooth:

```cpp
#define FIZZIQ_NO_BLE
#include <FizziQ.h>
```

## Good to know

- Measurements are sent as soon as `send()` is called. Keep at least 10 ms
  between two measurements of the same quantity.
- `begin()` starts `Serial` at 115200 baud: do not call `Serial.begin()` with
  another speed. Other text printed on `Serial` is ignored by FizziQ Web.
- The name must not contain the word "radio" (the FizziQ app would treat the
  board as a FizziQ Radio device).
- Only one Bluetooth connection at a time.

## Protocol

The library uses the formats of the FizziQ Connect sensor boxes:

- **USB**: one text line per measurement, `FZ<channel>:<code>:<value>:<time ms>`,
  for example `FZ0:Ten:3.21:12345`.
- **Bluetooth LE**: Nordic UART service (`6E400001-B5A3-F393-E0A9-E50E24DCCA9E`),
  notifications on `6E400002-…`. Each notification is `&` followed by a 10-byte
  record: channel (1 byte), quantity number (1 byte), value (float32,
  little-endian), time in ms (uint32, little-endian).

## Licence

MIT, © 2026 FizziQLabs. Contact: info@fizziqlab.org
