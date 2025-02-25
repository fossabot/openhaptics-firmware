#include <Arduino.h>
#include <Wire.h>

// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include "auto_output.h"
#include "openhaptics.h"
#include "utils.h"

#include "connections/bhaptics.h"
#include "output_components/closest.h"
#include "output_writers/ledc.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "battery/abstract_battery.h"
#include "battery/adc_battery.h"
#endif

#pragma region bHaptics_trash

const uint16_t _bh_max_x = 6;
const uint16_t _bh_max_y = 1;

inline Point2D* make_point(uint16_t x, uint16_t y) {
  return getPoint(x, y, _bh_max_x, _bh_max_y);
}

Point2D* indexesToPoints[_bh_max_x * _bh_max_y] = {
    make_point(0, 0), make_point(1, 0), make_point(2, 0),
    make_point(3, 0), make_point(4, 0), make_point(5, 0)};

void vestMotorTransformer(std::string& value) {
  for (size_t i = 0; i < _bh_max_x; i++) {
    uint8_t byte = value[i];
    outputData_t output_0;
    output_0.point = *indexesToPoints[i];
    output_0.intensity = map(byte, 0, 100, 0, UINT16_MAX);
    App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, output_0);
  }
}

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure PWM pins to their positions on the glove
  auto gloveOutputs = transformAutoOutput({
      // clang-format off
      {new LEDCOutputWriter(32), new LEDCOutputWriter(33), new LEDCOutputWriter(25), new LEDCOutputWriter(26), new LEDCOutputWriter(27), new LEDCOutputWriter(14)},
      // clang-format on
  });

  auto glove = new ClosestOutputComponent(gloveOutputs);

  App.getOutput()->addComponent(OUTPUT_PATH_ACCESSORY, glove);

  BHapticsBLEConnection* bhBleConnection =
      new BHapticsBLEConnection(BLUETOOTH_NAME, vestMotorTransformer);
  App.setConnection(bhBleConnection);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery = new ADCBattery(33);
  App.setBattery(battery);
#endif
}
