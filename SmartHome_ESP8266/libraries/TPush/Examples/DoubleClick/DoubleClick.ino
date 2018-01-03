#include <TPush.h>

TPush Btn1;

bool DoubleTap;
bool Lampeggia;

int BTNpin = 2;
int LEDpin = 3;

void setup() {
  // Pulsante impostato sul pin D2 - Attivo Basso (Si attiva quando riceve il livello logico 0)
  Btn1.setUp(BTNpin, LOW);
  // Imposto la modalità di OUTPUT digitale per il pin 2
  pinMode(LEDpin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  
  if (Btn1.DoubleClick()) {
    digitalWrite(LEDpin, DoubleTap = !DoubleTap);
  }

  if (DoubleTap && Btn1.Wait(100)) {
    digitalWrite(LEDpin, Lampeggia=!Lampeggia);
  }
  
}
