#define BLYNK_TEMPLATE_ID "TMPL3G_YMHJEw"
#define BLYNK_TEMPLATE_NAME "Temperature and Humidity Monitor"
#define BLYNK_AUTH_TOKEN "nQslOtnoEY1NINMIcYWaeiSrgj4M43JP"

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <DHT.h>

// GSM module settings
#define GSM_RX 7
#define GSM_TX 8
#define APN "airtelgprs.com"  
#define USER ""        
#define PASS ""        

#define SOIL_MOISTURE_PIN A0
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define RELAY_PIN 9

#define SOIL_MOISTURE_THRESHOLD_LOW 30  
#define SOIL_MOISTURE_THRESHOLD_HIGH 80 

DHT dht(DHT_PIN, DHT_TYPE);
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);
TinyGsm modem(gsmSerial);
TinyGsmClient client(modem);

char auth[] = BLYNK_AUTH_TOKEN;
char apn[] = APN;
char user[] = USER;
char pass[] = PASS;




void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.begin(9600);
  gsmSerial.begin(9600);
  dht.begin();

  Serial.println("Initializing modem...");
  if (!modem.restart()) {
    Serial.println("Failed to restart modem");
    while (true);
  }

  Serial.println("Connecting to GSM network...");
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println("Failed to connect to GPRS");
    while (true);
  }
  Serial.println("GPRS connected");

  Blynk.begin(auth, modem, client, apn, user, pass);
}

void loop() {
  Blynk.run();

  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Blynk.virtualWrite(V1, soilMoistureValue);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);

  if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD_LOW) {
    digitalWrite(RELAY_PIN, HIGH);
    Blynk.virtualWrite(V4, "Motor ON");
  } else if (soilMoistureValue > SOIL_MOISTURE_THRESHOLD_HIGH) {
    digitalWrite(RELAY_PIN, LOW);
    Blynk.virtualWrite(V4, "Motor OFF");
  }

  delay(5000);
}
