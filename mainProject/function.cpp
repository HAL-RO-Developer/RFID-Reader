#include "function.h"

//  WIFI
void WiFiSetup( void )
{
  const char* ssid     = "...";
  const char* password = "...";

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
  const char* host = "... .ngrok.io"; //ngrok
  const int port = 443;  //https
  
  ServerCommunication sc;
  
  // Use WiFiClient class to create TCP connections
  if (!sc.connect(String(host), String(port)) == SERVER_CONNECT_ERROR) {
    Serial.println("connection failed");
    return SYSTEM_NG;
  }
    
  /* --- リクエストJSONの作成 --- */
  String reqData = "{ \"foo\" : \" "+ readerID +" \" }"; 
  String url = "/api/foo";

  //Fingerprint は sc.post に入っている
  sc.post("/api/foo",reqData, String(host)); //POST処理
  delay(300);
  Serial.println("POST Done");
}

