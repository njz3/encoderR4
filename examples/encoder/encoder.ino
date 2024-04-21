
#include<encoderR4.h>

EncoderR4 encoder(4, 5);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  auto ret = encoder.begin();
  if (!ret) {
    Serial.println("Configuration failed!");
    while (true) { }
  }
}

void loop() {
  Serial.print(encoder.read());
  Serial.print("    ");
  Serial.println(encoder.readChange());
  delay(1000);
}
