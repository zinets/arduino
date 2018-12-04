#include "dallas_temp.h"

#include <OneWire.h>
#include <DallasTemperature.h> // id = 54

DallasSensor::DallasSensor(int sensorPin) {
  pin = sensorPin;
}

bool DallasSensor::update() {
  OneWire oneWire(pin);
  DallasTemperature sensors(&oneWire);
  DeviceAddress outsideThermometer;

  sensors.begin();
  if (sensors.getDeviceCount() > 0) {
    if (!sensors.getAddress(outsideThermometer, 0)) {
      Serial.println("Unable to find address for Device 0");
      return false;
    }

    sensors.setResolution(outsideThermometer, 9);
    sensors.requestTemperatures();

    temperature = sensors.getTempC(outsideThermometer);
    return true;
  } else {
    return false;
  }
}

float DallasSensor::getTemperature() {
  return temperature;
}
