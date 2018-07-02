/*
    Main.ino
    システムメイン

    Created 2018/06/28
    By Nakajim Yam
*/
/* Include Files */
#include "ArduinoLibrary.h"
#include "ServerCommunication.h"
#include "function.h"
#include "constants.h"
#include "System.h"

MFRC522 MF(SS_PIN, RST_PIN);  //Create MF instance
WiFiClient client;             //Wifi Instance
WIFICONFIG internet;

int onetime = 0;

/* --- プロトタイプ宣言 --- */
void deviceInit( void );

void setup() 
{
  deviceInit();
  
  if(internet.device_id==NULL){
    registerDevice();
  }
}

void loop()
{
  String readID;
  
  if ( ! MF.PICC_IsNewCardPresent()) { return; } //Wait for new IC
  if ( ! MF.PICC_ReadCardSerial()) { return; }   //Found, then Read IC

  //Take IC's ID
  readID = MFRCTake();
  Serial.println(readID);
  //サーバにPOSTする
  upload(readID);
  //ブザーを鳴らす
  Bip();
  delay(1000);
  }
}

void deviceInit()
{
  /* シリアル通信 */
  Serial.begin(115200);
  Serial.println();
  /* スイッチ */
  pinMode(APSWT, INPUT );
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZ_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  /* SPI通信 */
  SPI.begin();
  /* RFID初期化 */
  MF.PCD_Init();
  /* ファイルシステム */
  SPIFFS.begin();

  Serial.println("wait 3second for AP button");
  Serial.println("Pushed = AP, Release = none");
  delay(3000); //ボタン待ち

  if(digitalRead(APSWT)==LOW){
    Serial.println("WiFi Settings");
    setupWifi();
  } 
  else if(digitalRead(APSWT)==HIGH) {
    connectRouter();
  }
}

