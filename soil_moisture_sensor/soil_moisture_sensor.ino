#define SOIL_MOISTURE_PIN A0
#define SOIL_MOISTURE_THRESHOLD_LOW 30
#define SOIL_MOISTURE_THRESHOLD_HIGH 80

void setup() {
  Serial.begin(9600);
}

void loop() {
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);

  if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD_LOW) {
    Serial.println("Soil is too dry.");
  } else if (soilMoistureValue > SOIL_MOISTURE_THRESHOLD_HIGH) {
    Serial.println("Soil is too wet.");
  } else {
    Serial.println("Soil moisture is in an optimal range.");
  }

  delay(1000);
}
