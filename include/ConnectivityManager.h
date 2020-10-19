#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEFAULT_SSID "TOPNETE136FE54"
#define DEFAULT_PSWD "F57B95D1E6"
#define DEFAULT_MQTT "broker.hivemq.com"


void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;
PubSubClient client(DEFAULT_MQTT, 1883, callback, wifiClient);
byte* payloadCallback;

class ConnectivityManager{

private:

protected:

public:


    // Wifi SSID
    char* ssid = DEFAULT_SSID;
    // Wifi Password
    char* password =  DEFAULT_PSWD;
    // MQTT Server
    char* MQTT_Server = DEFAULT_MQTT;

    /* Server Data Start*/
    WiFiServer Server = WiFiServer(301);
    IPAddress IP = IPAddress(192, 168, 43, 67);
    IPAddress Gateway = IPAddress(192, 168, 43, 1);
    IPAddress Subnet = IPAddress(255, 255, 255, 0);
    IPAddress DNS = IPAddress(192, 168, 43, 1);
    /* Server Data End*/

    ConnectivityManager(){
        // Default constructor instructions here
    }

    ConnectivityManager(char* newSsid, char* newPassword){
        ssid = newSsid;
        password = newPassword;
    }

    ConnectivityManager(char* &newSsid, char* newPassword, const uint16_t& serverPort){
        ssid = newSsid;
        password = newPassword;
        Server = WiFiServer(serverPort);
    }

    // Connect to the Wifi using the ssid and the password already set in this class
    void connect(){
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.config(IP, DNS, Gateway, Subnet);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("--\nWiFi connected.");
    }

    // Run a server using the Data already set in this class
    void runServer(){
        Server.begin();
        Serial.println("Server started");
        // Print the IP address
        Serial.print("Listening on : ");
        Serial.print("http://");
        Serial.print(WiFi.localIP());
        Serial.print(":");
        Serial.print(Server.port());
    }

    bool connectToMQTT(const char* clientName, const char* topic){
        if(WiFi.status() == WL_CONNECTED){
            while (!client.connected()) {
                
                Serial.print("Attempting MQTT connection...");

                //if connected, subscribe to the topic(s) we want to be notified about
                if (client.connect(clientName)) {
                    Serial.print("\tMTQQ Connected");
                    client.subscribe(topic);
                    return true;
                }
                //otherwise print failed for debugging
                else {
                    Serial.println("\tFailed MQTT."); abort();
                    return false;
                }
            }
        }
        return false;
    }

    void sendMQTTMessage(const char* topic, const char* messageToSend){
        client.publish(topic, messageToSend, true);
    }


};

void callback(char* topic, byte* payload, unsigned int length) {
    payloadCallback = payload;
}

