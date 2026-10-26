// Two voltages (A0 and A1) on channels 0 and 1 : FizziQ shows them as two
// separate instruments.
#include <FizziQ.h>

FizziQ fizziq("TP2");

void setup() {
  fizziq.begin();
}

void loop() {
  fizziq.send(FQ_VOLTAGE, analogRead(A0) * 5.0 / 1023, 0);
  fizziq.send(FQ_VOLTAGE, analogRead(A1) * 5.0 / 1023, 1);
  delay(100);
}
