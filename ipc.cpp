#include <Wire.h>
#include <Arduino.h>
#include "protocol.h"

void ipcBegin()
{
    Wire.begin();
}

void ipcStartApp(uint8_t app)
{
    Wire.beginTransmission(8);

    Wire.write(CMD_START_APP);
    Wire.write(app);

    byte err = Wire.endTransmission();

    Serial.print("I2C Status: ");
    Serial.println(err);
}