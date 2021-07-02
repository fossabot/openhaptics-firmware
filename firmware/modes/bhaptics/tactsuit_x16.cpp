#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "firmware.h"
#include "main.h"
#include "connections/bhaptics.h"

#include "outputs/auto_margins.h"
#include "output_writers/pca9685.h"

void setupMode()
{
    Adafruit_PWMServoDriver* pwm = new Adafruit_PWMServoDriver(0x40);
    pwm->begin();
    pwm->setPWMFreq(60);

    autoOutputVector_t frontOutputs {
        { new PCA9685OutputWriter(pwm, 0), new PCA9685OutputWriter(pwm, 8), },
        { new PCA9685OutputWriter(pwm, 1), new PCA9685OutputWriter(pwm, 9), },
        { new PCA9685OutputWriter(pwm, 2), new PCA9685OutputWriter(pwm, 10), },
        { new PCA9685OutputWriter(pwm, 3), new PCA9685OutputWriter(pwm, 11), },
    };
    autoOutputVector_t backOutputs {
        { new PCA9685OutputWriter(pwm, 4), new PCA9685OutputWriter(pwm, 12) },
        { new PCA9685OutputWriter(pwm, 5), new PCA9685OutputWriter(pwm, 13) },
        { new PCA9685OutputWriter(pwm, 6), new PCA9685OutputWriter(pwm, 14) },
        { new PCA9685OutputWriter(pwm, 7), new PCA9685OutputWriter(pwm, 15) },
    };

    OutputAutoComponent_Margin* chestFront = new OutputAutoComponent_Margin(frontOutputs);
    OutputAutoComponent_Margin* chestBack = new OutputAutoComponent_Margin(backOutputs);

    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_FRONT, chestFront);
    App.getOutput()->addComponent(OUTPUT_PATH_CHEST_BACK, chestBack);

    BHapticsBLEConnection* bhBleConnection = new BHapticsBLEConnection(BLUETOOTH_NAME);
    App.registerComponent(bhBleConnection);
}