#include <string.h>             /* 初期化関連 */
#include "ArduinoLibrary.h"
#include "System.h"             /* システム共通データ定義ヘッダ */
#include "InfoStruct.h"         /* 情報管理構造体定義ヘッダ     */

#define APSWT   ( 0   )        /* APモードスイッチ */
#define SLPR    ( 4  )        /* ステータスランプ */

const   SCHR*   settings    = "/settings.json";
const   SCHR*   ap_ssid     = "ESP8266";
const   SCHR*   ap_pass     = "password";

ESP8266WebServer server( 80 );
IPAddress ip( 192, 168, 4, 1 );
IPAddress subnet( 255, 255, 255, 0 );

INFO_COMMON* common

/* --- プロトタイプ宣言 --- */
void deviceInit();
void getWiFiConfig( INFO_COMMON* common );
void setupStateAp( INFO_COMMON* common );
void handleRootGet();
void handleRootPost();

void setup()
{
  deviceInit();

  delay(3000);
  /* --- 初期状態設定 --- */
  Serial.println(digitalRead(APSWT));
  
  if(digitalRead(APSWT)==LOW){
    //setupStateAp(common);
    Serial.println("High");
  }
  Serial.println("if Passed");
}

void loop()
{
  server.handleClient();
}

/* --- 各種初期設定 --- */
void deviceInit()
{
  /* シリアル通信 */
  Serial.begin(115200);
  Serial.println();
  /* APスイッチ */
  pinMode( APSWT, INPUT );
  
  /* ファイルシステム */
  //SPIFFS.begin();   //<==これ
}

void setupStateAp( INFO_COMMON* common )
{
   /* MACアドレス取得 */
  byte mac_byte[6];
  WiFi.macAddress( mac_byte );
  for( int i = 0; i < 6; i++ ){
      common->mac += String( mac_byte[i], HEX );
  }
  
  getWiFiConfig( common );
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP( ap_ssid, ap_pass );
  Serial.println();     
  Serial.println(WiFi.softAPIP());  
  
  server.on( "/", HTTP_GET, handleRootGet );
  server.on( "/", HTTP_POST, handleRootPost );
  server.begin();
  
  delay(1);
}

void handleRootGet()
{
    String html = "";
    html += "<h1>WiFi Settings</h1>";
    html += "<form method='post'>";
    html += "  SSID : <input type='text' name='ssid' placeholder='SSID'><br>";
    html += "  PASS : <input type='password' name='pass' placeholder='PASS'><br>";
    html += "  Device ID  : <input type='text' name='dev' placeholder='DEV'><br>";
    html += "  <input type='submit'><br>";
    html += "</form>";
    
    server.send( 200, "text/html", html );
}

void handleRootPost()
{
    String  ssid = server.arg("ssid");
    String  pass = server.arg("pass");
    String  dev  = server.arg("dev");

    /* JSON作成 */
    String json = "{";
    json += "\"ssid\":\"" + ssid + "\",";
    json += "\"pass\":\"" + pass + "\",";
    json += "\"dev\":\""  + dev  + "\",";
    
    File    fd = SPIFFS.open( settings, "w" );
    fd.println( json );
    fd.close();
    
    String html = "";
    html += "<h1>WiFi Settings</h1>";
    html += "<p>Settings changed</p>";
    html += "<table>";
    html += "  <tr><td>SSID</td><td>" + ssid + "</td></tr>";
    html += "  <tr><td>PASS</td><td>[Not display]</td></tr>";
    html += "  <tr><td>DEV</td><td>" + dev + "</td></tr>";
    html += "</table>";
    server.send( 200, "text/html", html );
}

void getWiFiConfig( INFO_COMMON* common )
{
    SCHR json[256];
    
    File   fd = SPIFFS.open( settings, "r" );
    String jsonString = fd.readString();
    fd.close();

    Serial.println(jsonString);
    jsonString.toCharArray( json, jsonString.length() + 1 );

    DynamicJsonBuffer jb;
    JsonObject& root = jb.parseObject( json );

    const SCHR* ssid = root["ssid"];
    const SCHR* pass = root["pass"];
    const SCHR* dev  = root["dev"];

    (common->config).ssid = String( ssid );
    (common->config).pass = String( pass );
    (common->config).device_id = String( dev );

#if DEBUG 
    Serial.println("SSID:" + (common->config).ssid);
    Serial.println("PASS:" + (common->config).pass);
    Serial.println("DEV :" + (common->config).dev);
#endif    
}

/* Copyright HAL College of Technology & Design */

