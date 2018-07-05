#include "ArduinoLibrary.h"
#include "ServerCommunication.h"
#include "function.h"
#include "constants.h"
#include "config.h"
#include "System.h"

ESP8266WebServer server( 80 );
IPAddress ip( 192, 168, 4, 1 );
IPAddress subnet( 255, 255, 255, 0 );
extern WIFICONFIG internet;
String mac_id;

SINT upload( String readerID )
{
  ServerCommunication sc;
  /* サーバ接続 */
  if (!sc.connect(String(host), String(port)) == SERVER_CONNECT_ERROR) {
    Serial.println("connection failed");
    return SYSTEM_NG;
  }
    
  /* リクエストJSONの作成 */
  String reqData = "{ \"device_id\": \""+ internet.device_id +"\", \"uuid\": \""+ readerID +"\"}"; 
  String url = "/thing/reader";
  sc.post(url,reqData, String(host));  /* POST     */
  delay(300);
}

SINT registerDevice()
{
  ServerCommunication sc;
  /* サーバ接続 */
  if (!sc.connect(String(host), String(port)) == SERVER_CONNECT_ERROR) {
    Serial.println("connection failed");
    return SYSTEM_NG;
  }
   
  /*   リクエストJSONの作成 */
  String reqData = "{ \"pin\" : \""+ internet.pin +"\", \"mac\" : \""+ mac_id +"\" }"; 
  String url = "/thing/registration";
  sc.post(url,reqData, String(host));  /* POST     */
  delay(300);
  sc.response(&internet.device_id);    /* RESPONSE */
  delay(300);
  
  /* デバイスＩＤを保存する */
  // JSON作成
  String json = "{";
  json += "\"pin\":\"\",";
  json += "\"device_id\":\"" + internet.device_id + "\"";
  json += "}";
    
  File    fd = SPIFFS.open( setting_p, "w" );
  fd.println( json );
  fd.close();
  Serial.println("Device Registered : " +internet.device_id);
}

void getWiFiConfig( )
{
  SCHR json_w[256];
  SCHR json_p[256];
  
  /* MACアドレス取得 */
  byte mac_byte[6];
  WiFi.macAddress( mac_byte );
  for( int i = 0; i < 6; i++ ){
      mac_id += String( mac_byte[i], HEX );
  }
  delay(1);
  
  File   fd = SPIFFS.open( setting_w, "r" );
  String jsonStringW = fd.readString();
  fd.close();

  File   fd_p = SPIFFS.open( setting_p, "r" );
  String jsonStringP = fd_p.readString();
  fd_p.close();
    
  Serial.print(jsonStringW);
  Serial.print(jsonStringP);
  jsonStringW.toCharArray( json_w, jsonStringW.length() + 1 );
  jsonStringP.toCharArray( json_p, jsonStringP.length() + 1 );
  
  DynamicJsonBuffer jb;
  JsonObject& root_w = jb.parseObject( json_w );
  JsonObject& root_p = jb.parseObject( json_p );
    
  const SCHR* ssid = root_w["ssid"];
  const SCHR* pass = root_w["pass"];
  const SCHR* pin = root_p["pin"];
  const SCHR* dev = root_p["device_id"];
    
  internet.ssid = String( ssid );
  internet.pass = String( pass );
  internet.pin = String( pin );
  internet.device_id = String( dev );
}

void connectRouter()
{
  getWiFiConfig();
  Serial.print("Connecting to ");
  Serial.println(internet.ssid.c_str());
  WiFi.begin( (internet.ssid).c_str(), (internet.pass).c_str() );
  WiFi.mode( WIFI_STA );

  SINT time;
  int start = millis();
  while( WiFi.status() != WL_CONNECTED ){
      Serial.print(".");
      delay( 500 );
  }
  Serial.println("WiFi connected");  
}

void setupWifi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP( ap_ssid, ap_pass );
  
  server.on( "/", HTTP_GET, handleRootMain );
  server.on( "/wifi", HTTP_GET, handleGetWifi );
  server.on( "/wifi", HTTP_POST, handlePostWifi );
  server.on( "/pin", HTTP_GET, handleGetPin );
  server.on( "/pin", HTTP_POST, handlePostPin );
  server.begin();

  while(1){
    server.handleClient();
    delay(1);
  }
}

void handleRootMain()
{
  String html = "";
  html += "<h1>ESP8266 caseSTUDY2018 Setting</h1>";
  html += "<a href=\"/wifi\"> WiFi Setting<br>";
  html += "<a href=\"/pin\"> Device Registration<br>";

  server.send( 200, "text/html", html );
}

void handleGetWifi()
{
  String html = "";
  html += "<h1>WiFi Setting</h1>";
  html += "<form method='post'>";
  html += "  SSID : <input type='text' name='ssid' placeholder='SSID'><br>";
  html += "  PASS : <input type='password' name='pass' placeholder='PASS'><br>";
  html += "  <input type='submit'><br>";
  html += "<a href=\"/\"> Back<br>";
  html += "</form>";
  
  server.send( 200, "text/html", html );
}

void handlePostWifi()
{ 
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  
  // JSON作成
  String json = "{";
  json += "\"ssid\":\"" + ssid + "\",";
  json += "\"pass\":\"" + pass + "\"";
  json += "}";
    
  File    fd = SPIFFS.open( setting_w, "w" );
  fd.println( json );
  fd.close();
    
  String html = "";
  html += "<h1>WiFi Setting</h1>";
  html += "<p>Settings changed</p>";
  html += "<table>";
  html += "  <tr><td>SSID</td><td>" + ssid + "</td></tr>";
  html += "  <tr><td>PASS</td><td>[Not display]</td></tr>";
  html += "  <tr><td></td><td><a href=\"/\"> Back</td></tr>";
  html += "</table>";
  
  server.send( 200, "text/html", html );
}

void handleGetPin()
{
  String html = "";
  html += "<h1>Device Registration</h1>";
  html += "<form method='post'>";
  html += "  PIN  : <input type='text' name='pin' placeholder='PIN'><br>";
  html += "  <input type='submit'><br>";
  html += "<a href=\"/\"> Back<br>";
  html += "</form>";
  server.send( 200, "text/html", html );
}

void handlePostPin()
{
  String pin = server.arg("pin");
  
  // JSON作成
  String json = "{";
  json += "\"pin\":\"" + pin + "\",";
  json += "\"device_id\":\"\"";
  json += "}";
    
  File    fd = SPIFFS.open( setting_p, "w" );
  fd.println( json );
  fd.close();
  
  String html = "";
  html += "<h1>Device Registration</h1>";
  html += "<p>Registrated by</p>";
  html += "<table>";
  html += "  <tr><td>PIN</td><td>" + pin + "</td></tr>";
  html += "  <tr><td></td><td><a href=\"/\"> Back</td></tr>";
  html += "</table>";
  server.send( 200, "text/html", html );
}

/* Copyright HAL College of Technology & Design */
