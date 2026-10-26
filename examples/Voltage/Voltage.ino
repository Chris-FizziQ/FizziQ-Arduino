// Voltage on A0, shown in FizziQ (USB with FizziQ Web, or Bluetooth with the
// FizziQ app on boards that have Bluetooth LE).
#include <FizziQ.h>

FizziQ fizziq("TP1");   // Bluetooth name : "FizziQ Arduino -TP1"

void setup() {
  fizziq.begin();
}

void loop() {
  float u = analogRead(A0) * 5.0 / 1023;   // Uno : 10 bits, 5 V (adapt for your board)
  fizziq.send(FQ_VOLTAGE, u);
  delay(100);
}
