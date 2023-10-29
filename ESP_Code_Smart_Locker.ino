//This line sets up the environment for an ESP32-, It includes necessary libraries for working with Wi-Fi, BLE, and Blynk 
#define BLYNK_TEMPLATE_ID "TMPL3Pxd46BAK"
#define BLYNK_TEMPLATE_NAME "node"
#include <Arduino.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//WIFI_NAME and WIFI_PASSWORD with our actual Wi-Fi network credentials, and  should replace the blynkAuthToken placeholder with our own
 //Blynk authentication token for successful communication with the Blynk server. The CONNECTION_TIMEOUT value can be adjusted as needed to accommodate 
 //your application's network connection requirements.
const char* WIFI_NAME;
const char* WIFI_PASSWORD;
const char* blynkAuthToken = "uaxr4a1XD2dABHkZPk2Ibrk0tStZcSIE";
const int CONNECTION_TIMEOUT = 10; // Adjust the timeout value as needed
//this code sets up the server to handle HTTP requests on port 80, initializes pointers for managing BLE server and characteristics,and
//prepares the serial communication
WiFiServer server(80);

BLEServer* pServer;
BLECharacteristic* pCharacteristic;
bool isBLEConnected = false;

void setup() {
  Serial.begin(115200);
//this line of code configures the ESP32 as a BLE device with the name "Locker1". It creates a BLE server,defines a service with a specific UUID
// and adds a characteristic to that service, which can be read, written, and used to send notifications.
  BLEDevice::init("Locker1"); // BLE device name
  pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"));
  pCharacteristic = pService->createCharacteristic(
                      BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"),
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
//the provided line configures a BLE service and characteristic, retrieves advertising settings, and starts advertising the ESP32as a BLE device.              
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  //createtask();
  connectwifi();
  Blynk.begin(blynkAuthToken, WIFI_NAME, WIFI_PASSWORD, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();

  if (isBLEConnected) {
    // Handle BLE communication here
  }
}
//the getpass() function initializes the WIFI_NAME and WIFI_PASSWORD variables with predefined values,such as the network name "Harry" and
//the network password "#pass123#."
void getpass() {
  char tempchr[10];
  String tempstr;

  WIFI_NAME = NULL;
  tempchr[0] = '\0';
  tempstr = "USERID";

  WIFI_NAME = "your_wifi_name";
  WIFI_PASSWORD = NULL;
  tempchr[0] = '\0';
  tempstr = "PASSWORD";

  WIFI_PASSWORD = "your_wifi_password";
}
//The connectwifi() function essentially starts by printing a message indicating the network it's connecting to, then calls getpass() 
//to set the Wi-Fi credentials, and finally enters a loop that continuously checks the Wi-Fi connection status.
void connectwifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  getpass();
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  int timeout_counter = 0;
  int status;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
    status = WiFi.status();
  // repeatedly checks the Wi-Fi connection status
    Serial.println(get_wifi_status(status));
    timeout_counter++;
  //If the connection attempt takes too long, the code will handle this situation by restarting the ESP32.
    if (timeout_counter >= CONNECTION_TIMEOUT * 5) {
      Serial.println("Net failed - restarting the ESP32");
      delay(5000);
      ESP.restart();
    }
  }
  // A successful connection is established, the code proceeds to provide additional information about the connected network
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}



String get_wifi_status(int status) {
  // Implementation of get_wifi_status() function
}
