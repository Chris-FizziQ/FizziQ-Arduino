# FizziQ for Arduino

Send measurements from an Arduino board to [FizziQ](https://www.fizziq.org):
the FizziQ app on a phone (Bluetooth) or FizziQ Web on a computer (USB).
FizziQ then plots, records and analyses the data like any FizziQ sensor.

```cpp
#include <FizziQ.h>

FizziQ fizziq("TP1");          // Bluetooth name: "FizziQ Arduino -TP1"

void setup() {
  fizziq.begin();
}

void loop() {
  fizziq.send(FQ_VOLTAGE, analogRead(A0) * 5.0 / 1023);
  delay(100);
}
```

## Boards

| Board | USB (FizziQ Web) | Bluetooth (FizziQ app) |
|---|---|---|
| Uno, Nano, Mega | yes | no |
| ESP32, Nano ESP32, Uno R4 WiFi, Nano 33 BLE, Nano 33 IoT, Nano RP2040 Connect, MKR WiFi 1010 | yes | yes |

Bluetooth is enabled automatically on boards that have it.

## Quantities

`FQ_VOLTAGE` (V), `FQ_CURRENT` (A), `FQ_POWER` (W), `FQ_TEMPERATURE` (°C),
`FQ_HUMIDITY` (%), `FQ_PRESSURE` (hPa), `FQ_LIGHT` (lux), `FQ_DISTANCE` (mm),
`FQ_ACCELERATION` (m/s²), `FQ_SOUND_LEVEL` (dB), `FQ_ANALOG` (no unit).

## Channels

The same quantity can be sent on channels 0 to 5 (0 by default). FizziQ shows
each channel as a separate instrument:

```cpp
fizziq.send(FQ_VOLTAGE, u1, 0);
fizziq.send(FQ_VOLTAGE, u2, 1);
```

## Notes

- Keep at least 10 ms between two measurements of the same quantity.
- The name must not contain "radio".
- In FizziQ Web, choose the USB connection and select the board's port (115200 baud).

## Licence

MIT, FizziQLabs.
