
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  bool isFirst;
  for (int channel = 0; channel < GPT_HOWMANY; channel++) {
    char buf[16];
    snprintf(buf, sizeof(buf), "GPT %d", channel);
    Serial.println(buf);

    isFirst = true;
    Serial.print("  A: ");
    for (unsigned int pin = 0; pin < PINS_COUNT; pin++) {
      auto cfg = getPinCfgs(pin, PIN_CFG_REQ_PWM);

      if (IS_PIN_GPT_PWM(cfg[0]) && channel == GET_CHANNEL(cfg[0]) && IS_PWM_ON_A(cfg[0])) {
        if (!isFirst) {
          Serial.print(", ");
        }
        Serial.print(pin);
        isFirst = false;
      }
    }
    Serial.println();

    isFirst = true;
    Serial.print("  B: ");
    for (unsigned int pin = 0; pin < PINS_COUNT; pin++) {
      auto cfg = getPinCfgs(pin, PIN_CFG_REQ_PWM);

      if (IS_PIN_GPT_PWM(cfg[0]) && channel == GET_CHANNEL(cfg[0]) && IS_PWM_ON_B(cfg[0])) {
        if (!isFirst) {
          Serial.print(", ");
        }
        Serial.print(pin);
        isFirst = false;
      }
    }
    Serial.println();
    Serial.println();
  }
}

void loop() {
}
