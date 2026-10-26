#include "FizziQ.h"

#ifdef FIZZIQ_BLE
#include <ArduinoBLE.h>

static BLEService uartService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLECharacteristic txChar("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify, 20);
// FizziQ writes commands here (FC:...) : accepted and ignored
static BLECharacteristic rxChar("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLEWriteWithoutResponse, 20);
static bool bleOk = false;
#endif

// Text code and decimals of each quantity (same as the FizziQ Connect boxes)
static const char* quantityCode(uint8_t q, uint8_t* decimals) {
  switch (q) {
    case FQ_TEMPERATURE:  *decimals = 1; return "Tem";
    case FQ_PRESSURE:     *decimals = 1; return "Pre";
    case FQ_HUMIDITY:     *decimals = 1; return "Hum";
    case FQ_DISTANCE:     *decimals = 0; return "Dis";
    case FQ_LIGHT:        *decimals = 0; return "Lum";
    case FQ_CURRENT:      *decimals = 3; return "Int";
    case FQ_VOLTAGE:      *decimals = 2; return "Ten";
    case FQ_ACCELERATION: *decimals = 1; return "Acc";
    case FQ_SOUND_LEVEL:  *decimals = 1; return "Niv";
    case FQ_POWER:        *decimals = 1; return "wat";
    default:              *decimals = 1; return "an";
  }
}

FizziQ::FizziQ(const char* name) : _name(name) {}

void FizziQ::begin() {
  Serial.begin(115200);
#ifdef FIZZIQ_BLE
  static char fullName[30];
  snprintf(fullName, sizeof(fullName), "FizziQ Arduino -%s", _name);
  if (BLE.begin()) {
    BLE.setLocalName(fullName);
    BLE.setDeviceName(fullName);
    BLE.setAdvertisedService(uartService);
    uartService.addCharacteristic(txChar);
    uartService.addCharacteristic(rxChar);
    BLE.addService(uartService);
    BLE.advertise();
    bleOk = true;
  }
#endif
}

void FizziQ::send(FizziQQuantity quantity, float value, uint8_t channel) {
  if (channel > 5) return;            // FizziQ Web reads FZ0 to FZ5 only
  uint32_t t = millis() % 1000000000UL;
  uint8_t decimals;
  const char* code = quantityCode(quantity, &decimals);

  Serial.print("FZ");
  Serial.print(channel);
  Serial.print(':');
  Serial.print(code);
  Serial.print(':');
  Serial.print(value, decimals);
  Serial.print(':');
  Serial.println(t);

#ifdef FIZZIQ_BLE
  if (!bleOk) return;
  BLE.poll();
  if (!txChar.subscribed()) return;
  // one record per notification : fits the default 23-byte MTU
  uint8_t rec[11];
  rec[0] = '&';
  rec[1] = channel;
  rec[2] = quantity;
  memcpy(rec + 3, &value, 4);         // little-endian on all supported boards
  memcpy(rec + 7, &t, 4);
  txChar.writeValue(rec, sizeof(rec));
#endif
}
