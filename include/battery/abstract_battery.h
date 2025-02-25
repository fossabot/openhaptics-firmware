#pragma once

#include <stdint.h>
#include "../config/battery.h"
#include "component.h"

class AbstractBattery : public Component {
 protected:
  unsigned long last_battery_sample = 0;
  uint8_t level = 0;  // 0 = min, 255 = max
  virtual uint8_t updateLevel(void) = 0;

 public:
  void loop(void) override;
  uint8_t getLevel() { return this->level; };
};
