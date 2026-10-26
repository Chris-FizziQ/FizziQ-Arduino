// FizziQ - send measurements from an Arduino board to the FizziQ app
// (phone, Bluetooth) and to FizziQ Web (computer, USB).
//
// The board is seen as a FizziQ Connect box:
//   - USB : one text line per measurement, FZ<channel>:<code>:<value>:<time>
//   - Bluetooth LE (boards that have it) : Nordic UART service, '&' + 10-byte
//     records (channel, quantity, float value, uint32 time in ms)
#ifndef FIZZIQ_H
#define FIZZIQ_H

#include <Arduino.h>

// Boards with Bluetooth LE (ArduinoBLE). Define FIZZIQ_NO_BLE before the
// #include to use USB only.
#if !defined(FIZZIQ_NO_BLE) && \
    ((defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_BT_ENABLED)) || \
     defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_ARDUINO_NANO33BLE) || \
     defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_SAMD_NANO_33_IOT) || \
     defined(ARDUINO_SAMD_MKRWIFI1010))
#define FIZZIQ_BLE 1
#endif

// Quantities known by FizziQ (numbers of the FizziQ Connect protocol)
enum FizziQQuantity : uint8_t {
  FQ_TEMPERATURE  = 1,   // °C
  FQ_PRESSURE     = 2,   // hPa
  FQ_HUMIDITY     = 4,   // %
  FQ_DISTANCE     = 5,   // mm
  FQ_LIGHT        = 6,   // lux
  FQ_CURRENT      = 8,   // A
  FQ_VOLTAGE      = 9,   // V
  FQ_ACCELERATION = 18,  // m/s²
  FQ_SOUND_LEVEL  = 20,  // dB
  FQ_ANALOG       = 31,  // no unit
  FQ_POWER        = 32   // W
};

class FizziQ {
public:
  // name : shown in the FizziQ Bluetooth list as "FizziQ Arduino -<name>".
  // Must not contain "radio" (FizziQ would switch to FizziQ Radio mode).
  explicit FizziQ(const char* name = "ARDUINO");

  void begin();                     // starts Serial (115200) and Bluetooth if available
  // channel 0 to 5 : the same quantity on several channels gives several
  // instruments in FizziQ
  void send(FizziQQuantity quantity, float value, uint8_t channel = 0);

private:
  const char* _name;
};

#endif
