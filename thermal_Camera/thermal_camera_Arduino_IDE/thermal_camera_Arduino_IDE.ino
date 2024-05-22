#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

void setup() {
  Serial.begin(9600);
  if (!amg.begin()) {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1);
  }
}

void loop() {
  float pixels[64];  // 8x8 pixels
  amg.readPixels(pixels);
  
  for (int i = 0; i < 64; i++) {
    Serial.print(pixels[i]);
    if (i < 63) {
      Serial.print(",");
    }
  }
  Serial.println();
  
  delay(100);  // ajuste o delay conforme necessário
}
