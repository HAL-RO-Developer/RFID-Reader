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
RFIDCONFIG rfid;

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
  rfid.new_uuid="";
  waitBlink();
  /*if ( ! MF.PICC_IsNewCardPresent()) { return; } //Wait for new IC
  if ( ! MF.PICC_ReadCardSerial()) { return; }   //Found, then Read IC
  */
  //RFIDのIDを読込む
  //rfid.new_uuid = MFRCTake();
  Serial.println("new uuid : " + rfid.new_uuid);
  Serial.println("old uuid : " + rfid.old_uuid);
  //サーバにPOSTする
  upload();
  //新しいUUIDを保存する
  rfid.old_uuid = rfid.new_uuid;
  //ブザーを鳴らす
  bip();
  delay(1000);
}

void deviceInit()
{
  /* シリアル通信 */
  Serial.begin(115200);
  Serial.println();
  /* ピン設定 */
  pinMode(APSWT, INPUT );    /* push Button */
  pinMode(LED_PIN, OUTPUT);  /* LED         */
  pinMode(BUZ_PIN, OUTPUT);  /* Buzzer      */
  digitalWrite(LED_PIN, LOW);/* LED OFF     */
  /* SPI通信 */
  SPI.begin();
  /* RFID初期化 */
  MF.PCD_Init();
  /* ファイルシステム */
  SPIFFS.begin();
  /* マイコンのモード設定 */
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

