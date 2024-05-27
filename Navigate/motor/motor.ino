#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <Servo.h>
#include <PubSubClient.h>


#define pwmMotorA 4 // D2
#define dirMotorA 0 // D3
/* WiFi Credentials*/
#define STA_SSID "ABBgym_2.4"
#define STA_PASS "mittwifiarsabra"
/* MQTT Credentials */
#define MQTT_USERNAME "oliver.sjodahl@hitachigymnasiet.se"
#define MQTT_KEY "reesesPuffs"
#define MQTT_BROKER "maqiatto.com"
#define MQTT_BROKER_PORT 1883

String clientId = "IoTPractice-" + String(ESP.getChipId());

int speed = 0;
int motorDirection = HIGH; // LOW = forward, HIGH = backwards

int servoRight = 140;
int servoLeft = 40;

WiFiClient espClient;
PubSubClient client(espClient);
Servo servo;

/* TODO:
Gör klart MQTT maqiatto
 */
/*const int pwmMotorA = D2;
const int dirMotorA = D3;
const char* ssid = "ABBgym_2.4"
char const* password = "mittwifiarsabra"
*/


void handleMqttMessage(char* topic, byte* payload, unsigned int length) {
  // Convert payload to a string
  char payloadStr[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    payloadStr[i] = (char)payload[i];
  }
  payloadStr[length] = '\0'; // Null-terminate the string
  // Check if the payload is "up"
  if (strcmp(payloadStr, "forward") == 0){
    Serial.println("Received 'forward' command. Performing action...");
    servo.write(90);
    motorDirection = HIGH;
    speed = 255;
    Movement(motorDirection, speed);
    return;
  }
  if (strcmp(payloadStr, "backwards") == 0) {
    Serial.println("Received 'backwards' command. Performing action...");
    motorDirection = LOW;
    speed = 100;
    Movement(motorDirection, speed);
    return;
  }
  if (strcmp(payloadStr, "left") == 0) {
    Serial.println("Received 'left' command. Performing action...");
    rotateServo(servoLeft);
    return;
  }
  if (strcmp(payloadStr, "right") == 0) {
    Serial.println("Received 'right' command. Performing action...");
    rotateServo(servoRight);
    return;
  }
  if (strcmp (payloadStr, "stop") == 0){
    speed = 0;
    motorDirection = LOW;
    Movement (motorDirection, speed);
  }
  
}

void setup_WiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connect to ");
  Serial.println(STA_SSID);
  WiFi.begin(STA_SSID, STA_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
  }
  Serial.print("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY)) {
      Serial.println("connected");
      client.subscribe("oliver.sjodahl@hitachigymnasiet.se/Driverbot");
      break;
    } else {
      Serial.print("failed, reconnect=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      break;
    }
    break;
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(pwmMotorA, OUTPUT);
  pinMode(dirMotorA, OUTPUT);
  servo.attach(D1);
  servo.write(90);
  Serial.begin(115200);
  setup_WiFi();
  client.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  client.setCallback(handleMqttMessage);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void Movement(int motorDirection, int speed){
    digitalWrite(dirMotorA, motorDirection);
    analogWrite(pwmMotorA, speed);
}


void rotateServo(int servoDirection){
  servo.write(servoDirection);
}
