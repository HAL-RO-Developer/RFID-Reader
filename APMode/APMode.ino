
#include <string.h>             /* 初期化関連 */
#include "ArduinoLibrary.h"
#include "System.h"             /* システム共通データ定義ヘッダ */
#include "InfoStruct.h"         /* 情報管理構造体定義ヘッダ     */

#define APSWT   ( 0   )        /* APモードスイッチ */
#define SLPR    ( 4  )        /* ステータスランプ */
#define TIMEOUT ( 30000 )

const   SCHR*   settings    = "/settings.json";
const   SCHR*   ap_ssid     = "caseAESP";
const   SCHR*   ap_pass     = "password";

WIFICONFIG internet;
ESP8266WebServer server( 80 );
IPAddress ip( 192, 168, 4, 1 );
IPAddress subnet( 255, 255, 255, 0 );

/* --- プロトタイプ宣言 --- */
void deviceInit();
void handleRootGet();
void handleRootPost();
void setupStateAp();
void getWiFiConfig();
void connectRouter();

void setup()
{
  deviceInit();
  Serial.println("wait 3second for AP button");
  Serial.println("Pushed = AP, Release = none ");
  delay(3000); //ボタン待ち

  if(digitalRead(APSWT)==LOW){
    Serial.println("Button is LOW");
    setupStateAp();
  } else {
    Serial.println("Button is HIGH");
    connectRouter();
  }

  delay(100);
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

  /* MACアドレス取得 */
  byte mac_byte[6];
  WiFi.macAddress( mac_byte );
  for( int i = 0; i < 6; i++ ){
      internet.mac += String( mac_byte[i], HEX );
  }
  
  Serial.println("taken MAC");
  delay(1);
  /* ファイルシステム */
  SPIFFS.begin();
}

void handleRootGet()
{
    String html = "";
    html += "<h1>WiFi Settings</h1>";
    html += "<form method='post'>";
    html += "  SSID : <input type='text' name='ssid' placeholder='SSID'><br>";
    html += "  PASS : <input type='password' name='pass' placeholder='PASS'><br>";
    html += "  Device ID  : <input type='text' name='device_id' placeholder='Device ID'><br>";
    html += "  <input type='submit'><br>";
    html += "</form>";
    
    server.send( 200, "text/html", html );
}

void handleRootPost()
{
    String  ssid = server.arg("ssid");
    String  pass = server.arg("pass");
    String  device_id  = server.arg("device_id");

    // JSON作成
    String json = "{";
    json += "\"ssid\":\"" + ssid + "\",";
    json += "\"pass\":\"" + pass + "\",";
    json += "\"device_id\":\""  + device_id  + "\",";
    
    File    fd = SPIFFS.open( settings, "w" );
    fd.println( json );
    fd.close();
    
    
    String html = "";
    html += "<h1>WiFi Settings</h1>";
    html += "<p>Settings changed</p>";
    html += "<table>";
    html += "  <tr><td>SSID</td><td>" + ssid + "</td></tr>";
    html += "  <tr><td>PASS</td><td>[Not display]</td></tr>";
    html += "  <tr><td>Device ID</td><td>" + device_id + "</td></tr>";
    html += "</table>";
    server.send( 200, "text/html", html );
}

void setupStateAp()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP( ap_ssid, ap_pass );
  Serial.println();     
  Serial.println(WiFi.softAPIP()); 
  
  server.on( "/", HTTP_GET, handleRootGet );
  server.on( "/", HTTP_POST, handleRootPost );
  server.begin();
}

void getWiFiConfig( )
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
    const SCHR* device_id = root["device_id"];

    internet.ssid = String( ssid );
    internet.pass = String( pass );
    internet.device_id = String( device_id );

#if DEBUG 
    Serial.println("SSID:" + internet.ssid);
    Serial.println("PASS:" + internet.pass);
    Serial.println("DEV :" + internet.device_id);
#endif    
}

void connectRouter()
{
    Serial.print("Connecting to ");
    Serial.println(internet.ssid.c_str());
    WiFi.begin( internet.ssid.c_str(), internet.pass.c_str() );
    WiFi.mode( WIFI_STA );

    SINT time;
    int start = millis();
    while( WiFi.status() != WL_CONNECTED ){
        int current = millis();
        Serial.print(".");
        if( ( time = current - start ) >= TIMEOUT ){
            break;
        }
        delay( 100 );
    }
    Serial.println("WiFi connected");
}

/* Copyright HAL College of Technology & Design */

