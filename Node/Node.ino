#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "";//Add credentials of the wifi here
const char* password = "";//Add credentials of the wifi here
const char* mqttServer = ;//Add your cloudmqtt.com server name here 
const int mqttPort = ;//Add your cloudmqtt.com port here 
const char* mqttUser = ;//Add your cloudmqtt.com credentials here (username)
const char* mqttPassword = ;//Add your cloudmqtt.com credentials here 
const int RED = 5;     
const int GREEN = 4;
const int BLUE = 16;

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.subscribe("SmartHome/Automation");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.println(length);
  Serial.print("Message:");
  char string[length];
  char var[3][3];
  int num[3]= {0,0,0};
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)(payload[i]));
    string[i] = ((char)(payload[i]));
  }
  Serial.println("");
  int m=0;
  int j=0;
  Serial.println("After loop");
  for (int i=0 ; i< length && m<4;i++){
    if(string[i]!=','){
      var[j][m]=string[i];
      const char *b = &var[j][m];
      num[j] = (10*num[j])+ (atoi(b));
      m++;  
    }
    else{
      j+=1;
      m=0;
    }
  }
    
  setColor(num);
  Serial.println();
  Serial.println("-----------------------");
 
}

void setColor(int payload[]){
  analogWrite(RED, payload[0]);
  analogWrite(GREEN, payload[1]);
  analogWrite(BLUE, payload[2]);  
}
 
void loop() {
  client.loop();
}
