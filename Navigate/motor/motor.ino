#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <Servo.h>
#include <PubSubClient.h>
#include <stddef.h>

#define pwmMotorA 4 // D2
#define dirMotorA 0 // D3
// WiFi Credentials
#define STA_SSID "ABBgym_2.4"
#define STA_PASS "mittwifiarsabra"
// MQTT Credentials 
#define MQTT_USERNAME "oliver.sjodahl@hitachigymnasiet.se"
#define MQTT_KEY "reesesPuffs"
#define MQTT_BROKER "maqiatto.com"
#define MQTT_BROKER_PORT 1883

String clientId = "IoTPractice-" + String(ESP.getChipId());

int speed = 0;
int motorDirection = HIGH; // HIGH = forward, LOW = backwards

int servoRight = 140;
int servoLeft = 40;

const int maxCommands = 40;
struct { String str; size_t time; } recordCommand[maxCommands];
bool recording = false;
int commandIndex = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Servo servo;

static unsigned long int lastMillis = 0;

/* TODO:
Connect to MQTT and send messages | DONE
Movement code | DONE
Extra function (drive back to original position when record button was initiated) | DONE
Add necessary comments ONLY | DONE */

// HAR ÄNDRAT LITE SAKER HÄR OCH I VS CODE UTAN ATT KUNNAT TESTA, HOPPAS DET FUNKAR

void handleMqttMessage(char* topic, byte* payload, unsigned int length) {
  char payloadStr[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    payloadStr[i] = (char)payload[i];
  }
  payloadStr[length] = '\0'; 
  
  Serial.print("Received command: ");
  Serial.println(payloadStr);
  if (strcmp(payloadStr, "forward") == 0){
    Serial.println("Performing 'forward' action...");
    servo.write(90);
    motorDirection = HIGH;
    speed = 100;
    Movement(motorDirection, speed);
    if (recording && (commandIndex < maxCommands)) {
      // drive opposite
      recordCommand[commandIndex++] = { "backwards", millis() - lastMillis }; 
    }
  }

  else if (strcmp(payloadStr, "backwards") == 0) {
    Serial.println("Performing 'backwards' action...");
    motorDirection = LOW;
    speed = 100;
    Movement(motorDirection, speed);
    if (recording && (commandIndex < maxCommands)) {
      recordCommand[commandIndex++] = { "forward", millis() - lastMillis };
    }
  }

  else if (strcmp(payloadStr, "left") == 0) {
    Serial.println("Performing 'left' action...");
    rotateServo(servoLeft);
    if (recording && (commandIndex < maxCommands)) {
      recordCommand[commandIndex++] = { "left", millis() - lastMillis };
    }
  }

  else if (strcmp(payloadStr, "right") == 0) {
    Serial.println("Performing 'right' action...");
    rotateServo(servoRight);
    if (recording && (commandIndex < maxCommands)) {
      recordCommand[commandIndex++] = { "right", millis() - lastMillis }; 
    }
  }

  else if (strcmp(payloadStr, "stop") == 0) {
    stopMovement();
    if (recording && (commandIndex < maxCommands)) {
      recordCommand[commandIndex++] = { "stop", millis() - lastMillis };
    }
  }

  else if (strcmp(payloadStr, "record") == 0) {
    commandIndex = 0;
    recordCommand[commandIndex++] = { "stop", 0 };
    lastMillis = millis();
    recording = true;
    Serial.println("Recording started...");
  }
  else if (strcmp(payloadStr, "play") == 0) {
  recording = false;
  Serial.println("Playing back recorded commands...");
  playCommandsReverse();
  }

  lastMillis = millis();
}

// play back the list of commands
void playCommand(typeof(*recordCommand) command) {
  Serial.printf("Playing back command of type '%s'\n", command.str);
  if (command.str == "forward") {
    servo.write(90);
    motorDirection = HIGH;
    speed = 100;
    Movement(motorDirection, speed);
  } else if (command.str == "backwards") {
    motorDirection = LOW;
    speed = 100;
    Movement(motorDirection, speed);
  } else if (command.str == "left") {
    rotateServo(servoLeft);
  } else if (command.str == "right") {
    rotateServo(servoRight);
  }
  else if (command.str == "stop"){
    stopMovement();
  }
  delay(command.time);
}

// run extra function
void playCommandsReverse() {
  for (int i = commandIndex - 1; i >= 0; i--) {
    typeof(*recordCommand) command = recordCommand[i];
    playCommand(command);
  }
}

// Connect to WiFi
void setup_WiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
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

// Reconnect to MQTT server
void reconnect(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY)) {
      Serial.println("connected");
      client.subscribe("oliver.sjodahl@hitachigymnasiet.se/Driverbot");
    } else {
      Serial.print("failed, reconnect=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(pwmMotorA, OUTPUT);
  pinMode(dirMotorA, OUTPUT);

  servo.attach(D1);
  servo.write(90);

  Serial.begin(115200);
  setup_WiFi();
  client.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  client.setCallback(handleMqttMessage);

  commandIndex = 0;
  recordCommand[commandIndex++] = { "stop", 0 };
}

void loop()
{
  // Stay connected
  int state;
  if ((state = client.state()) != MQTT_CONNECTED) {
    Serial.printf("MQTT Reconnecting due to status: %d\n", state);
    reconnect();

    if ((state = client.state()) != MQTT_CONNECTED) {
      Serial.printf("MQTT Connection Error: %d\n", state);
      delay(4000);
      goto POST_CLIENT_LOOP;
    }
  }
  client.loop();
POST_CLIENT_LOOP:
  delayMicroseconds(16);
}

// horizontal movement
void Movement(int motorDirection, int speed){
  digitalWrite(dirMotorA, motorDirection);
  analogWrite(pwmMotorA, speed);
}

void stopMovement() {
  speed = 0;
  motorDirection = LOW;
  Movement(motorDirection, speed);
}

void rotateServo(int servoDirection){
  servo.write(servoDirection);
}
