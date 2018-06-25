#include <string.h>
#include "ArduinoLibrary.h"
#include "ServerCommunication.h"
#include "function.h"

//RFID's
constexpr uint8_t RST_PIN = 15;//PIN 15 as RFID's RST
constexpr uint8_t SS_PIN = 2;  //PIN 2 as RFID's SDA
#define LED_PIN 4              //PIN 4 as LED
const int buzzer = 5;          //PIN 5 as Buzzer
MFRC522 MF(SS_PIN, RST_PIN);  //Create MF instance

void MFRCSetup( void );  //RFID Setup
String MFRCTake( void ); //Read RFID ID
void Bip( void );        //Buzzer ON

void setup() 
{
  Serial.begin(9600);
  delay(10);
  pinMode(LED_PIN, OUTPUT);
  pinMode(buzzer, OUTPUT); //Buzzer
  digitalWrite(LED_PIN, LOW);
  
  //Wifi Setup
  WiFiSetup();
  //Reader Setup
  MFRCSetup();
  Serial.println("RFID待ち... ");
}

void loop()
{
  String readID;
  if ( ! MF.PICC_IsNewCardPresent()) { return; } //Wait for new IC
  if ( ! MF.PICC_ReadCardSerial()) { return; }   //Found, then Read IC
  
  readID = MFRCTake(); //Take IC's ID
  Serial.println("ReaderID : "+readID);
  //サーバにPOSTする
  upload(readID);
  //ブザーを鳴らす
  Bip();
  delay(1000);
}

void MFRCSetup ( void )
{
  SPI.begin();   // Init SPI bus
  MF.PCD_Init(); // Init MF 
}

String MFRCTake( void )
{
  String content= "";
  //UIDを読み込む
  for (byte i = 0; i < MF.uid.size; i++) {
    content.concat(String(MF.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(MF.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content;
}

void Bip ( void )
{
  tone(buzzer,1000);
  delay(100);
  noTone(buzzer);
}

