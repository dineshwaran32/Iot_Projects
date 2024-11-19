#include <DHT.h>
#include <SoftwareSerial.h>

#define SOIL_MOISTURE_PIN1 A0
#define SOIL_MOISTURE_PIN2 A1
#define DHT_PIN1 2
#define DHT_PIN2 3
#define DHT_TYPE DHT22
#define RELAY_CHANNEL_1 9
#define RELAY_CHANNEL_2 10
#define GSM_RX 7
#define GSM_TX 8

#define SOIL_MOISTURE_THRESHOLD_LOW 30  
#define SOIL_MOISTURE_THRESHOLD_HIGH 80 

const char phoneNumber[] = "+916381439780";

DHT dht1(DHT_PIN1, DHT_TYPE);
DHT dht2(DHT_PIN2, DHT_TYPE);
SoftwareSerial gsm(GSM_RX, GSM_TX);

void setup() {
  pinMode(RELAY_CHANNEL_1, OUTPUT);
  pinMode(RELAY_CHANNEL_2, OUTPUT);
  digitalWrite(RELAY_CHANNEL_1, LOW);
  digitalWrite(RELAY_CHANNEL_2, LOW);

  Serial.begin(9600);
  gsm.begin(9600);

  dht1.begin();
  dht2.begin();

  Serial.println("System initialized");
}

void loop() {
  int soilMoistureValue1 = analogRead(SOIL_MOISTURE_PIN1);
  int soilMoistureValue2 = analogRead(SOIL_MOISTURE_PIN2);

  float humidity1 = dht1.readHumidity();
  float temperature1 = dht1.readTemperature();
  float humidity2 = dht2.readHumidity();
  float temperature2 = dht2.readTemperature();

  Serial.println("Sensor Data:");
  Serial.print("Soil Moisture 1: "); Serial.println(soilMoistureValue1);
  Serial.print("Soil Moisture 2: "); Serial.println(soilMoistureValue2);
  Serial.print("DHT1 - Temperature: "); Serial.print(temperature1); Serial.print(" °C, Humidity: "); Serial.println(humidity1);
  Serial.print("DHT2 - Temperature: "); Serial.print(temperature2); Serial.print(" °C, Humidity: "); Serial.println(humidity2);

  String message = "Soil Moisture 1: " + String(soilMoistureValue1) +
                   ", Soil Moisture 2: " + String(soilMoistureValue2) +
                   ", DHT1 - Temp: " + String(temperature1) + "C, Humidity: " + String(humidity1) +
                   ", DHT2 - Temp: " + String(temperature2) + "C, Humidity: " + String(humidity2);

  sendSMS(message);

  if (soilMoistureValue1 < SOIL_MOISTURE_THRESHOLD_LOW || soilMoistureValue2 < SOIL_MOISTURE_THRESHOLD_LOW) {
    digitalWrite(RELAY_CHANNEL_1, HIGH);
    digitalWrite(RELAY_CHANNEL_2, HIGH);
    sendSMS("Motor turned ON due to low soil moisture");
    Serial.println("Motor ON");
  } else if (soilMoistureValue1 > SOIL_MOISTURE_THRESHOLD_HIGH && soilMoistureValue2 > SOIL_MOISTURE_THRESHOLD_HIGH) {
    digitalWrite(RELAY_CHANNEL_1, LOW);
    digitalWrite(RELAY_CHANNEL_2, LOW);
    sendSMS("Motor turned OFF due to sufficient soil moisture");
    Serial.println("Motor OFF");
  }

  delay(5000);
}

void sendSMS(String message) {
  gsm.print("AT+CMGF=1\r");
  delay(100);
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneNumber);
  gsm.print("\"\r");
  delay(100);
  gsm.print(message);
  delay(100);
  gsm.write(26);
  delay(5000);
}
