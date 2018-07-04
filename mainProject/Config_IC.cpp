#include "ArduinoLibrary.h"
#include "ServerCommunication.h"
#include "function.h"
#include "constants.h"
#include "System.h"

extern MFRC522 MF;

String MFRCTake( void )
{
  digitalWrite(LED_PIN, HIGH);
  String content= "";
  //UIDを読み込む
  for (byte i = 0; i < MF.uid.size; i++) {
    content.concat(String(MF.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(MF.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content;
}

void bip ( void )
{
  tone(BUZ_PIN,1000);
  delay(100);
  noTone(BUZ_PIN);
  digitalWrite(LED_PIN,LOW);
}
