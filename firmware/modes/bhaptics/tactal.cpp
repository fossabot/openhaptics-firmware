// Override you configs in this file (Ctrl+Click)
#include "config/all.h"

#include <Arduino.h>
#include <Wire.h>

#include <utility.hpp>

#include "openhaptics.h"
#include "auto_output.h"

#include <connection_bhble.hpp>
#include "output_components/closest.h"
#include "output_writers/ledc.h"

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
#include "battery/adc_battery.h"
#endif

using namespace OH;
using namespace BH;

#pragma region bHaptics_trash

static const oh_output_point_t* indexesToPoints[BH_LAYOUT_TACTAL_SIZE] = BH_LAYOUT_TACTAL;

void vestMotorTransformer(std::string& value) {
  for (size_t i = 0; i < BH_LAYOUT_TACTAL_SIZE; i++) {
    uint8_t byte = value[i];

    oh_output_data_t output_0{
      .point = *indexesToPoints[i],
      .intensity = map(byte, 0, 100, 0, OH_OUTPUT_INTENSITY_MAX)
    };

    App.getOutput()->writeOutput(OUTPUT_PATH_ACCESSORY, output_0);
  }
}

#pragma endregion bHaptics_trash

void setupMode() {
  // Configure PWM pins to their positions on the face
  std::vector<std::vector<AbstractOutputWriter*>> v = {
      // clang-format off
      {new LEDCOutputWriter(32), new LEDCOutputWriter(33), new LEDCOutputWriter(25), new LEDCOutputWriter(26), new LEDCOutputWriter(27), new LEDCOutputWriter(14)},
      // clang-format on
  };
  auto faceOutputs = transformAutoOutput(v);

  OutputComponent* face = new ClosestOutputComponent(OUTPUT_PATH_ACCESSORY, faceOutputs);
  App.addOutputComponent(face);

#if defined(BATTERY_ENABLED) && BATTERY_ENABLED == true
  AbstractBattery* battery = new ADCBattery(33, { .sampleRate = BATTERY_SAMPLE_RATE }, &App);
  App.setBattery(battery);
#endif

  uint8_t serialNumber[BH_SERIAL_NUMBER_LENGTH] = BH_SERIAL_NUMBER;
  ConnectionBHBLE_Config config = {
      .deviceName = BLUETOOTH_NAME,
      .appearance = BH_BLE_APPEARANCE,
      .serialNumber = serialNumber,
  };
  AbstractConnection* bhBleConnection = new ConnectionBHBLE(&config, vestMotorTransformer, &App, tskNO_AFFINITY);
  App.setConnection(bhBleConnection);
}
