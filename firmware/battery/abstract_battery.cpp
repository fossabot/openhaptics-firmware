#include "battery/abstract_battery.h"
#include <Arduino.h>

void AbstractBattery::loop() {
  auto now_ms = millis();

  if (now_ms - this->last_battery_sample >= BATTERY_SAMPLE_RATE) {
    this->last_battery_sample = now_ms;

    this->level = this->updateLevel();

    Serial.printf(">>\t%s: %3u (took %lu ms)\n", __PRETTY_FUNCTION__,
                  this->level, now_ms - millis());
  }
};
