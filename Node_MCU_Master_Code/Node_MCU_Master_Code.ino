#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define LED D0


//defining constants for making connetions

// these variables are  
const char* ssid = "Wifi_1";
const char* password =  "123456789";
const char* mqtt_server = "192.168.137.44";
const int mqtt_port = 1883;


// make objects for calling it latera

WiFiClient espClient;
PubSubClient client(espClient);

// Make connections part in the node mcu


void setup() 
{
  pinMode(LED, OUTPUT);
  
  Serial.begin(115200);
  pinMode(D5,INPUT);
  Wire.begin(D1,D2); //i2c Connection with Arduino
  
  //making WiFi connections

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());


  //connections to the ===================================== MQTT "SERVER" ----------------===========================
  
  
  client.setServer(mqtt_server, mqtt_port);


  //WHICH FUNCTION TO EXECUTE???

  client.setCallback(MQTTcallback);//
  
  //THIS FUNCTION
  
  
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");+
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp/test");
}

// MQTT CALLback part =========================> dk bout it rn, but by debugging it looks like something that listens to the messages and reacts over it

// Listener part
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);

  //listen messages and do what????
  if (message == "on") 
  {
    digitalWrite(LED, HIGH);
  }
  else if (message == "off") 
  {
    digitalWrite(LED, LOW);
  }
  //start defining messages
  else if (message == "Light ON"){
    Wire.beginTransmission(8);
    Wire.write("1");
    Wire.endTransmission();
    client.publish("esp/home","Light ON!");

  }
  else if (message == "Door Open"){
    Wire.beginTransmission(8);
    Wire.write("11");
    Wire.endTransmission();
    client.publish("esp/home101","Door Open!");
  }
  else if (message == "Door Close"){
    Wire.beginTransmission(8);
    Wire.write("00");
    Wire.endTransmission();
    client.publish("esp/home101","Door Close!");
  }
  else if (message == "Light OFF"){
    Wire.beginTransmission(8);
    Wire.write("0");
    Wire.endTransmission();
    client.publish("esp/home","LIGHT OFF!");
  }


  Serial.println();
  Serial.println("-----------------------");
}



// Put what the board has to do on idle time in the void loop()  part <=====================>
void loop() 
{
  
  if(digitalRead(D5)== HIGH)
  {
  client.publish("esp/PIR", "Hello someone is at the Door.");
  delay(1000);
  }
  
  client.loop(); //this line is important !
  
}
