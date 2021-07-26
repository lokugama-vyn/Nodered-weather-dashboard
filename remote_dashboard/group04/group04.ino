/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outgoingTopic" every two seconds
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "ssid";
const char* password = "password";
const char* mqtt_server = "Broker.mqttdashboard.com";
//const char* incommingTopic2="Access denied";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char myinput1[300];
String myinput;
int value=0;
String update_time1="";
int toint_update_time;
long int t;

char copy[300];

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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  // i create
  /*
  if(strcmp(topic,"AccessFailure")==0){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }  
      
    
  }
  
  if(strcmp(topic,"incommingTopic")==0){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
      for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  }
  */
  
  if(strcmp(topic,"WarningTopic")==0){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
      for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
        //WARN
  }
  if(strcmp(topic,"update_time")==0){
    
      for (int i = 0; i < length; i++) {
      update_time1+=(char)payload[i];
    }
    int toint_update_time=update_time1.toInt();
    //Serial.print(toint_update_time);
    t=millis();
    update_time1="";
    for(int i=0;i<toint_update_time/1000;i++){
      digitalWrite(BUILTIN_LED,LOW);
      delay(500);
      digitalWrite(BUILTIN_LED,HIGH);
      delay(500);
    }
    
  }
  
  
  Serial.println();
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outgoingTopic", "hello world");       i commented
      // ... and resubscribe
      client.subscribe("incommingTopic");
      client.subscribe("AccessFailure");
      client.subscribe("WarningTopic");
      client.subscribe("update_time");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED,HIGH);                                          // Initialize the BUILTIN_LED pin as an output and put low as we need to on when warn come
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //client.setCallback(callback1);      //i put to incomming topic
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(Serial.available()>0){
  String myinput=Serial.readString();                       //when giving coordinates put , at last
  
 myinput.toCharArray(copy,300);            //final
  Serial.print("Publish message: ");
 Serial.println(copy);
  client.publish("outgoingTopic", copy);

  delay(5000);
  }
  
  
}
