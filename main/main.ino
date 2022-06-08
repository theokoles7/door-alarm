// AUTHOR:    Gabriel C. Trahan
// DATE:      03 June 2022

// ============================================================ IMPORTS
#include <WiFi.h>
#include <HTTPClient.h>

// ============================================================= VARS
// PINS
#define PIN_SWITCH_REED   14                                  // Door sensor pin
#define PIN_SPEAKER       25                                  // Speaker pin
#define PIN_SWITCH_ARM    13                                  // Arming switch
#define PIN_LED_ARM       26                                  // Arming status
#define PIN_LED_DOOR      33                                  // Door status
#define PIN_LED_WIFI      32                                  // WiFi status

// DOOR STATES
int DOOR_STATE_CUR;                                           // Current door state
int DOOR_STATE_PRE;                                           // Precious door state

// CONFIGURABLES
String DOOR_NAME =    "Front";                                // Name of door to be sent in notification
char* SSID = (char*)  "SANCTUM";                              // Wifi SSID
char* PSWD = (char*)  "Jessi712189512";                       // Wifi Password

// ============================================================= SETUP
void setup(){
  Serial.begin(9600);                                         // Initialize serial monitor

  // ----------------------------------------------------------- Initialize pin modes
  pinMode(PIN_SWITCH_REED,  INPUT_PULLUP);
  pinMode(PIN_SWITCH_ARM,   INPUT_PULLUP);
  pinMode(PIN_SPEAKER,      OUTPUT);
  pinMode(PIN_LED_ARM,      OUTPUT);
  pinMode(PIN_LED_DOOR,     OUTPUT);
  pinMode(PIN_LED_WIFI,     OUTPUT);

  connectWiFi(SSID, PSWD);                                    // Connect to Wifi
}

// ============================================================= CYCLE OF OPS
void loop(){
  while(digitalRead(PIN_SWITCH_ARM) == LOW){                  // If sensor is armed...
    digitalWrite(PIN_LED_ARM, HIGH);                          // Turn on arming light
    DOOR_STATE_PRE = DOOR_STATE_CUR;                          // Assign current state to previous state
    DOOR_STATE_CUR = digitalRead(PIN_SWITCH_REED);            // Read new current state
    digitalWrite(PIN_LED_WIFI, WiFi.status() == WL_CONNECTED);// Maintain communication of Wifi status

    if(DOOR_STATE_CUR == HIGH && DOOR_STATE_PRE == LOW){      // If door was just opened...
      soundAlarm();                                           // Sound the alarm
    }

    if(WiFi.status() != WL_CONNECTED){                        // If Wifi gets disconnected...
      Serial.println("Wifi disconnected, hold on...");        // Communicate issue
      connectWiFi(SSID, PSWD);                                // And reconnect
    }

    delay(1000);                                              // Wait one second to avoid multiple triggers
  }
  digitalWrite(PIN_LED_ARM, LOW);                             // Turn off arming light
}

// ============================================================= METHODS

// ------------------------------------------------------------- Connect to Wifi
void connectWiFi(char* SSID, char* PSWD){
  WiFi.begin(SSID, PSWD);
  while(WiFi.status() != WL_CONNECTED){
    digitalWrite(PIN_LED_WIFI, HIGH);
    delay(100);
    Serial.print(".");
    digitalWrite(PIN_LED_WIFI, LOW);
    delay(100);
  }
  Serial.println("");
  Serial.println("Wifi connected");
}

// ------------------------------------------------------------- Sound alarm
void soundAlarm(){
  Serial.println("Door opened");                              // Communicate event
  notify(DOOR_NAME);                                          // Send notification
  while(digitalRead(PIN_SWITCH_REED) == HIGH){                // Until the door is closed again...
    Serial.print(".");
    digitalWrite(PIN_LED_DOOR, HIGH);                         // WEE
    digitalWrite(PIN_SPEAKER, HIGH);
    delay(250);
    digitalWrite(PIN_LED_DOOR, LOW);                          // WOO
    digitalWrite(PIN_SPEAKER, LOW);
    delay(250);
  }
  Serial.println("");
  Serial.println("Door closed");                              // Communicate event
}

// ------------------------------------------------------------- Send notification
void notify(String DOOR_NAME){
  HTTPClient http;                                            // Initialize HTTP Client object
  http.begin(                                                 // Build request
    "http://maker.ifttt.com"                                  // URL
    "/trigger/door_open/with/key/"                            // Request
    "iHMMnkh_6PehIF-Yre60JJ8NtPfo_cpKS4ng1l2nR7D"             // API Key
    "/?value1=" + DOOR_NAME                                   // Value1 = <DOOR_NAME>
    );
  http.GET();                                                 // Make request
  Serial.println("Notification sent");                        // Communicate success
}
