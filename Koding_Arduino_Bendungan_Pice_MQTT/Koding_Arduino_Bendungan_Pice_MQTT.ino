//Koding LED -----------------------------
int pinLed = 5;

//Koding LDR -----------------------------
int pinLdr = 35;
int nilaiSensor = 0;

//Koding WiFi-----------------------------
#include <WiFi.h>
const char* ssid = "KelasRobot.com";
const char* password = "k3l4sr0b0t";
WiFiClient espClient;

//Koding MQTT-----------------------------
#include "PubSubClient.h"
PubSubClient client(espClient);
const char* mqttServer = "broker.emqx.io";
int port = 1883;
char clientId[] = "Ajang92917926129121m1n4";
char topic_led[] = "m1n4mqtt/92917926129121/led";
char topic_ldr[] = "m1n4mqtt/92917926129121/ldr";
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

//Timer-------------------------------------
unsigned const long jeda = 1000;
unsigned long zero = 0;

void setup(){

  //Koding LED -----------------------------
  pinMode(pinLed, OUTPUT);
  Serial.begin(115200);

  //Fungsi Connect WiFi---------------------
  wifiConnect();

  //Koding MQTT-----------------------------
  client.setServer(mqttServer, port);
  client.setCallback(callback);

}

void loop(){

  ///Koding MQTT----------------------------
  delay(10);
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

  //Koding LDR -----------------------------
  nilaiSensor = analogRead(pinLdr);
  // Serial.println(nilaiSensor);

  //Logika Lampu Otomatis ------------------
  // if( nilaiSensor > 2000 ){
  //   digitalWrite(pinLed, HIGH);
  // }else{
  //   digitalWrite(pinLed, LOW);
  // }

  //Koding pengiriman ( Publish ) ----------
  if(millis() - zero >= jeda){
      Serial.println("Mqtt Publish..." + String(nilaiSensor));
      snprintf (msg, MSG_BUFFER_SIZE, "%ld", nilaiSensor);
      client.publish(topic_ldr, msg);
      zero = millis();
  }
}

//Fungsi Untuk Terhubung ke WiFi ---------------
void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Terhubung ke WiFi");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

//Fungsi Untuk Terhubung ke MQTT----------------
void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT Server... ");
    if (client.connect(clientId)) {
      Serial.print(clientId);
      Serial.println(" Terhubung");
      client.subscribe(topic_led);
    } else {
      Serial.print("Gagal Terhubung, stataus=");
      Serial.print(client.state());
      Serial.println(" tunggu 2 detik");
      delay(2000);
    }
  }
}

//Fungsi Kalo Menerima Pesan---------------------
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Pesan Baru, Topik: ");
  Serial.print(topic);
  Serial.print(" Pesan: ");
  String stMessage;
  for (int i = 0; i < length; i++) {
    stMessage += (char)message[i];
  }
  Serial.println(stMessage);

  if( stMessage == "ON"){
    digitalWrite(pinLed, HIGH);
  }else{
    digitalWrite(pinLed, LOW);
  }
}