#include "Arduino.h"
#include "main.hpp"
#include "dht.h"
#include <ldr.hpp>
#include <../lib/MQ-Sensor/src/MQSensor.hpp>
#include <../lib/PrintManager/src/PrintManager.hpp>

#ifdef RH_PLATFORM_NANO
# include <SoftwareSerial.h>
SoftwareSerial Serial1(10, 11); // RX, TX
#endif

#define DHT11_PIN 5
#define LDR_PIN   A0

// Sensores
dht DHTSensor;
ldr LDRSensor          = ldr(LDR_PIN);
MQSensor Dummy         = MQSensor::NewMQSensor(1, MQ_SENSOR_DUMMY);
PrintManager BT_Serial = PrintManager(&Serial1); // Explicit pointer conversion

/* MONTAGEM
 * LDR/pot pino 4
 * DHT pino 5
 * VAZIO pino 1
 * BLUETOOTH Serial1
 *
 * //*/

// Variáveis de Ambiente
double TemperatureHumidity[10];

// Build
void setup(){
    pinMode(DHT11_PIN, INPUT);
    pinMode(LDR_PIN, INPUT);

    Serial.begin(115200);
    // Por enquanto
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

    // #ifdef RH_PLATFORM_MEGA
    Serial1.begin(115200);
    // #endif

    Serial1.println("DHT TEST PROGRAM ");
    Serial1.print("LIBRARY VERSION: ");
    Serial1.println(DHT_LIB_VERSION);
    Serial1.println();
    Serial1.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop(){
    // Serial1.println("Hello");
    // TODO: reescrever isso
    dhtDebug(DHTSensor, DHT11_PIN);
    TemperatureHumidity[0] = DHTSensor.read11(DHT11_PIN);
    TemperatureHumidity[1] = DHTSensor.temperature;
    TemperatureHumidity[2] = DHTSensor.humidity;
    TemperatureHumidity[3] = LDRSensor.check();

    BT_Serial.addValue("th", DHTSensor.temperature, DHTSensor.humidity);

    TemperatureHumidity[6] = analogRead(LDR_PIN);
    BT_Serial.addValue("l", TemperatureHumidity[6]);

    TemperatureHumidity[5] = (double) Dummy.check();
    BT_Serial.addValue("M", TemperatureHumidity[5]);

    BT_Serial.sendData();

    delay(2000);
} // loop
