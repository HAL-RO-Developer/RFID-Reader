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
extern RFIDCONFIG rfid;
HOSTCONFIG hosts;

void upload( )
{
  ServerCommunication sc;
  /* サーバ接続 */
  if (!sc.connect(hosts.host, String(port)) == SERVER_CONNECT_ERROR) {
    Serial.println("connection failed");
  }
    
  /* リクエストJSONの作成 */
  String reqData = "{";
  reqData += "\"device_id\": \""+ internet.device_id +"\",";
  reqData += "\"uuid\": \""+ rfid.new_uuid +"\",";
  reqData += "\"old_uuid\": \""+ rfid.old_uuid+"\"";
  reqData += "}"; 
  
  String url = "/thing/reader";
  sc.post(url,reqData, hosts.host);  /* POST     */
  delay(300);
}

void registerDevice()
{
  digitalWrite(LED_PIN, HIGH);
  ServerCommunication sc;
  /* サーバ接続 */
  if (!sc.connect(hosts.host, String(port)) == SERVER_CONNECT_ERROR) {
    Serial.println("connection failed");
  }
   
  /*   リクエストJSONの作成 */
  String reqData = "{ \"pin\" : \""+ internet.pin +"\" }"; 
  String url = "/thing/registration";
  sc.post(url,reqData, hosts.host);  /* POST     */
  delay(300);
  digitalWrite(LED_PIN, HIGH);
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
  digitalWrite(LED_PIN, LOW);
}

void getWiFiConfig( )
{
  SCHR json_w[256];
  SCHR json_p[256];
  SCHR json_h[256];
  
  File   fd_w = SPIFFS.open( setting_w, "r" );
  String jsonStringW = fd_w.readString();
  fd_w.close();

  File   fd_p = SPIFFS.open( setting_p, "r" );
  String jsonStringP = fd_p.readString();
  fd_p.close();

  File   fd_h = SPIFFS.open( setting_h, "r" );
  String jsonStringH = fd_h.readString();
  fd_h.close();
  
  Serial.print(jsonStringW);
  Serial.print(jsonStringP);
  Serial.print(jsonStringH);
  
  jsonStringW.toCharArray( json_w, jsonStringW.length() + 1 );
  jsonStringP.toCharArray( json_p, jsonStringP.length() + 1 );
  jsonStringH.toCharArray( json_h, jsonStringH.length() + 1 );
  
  DynamicJsonBuffer jb;
  JsonObject& root_w = jb.parseObject( json_w );
  JsonObject& root_p = jb.parseObject( json_p );
  JsonObject& root_h = jb.parseObject( json_h );
  
  const SCHR* ssid = root_w["ssid"];
  const SCHR* pass = root_w["pass"];
  const SCHR* pin = root_p["pin"];
  const SCHR* dev = root_p["device_id"];
  const SCHR* host = root_h["host"];
  
  internet.ssid = String( ssid );
  internet.pass = String( pass );
  internet.pin = String( pin );
  internet.device_id = String( dev );
  hosts.host = String( host );
}

void connectRouter()
{
  getWiFiConfig();
  Serial.print("Connecting to ");
  Serial.println(internet.ssid.c_str());
  WiFi.begin( (internet.ssid).c_str(), (internet.pass).c_str() );
  WiFi.mode( WIFI_STA );

  while( WiFi.status() != WL_CONNECTED ){
    digitalWrite(LED_PIN, HIGH);
    delay( 400 );
    digitalWrite(LED_PIN, LOW);
    delay( 100 );
  }
  Serial.println("WiFi connected");
}

void setupWifi()
{
  digitalWrite(LED_PIN, HIGH);
  delay(10);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP( ap_ssid, ap_pass );
  
  server.on( "/", HTTP_GET, handleRootMain );
  server.on( "/wifi", HTTP_GET, handleGetWifi );
  server.on( "/wifi", HTTP_POST, handlePostWifi );
  server.on( "/pin", HTTP_GET, handleGetPin );
  server.on( "/pin", HTTP_POST, handlePostPin );
  server.on( "/host", HTTP_GET, handleGetHost );
  server.on( "/host", HTTP_POST, handlePostHost );
  server.begin();

  while(1){
    server.handleClient();
    delay(1);
  }
}

void handleRootMain()
{
  String html = "";
  html += "<h1>ESP8266 caseAESP Settings</h1>";
  html += "<a href=\"/wifi\"> WiFi Settings<br>";
  html += "<a href=\"/pin\"> Device Registration<br>";

  server.send( 200, "text/html", html );
}

void handleGetWifi()
{
  String html = "";
  html += "<h1>WiFi Settings</h1>";
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
  html += "<h1>WiFi Settings</h1>";
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
  html += "<h1>PIN Settings</h1>";
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
  html += "<h1>PIN Settings</h1>";
  html += "<p>Settings changed</p>";
  html += "<table>";
  html += "  <tr><td>PIN</td><td>" + pin + "</td></tr>";
  html += "  <tr><td></td><td><a href=\"/\"> Back</td></tr>";
  html += "</table>";
  server.send( 200, "text/html", html );
}

void handleGetHost()
{
  String html = "";
  html += "<h1>Host Settings</h1>";
  html += "<form method='post'>";
  html += "  HOST  : <input type='text' name='host' placeholder='hogehoge.com'><br>";
  html += "  <input type='submit'><br>";
  html += "<a href=\"/\"> Back<br>";
  html += "</form>";
  server.send( 200, "text/html", html );
}

void handlePostHost()
{
  String host = server.arg("host");
  
  // JSON作成
  String json = "{";
  json += "\"host\":\"" + host + "\"";
  json += "}";
    
  File    fd = SPIFFS.open( setting_h, "w" );
  fd.println( json );
  fd.close();
  
  String html = "";
  html += "<h1>Host Settings</h1>";
  html += "<p>Settings changed</p>";
  html += "<table>";
  html += "  <tr><td>HOST</td><td>" + host + "</td></tr>";
  html += "  <tr><td></td><td><a href=\"/\"> Back</td></tr>";
  html += "</table>";
  server.send( 200, "text/html", html );
}
