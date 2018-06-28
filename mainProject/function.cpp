#include "function.h"

//  WIFI
void WiFiSetup( void )
{
  const char* ssid     = "logitec-00f466";
  const char* password = "9052796934449";

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("WiFi connected");
}

SINT upload( String readerID )
{
  //HTTPS-Server
  const char* host = "74a50909.ngrok.io"; //ngrok
  const int port = 443;  //https
  
  ServerCommunication sc;
  
  // Use WiFiClient class to create TCP connections
  if (!sc.connect(String(host), String(port)) == SERVER_CONNECT_ERROR) {
    Serial.println("connection failed");
    return SYSTEM_NG;
  }
    
  /* --- リクエストJSONの作成 --- */
  String reqData = "{ \"device_id\": \"NxuCgLt49qyc\", \"data\": { \"book_id\": 1, \"q_no\": 1, \"solution\": 1}}"; 
  String url = "/work/reader";

  //Fingerprint は sc.post に入っている
  sc.post(url,reqData, String(host)); //POST処理
  delay(300);
  Serial.println("POST Done");
}

