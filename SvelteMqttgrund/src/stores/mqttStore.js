import mqtt from 'mqtt';
import { isConnected } from './store';
let connected = false;
let User = { adress: "maqiatto.com", port: "8883", clientId: "clientID_3", name: "oliver.sjodahl@hitachigymnasiet.se", password: "reesesPuffs" };
let clientId = "notyetAssigned";
let client = null;
let options = {};
 
/**
 * Disconnects the MQTT client.
 * @returns {boolean} Returns false.
 */
export function startDisconnect() {
  client.disconnect();
  return false;
}
 
/**
 * Connects the MQTT client.
 * @returns {string} Returns an empty string if already connected.
 */
export function startConnect() {
  if (connected) {
    return "";
  }
  clientId = "ElCliento" + parseInt(Math.random() * 100);
  const mqtt_url = User.adress;
  const url = "mqtt://" + mqtt_url;
  options = {
    port: User.port,
    clientId: clientId,
    username: User.name,
    password: User.password
  };
  client = mqtt.connect(url, options);
  client.on('message', onMessage);
 
  client
    .on("connect", function () {
      connected = client.connected;
      console.log(connected);
      console.log(clientId + " success");
      Connecting(true);
    })
    .on("error", function () {
      console.log("error");
      Connecting(false);
    })
    .on("close", function () {
      Connecting(false);
      console.log("closing");
    });
}
 
/**
 * Handles the incoming MQTT message.
 * @param {string} topic - The topic of the message.
 * @param {Buffer} message - The message payload.
 * Here you can make a client that listens to a topic and then does something with the message.
 */
const onMessage = (topic, message) => {
  console.log(message);
  console.log(message.toString());
};
 
/**
 * Sets the connection status and sends a log message.
 * @param {boolean} c - The connection status.
 */
function Connecting(c) {
  isConnected.set(c);
  onSend("logger", clientId + " har anslutits.");
}
 
/**
 * Publishes a message to the MQTT broker.
 The address to publish to.
 * @param {string} message - The message to publish.
 */
 
export function onSend(adress, message) {
  let address = "Driverbot"
  client.publish(
    User.name + "/" + address,
    message
  );
  onMessage(adress, message);
}
 
/**
 * Handles the arrived MQTT message.
 * @param {Object} message - The arrived message object.
 * @returns {string} Returns the payload string of the message.
 */
export function onMessageArrived(message) {
  console.log("onMessageArrived: " + message.payloadString);
  return message.payloadString;
}
