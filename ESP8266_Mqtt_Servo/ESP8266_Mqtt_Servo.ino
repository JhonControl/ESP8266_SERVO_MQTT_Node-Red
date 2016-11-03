/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "  " every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
  
    More information about projects PDAControl
    Mas informacion sobre proyectos PDAControl
    Blog PDAControl English   http://pdacontrolenglish.blogspot.com.co/   
    Blog PDAControl Espanol   http://pdacontrol.blogspot.com.co/
    Channel  Youtube          https://www.youtube.com/c/JhonValenciaPDAcontrol/videos   

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


///https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <OneWire.h>

#include <Servo.h> 
Servo myservo; 

// Update these with values suitable for your network.

const char* ssid = "#########";
const char* password = "###########";
const char* mqtt_server = "#############";    ///broker MQTT
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  Serial.begin(115200);
   myservo.attach(2);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
   String string;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
   // Serial.print((char)payload[i]);
     string+=((char)payload[i]);  
  }
  
    Serial.print(string);

if (topic ="servo")   
    Serial.print(" ");
   int resultado = string.toInt();   
   int pos = map(resultado, 1, 100, 0, 180);
   Serial.println(pos);
    myservo.write(pos);
    delay(15); 
   
 }


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {

      Serial.println("connected");
      client.subscribe("servo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {



   
  if (!client.connected()) {
    reconnect();

  }
  client.loop();


 delay(100);
  
}

