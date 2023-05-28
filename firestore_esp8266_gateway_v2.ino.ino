#include <ESP8266WiFi.h>
#include <SPI.h>
#include <LoRa.h>

#include <Firebase_ESP_Client.h>
// #include <FirebaseESP8266.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/*-----------------------------------------------------
Node MCU Pins                             SX1278 Pins
D8                                        NSS
D7                                        MOSI
D6                                        MISO
D5                                        SCK
D0                                        RST
D2                                        DIO0
------------------------------------------------------*/

// Define the WiFi
#define WIFI_SSID "Chi Hieu"
#define WIFI_PASSWORD "hieunghia"

// Define the API Key
#define API_KEY "AIzaSyDUuHWyF_LW-01LPW5ABgTzqrqStjn_jMs"

// Define the project ID
#define FIREBASE_PROJECT_ID "fluttertest-28d13"

// Define the user Email and password that alreadey registerd or added in the project
#define USER_EMAIL "hanlun04@gmail.com"
#define USER_PASSWORD "newyork424"

//Define LoRa Pins
#define ss 15 //D8
#define reset 16 //D0
#define dio0 4 //D2

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count1 = 0;
int count2 = 0;

// Define LoRa object
int dataLoraOK;
String dataSendFB, string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8;
String string_9, string_10, string_11, string_12, string_13, string_14, string_15, string_16;
String relay1, relay2;
byte comma;
unsigned long t1 = 0;

// The Firestore payload upload callback function
// void fcsUploadCallback(CFS_UploadStatusInfo info)
// {
//     if (info.status == fb_esp_cfs_upload_status_init)
//     {
//         Serial.printf("\nUploading data (%d)...\n", info.size);
//     }
//     else if (info.status == fb_esp_cfs_upload_status_upload)
//     {
//         Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
//     }
//     else if (info.status == fb_esp_cfs_upload_status_complete)
//     {
//         Serial.println("Upload completed ");
//     }
//     else if (info.status == fb_esp_cfs_upload_status_process_response)
//     {
//         Serial.print("Processing the response... ");
//     }
//     else if (info.status == fb_esp_cfs_upload_status_error)
//     {
//         Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
//     }
// }

void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /*Assign the callback function for the long running token generation task*/
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  fbdo.setBSSLBufferSize(2048, 2048);

  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  /* LoRa Init */
  LoRa.setPins(ss, reset, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}

void loop()
{
  if (Firebase.ready() && dataLoraOK == 1)
  {
    FirebaseJson content;

    String documentPath;

    string_0 = splitString(dataSendFB, ",", 0);

    if(string_0 == "1"){

      documentPath = "Node1_Data/Doc" + String(count1);
      
      string_1 = splitString(dataSendFB, ",",1);
      content.set("fields/Node1/mapValue/fields/Temperature/stringValue", string_1);
      string_2 = splitString(dataSendFB, ",", 2);
      content.set("fields/Node1/mapValue/fields/Humidity/stringValue", string_2);
      string_3 = splitString(dataSendFB, ",", 3);
      content.set("fields/Node1/mapValue/fields/Voltage/stringValue", string_3);
      string_4 = splitString(dataSendFB, ",", 4);
      content.set("fields/Node1/mapValue/fields/Current/stringValue", string_4);
      string_5 = splitString(dataSendFB, ",", 5);
      content.set("fields/Node1/mapValue/fields/Power/stringValue", string_5);
      string_6 = splitString(dataSendFB, ",", 6);
      content.set("fields/Node1/mapValue/fields/Energy/stringValue", string_6);
      string_7 = splitString(dataSendFB, ",", 7);
      content.set("fields/Node1/mapValue/fields/Frequency/stringValue", string_7);
      string_8 = splitString(dataSendFB, ",", 8);
      content.set("fields/Node1/mapValue/fields/PowerFactor/stringValue", string_8);
      dataLoraOK = 0;

      count1 ++;

    }

    if(string_0 == "2"){

      documentPath = "Node2_Data/Doc" + String(count2);

      string_9 = splitString(dataSendFB, ",",1);
      content.set("fields/Node2/mapValue/fields/Temperature/stringValue", string_9);
      string_10 = splitString(dataSendFB, ",", 2);
      content.set("fields/Node2/mapValue/fields/Humidity/stringValue", string_10);
      string_11 = splitString(dataSendFB, ",", 3);
      content.set("fields/Node2/mapValue/fields/Voltage/stringValue", string_11);
      string_12 = splitString(dataSendFB, ",", 4);
      content.set("fields/Node2/mapValue/fields/Current/stringValue", string_12);
      string_13 = splitString(dataSendFB, ",", 5);
      content.set("fields/Node2/mapValue/fields/Power/stringValue", string_13);
      string_14 = splitString(dataSendFB, ",", 6);
      content.set("fields/Node2/mapValue/fields/Energy/stringValue", string_14);
      string_15 = splitString(dataSendFB, ",", 7);
      content.set("fields/Node2/mapValue/fields/Frequency/stringValue", string_15);
      string_16 = splitString(dataSendFB, ",", 8);
      content.set("fields/Node2/mapValue/fields/PowerFactor/stringValue", string_16);
      dataLoraOK = 0;

      count2++;
    }

    // timestamp
        // content.set("fields/myTimestamp/timestampValue", fbdo.to<int>());

    Serial.print("Create a document... ");

    if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else
        Serial.println(fbdo.errorReason());
  }
}

void LoRa_rxMode(){
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  
  dataSendFB = message;
  Serial.print("Gateway Receive: ");
  Serial.println(message);
  dataLoraOK = 1;

  Serial.print("Gateway Receive: ");
  Serial.println(message);
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

String splitString(String str, String strRe, int posStr)
{
  String temp = str;
  for(int i = 0; i < posStr; i++)
  {
    temp = temp.substring(temp.indexOf(strRe) + 1);
    // if(temp.indexOf(strRe)== -1 && i != posStr -1 )
    // {
    //   return "";
    // }

  }
  return temp.substring(0, temp.indexOf(strRe));
}