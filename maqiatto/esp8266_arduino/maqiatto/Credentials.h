#ifndef ARDUINO_CREDENTIALS_H
#define ARDUINO_CREDENTIALS_H

/* WiFi Credentials to connect Internet */
#define STA_SSID "ABBgym_2.4"
#define STA_PASS "mittwifiarsabra"

/* Provide MQTT broker credentials as denoted in maqiatto.com. */
#define MQTT_BROKER       "maqiatto.com"
#define MQTT_BROKER_PORT  1883
#define MQTT_USERNAME     "oliver.sjodahl@hitachigymnasiet.se"
#define MQTT_KEY          "reesesPuffs"


/* Provide topic as it is denoted in your topic list. 
 * For example mine is : cadominna@gmail.com/topic1
 * To add topics, see https://www.maqiatto.com/configure
 */
#define TOPIC    "oliver.sjodahl@hitachigymnasiet.se/Driverbot"

#endif /* ARDUINO_CREDENTIALS_H */
