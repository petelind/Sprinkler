#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <SHT1x.h>


#include "config.h"

#if SIMULATED_DATA

void initSensor()
{
    // use SIMULATED_DATA, no sensor need to be inited
}

float readTemperature()
{
    return random(20, 30);
}

float readHumidity()
{
    return random(30, 40);
}

#else

SHT1x sht1x(SHT_DATA_PIN, SHT_CLOCK_PIN);

void initSensor()
{
    // SHT10 does not need init
}

float readTemperature()
{
    return sht1x.readTemperatureC();
}

float readTemperatureC() {  return readTemperature(); }

float readTemperatureF() {
  sht1x.readTemperatureF();
}


float readHumidity()
{
    return sht1x.readHumidity();
}

#endif

bool readMessage(int messageId, char *payload)
{
    float temperature = readTemperature();
    float temperature_f = readTemperatureF();
    float humidity = readHumidity();

    showDataOnOled(temperature_f, temperature, humidity);
    
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
    root["messageId"] = messageId;
    bool temperatureAlert = false;

    // NAN is not the valid json, change it to NULL
    if (std::isnan(temperature))
    {
        root["temperature"] = NULL;
    }
    else
    {
        root["temperature"] = temperature;
        if (temperature > TEMPERATURE_ALERT)
        {
            temperatureAlert = true;
        }
    }

    if (std::isnan(humidity))
    {
        root["humidity"] = NULL;
    }
    else
    {
        root["humidity"] = humidity;
    }
    root.printTo(payload, MESSAGE_MAX_LEN);
    return temperatureAlert;
}

void parseTwinMessage(char *message)
{
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(message);
    if (!root.success())
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }

    if (root["desired"]["interval"].success())
    {
        interval = root["desired"]["interval"];
    }
    else if (root.containsKey("interval"))
    {
        interval = root["interval"];
    }
    Serial.println("New interval set: ");
    Serial.print(interval);
}
