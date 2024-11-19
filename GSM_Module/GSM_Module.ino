#include <SoftwareSerial.h>

#define GSM_RX 7
#define GSM_TX 8

const char phoneNumber[] = "+916381439780";
SoftwareSerial gsm(GSM_RX, GSM_TX);

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);

  Serial.println("Initializing GSM module...");
  gsm.print("AT\r");
  delay(1000);
}

void loop() {
  sendSMS("Hello! This is a test message from Arduino.");
  delay(10000);
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
