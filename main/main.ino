// AUTHOR:  Gabriel C. Trahan
// DATE:    03 June 2022

// ============================================================ IMPORTS
#include <WiFi.h>
#include <HTTPClient.h>

// ============================================================= VARS
// PINS
#define PIN_SWITCH_REED   14                                  // Door sensor pin
#define PIN_LED_DOOR      33                                  // Door status
#define PIN_LED_WIFI      32                                  // WiFi status

// DOOR STATES
int DOOR_STATE_CUR;                                           // Current door state
int DOOR_STATE_PRE;                                           // Previous door state

// CONFIGURE
String DOOR_NAME = "Front";                                   // Name of door to be sent in notification
char* SSID = "";                                              // Wifi SSID
char* PSWD = "";                                              // Wifi Pwassword

// ============================================================= SETUP
void setup(){
  Serial.begin(9600);                                         // Initialize serial monitor
  
  // ----------------------------------------------------------- Initialize pin modes
  pinMode(PIN_SWITCH_REED, INPUT_PULLUP);
  pinMode(PIN_LED_DOOR, OUTPUT);
  pinMode(PIN_LED_WIFI, OUTPUT);

  connectWiFi(SSID, PSWD);                                    // Connect to Wifi
}

// ============================================================= CYCLE OF OPS
void loop(){
  DOOR_STATE_PRE = DOOR_STATE_CUR;                            // Assign current state to previous state
  DOOR_STATE_CUR = digitalRead(PIN_SWITCH_REED);              // Read new current state
  digitalWrite(PIN_LED_WIFI, WiFi.status() == WL_CONNECTED);  // Maintain communication of Wifi status

  if(DOOR_STATE_CUR == HIGH && DOOR_STATE_PRE == LOW){        // If door was just opened...
    Serial.println("Door opened");                            // Communicate event
    digitalWrite(PIN_LED_DOOR, HIGH);                         // Communicate status
    notify(DOOR_NAME);                                        // Send notification
  }
  else if(DOOR_STATE_CUR == LOW && DOOR_STATE_PRE == HIGH){   // If door was just closed...
    Serial.println("Door closed");                            // Communicate event
    digitalWrite(PIN_LED_DOOR, LOW);                          // Communicate status
  }

  if(WiFi.status() != WL_CONNECTED){                          // If Wifi was disconnected...
    Serial.println("Wifi disconnected...");
    connectWiFi(SSID, PSWD);                                  // Reconnect
  }

  delay(1000);                                                // Wait 1 second to prevent multiple triggers
}

// ============================================================= METHODS

// ------------------------------------------------------------- Connect to Wifi
void connectWiFi(char* SSID, char* PSWD){
  WiFi.begin(SSID, PSWD);                                     // Initialize WiFi object
  while(WiFi.status() != WL_CONNECTED){                       // While waiting for Wifi to connect
    digitalWrite(PIN_LED_WIFI, HIGH);                         // Flash Wifi LED on
    delay(250);
    Serial.print(".");                                        // Communicate action
    digitalWrite(PIN_LED_WIFI, LOW);                          // Flash Wifi LED off
    delay(250);
  }
  Serial.println("");
  Serial.println("Wifi connected!");                          // Communicate success
}

// ------------------------------------------------------------- Send notification
void notify(String DOOR_NAME){
  HTTPClient http;                                            // Initialize HTTP Client object
  http.begin(                                                 // Build request
    "http://maker.ifttt.com"                                  // URL
    "/trigger/door_status/with/key/"                          // Request
    "<INSERT_KEY>"                                            // API Key
    "/?value1=" + DOOR_NAME                                   // Value1 = <DOOR_NAME>
    );
  http.GET();                                                 // Make request
  Serial.println("Notification sent");                        // Communicate success
}
