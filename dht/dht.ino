#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>

const char *ssid =     // cannot be longer than 32 characters!
const char *pass =     //

const char *mqtt_server = ;//Add your cloudmqtt.com server name here 
const int mqtt_port = ;//Add your cloudmqtt.com port here 
const char *mqtt_user = ;//Add your cloudmqtt.com credentials here (username)
const char *mqtt_pass = ;//Add your cloudmqtt.com credentials here 
//const char *mqtt_client_name = "XXXX"; // Client connections cant have the same connection name

const int dhtPin = 2;
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE, 12); // The 12 might need tuning so play with 10-15 till it works for the ESP8266
unsigned long previousMillis = 0;
const long interval = 10000;   


WiFiClient wclient;
//PubSubClient client(wclient, mqtt_server, mqtt_port);
//PubSubClient client(mqtt_server, mqtt_port, wclient);

PubSubClient client(wclient);




void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);

  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");



  client.setServer(mqtt_server, mqtt_port);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  client.publish("/DHT", "Hello from ESP32");

  
  Serial.println();
  Serial.println();

  dht.begin();
}

void loop() {
  client.loop();
  SendTempHumid();
}

// Non-Blocking delay
void SendTempHumid(){
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
 
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      //return; This will ensure that data is always sent
    }

    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C ");

    char s[10];
    //itoa(int(h), s, 10);
    dtostrf(h,2,2,s);
    client.publish("Humidity",s );
    dtostrf(t,2,2,s);
    client.publish("Temperature",s );
    
  }
}
