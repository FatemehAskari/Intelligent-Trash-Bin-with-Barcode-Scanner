#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>

Servo myservo;
int pos = 0; 
int ledPin = 4;

// WiFi
const char* ssid = "Pooria";
const char* password = "Mrx00120012";

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32/barcode";
const char *topictype = "esp32/type";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

HardwareSerial barcodeSerial(2);

String receivedMessage; 

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    barcodeSerial.begin(9600, SERIAL_8N1, 1, 3);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    myservo.setPeriodHertz(50);// Standard 50hz servo
    myservo.attach(18, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                 
      pinMode(ledPin, OUTPUT);
      //Serial.begin(9600); 

    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    //Serial.print("Message arrived in topic: ");
    //Serial.println(topic);
    //Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        //Serial.print((char) payload[i]);
        receivedMessage = (char)payload[i];
    
    if(receivedMessage=="0" || receivedMessage=="1"){
      //Serial.print("\n");
      //Serial.println(receivedMessage);
      //Serial.print("\n");
      turner();

    }
    else if(receivedMessage=="2"){
      digitalWrite(ledPin, HIGH);
      delay(5000);                  
      digitalWrite(ledPin, LOW);
    }
    }
    //turner();
    
    //Serial.println();
    //Serial.println("-----------------------");
    //Serial.print("Received Message: ");
    //Serial.println(receivedMessage);
}

void turner(){
   for (pos = 0; pos <= 45; pos += 1) {
        myservo.write(pos);
        delay(15);
      }
      delay(20000); 
      for (pos = 45; pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(15);
      }
}

void loop() {
    client.loop();
    if (barcodeSerial.available()) {
        String data = "";
        //receivedMessage = "";
        while (barcodeSerial.available()) {
            char ch = barcodeSerial.read();
            data += ch;
            delay(2);
        }
        //barcodeSerial.println("\nI am Test\n");
        Serial.println("\ndata: " + data);
        //Serial.println("Received Message: ");
        client.publish(to
        pic, data.c_str());
        client.subscribe(topictype);

        //Serial.print("\nReceived Message: ");
        //if (!receivedMessage.isEmpty()) {
        //receivedMessage = "";
     // }
      //else{
         //Serial.println("Can't receive ");
      //}
    }
     
}