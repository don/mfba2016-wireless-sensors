#include <Phant.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include "keys.h"

#define SLEEP_SECONDS 10

#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Phant phant(HOST, PUBLIC_KEY, PRIVATE_KEY);

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  delay(2000); // give the DHT time to start
}

void loop() {

  float temperature = dht.readTemperature(true); // true == Fahrenheit
  float humidity = dht.readHumidity();

  Serial.print(temperature);
  Serial.print(", ");
  Serial.println(humidity);

  phant.add("humidity", humidity);
  phant.add("temp", temperature);

  WiFiClient client;
  if (!client.connect(HOST, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(phant.post());
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  
  delay(SLEEP_SECONDS * 1000);

}

