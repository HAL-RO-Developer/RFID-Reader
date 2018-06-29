#include "ArduinoLibrary.h"
#include "ServerCommunication.h"
#include "function.h"
#include "constants.h"
#include "System.h"

MFRC522 MF_IC(SS_PIN, RST_PIN);  //Create MF instance

String MFRCTake( void )
{
  String content= "";
  //UIDを読み込む
  for (byte i = 0; i < MF_IC.uid.size; i++) {
    content.concat(String(MF_IC.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(MF_IC.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content;
}

void Bip ( void )
{
  tone(BUZ_PIN,1000);
  delay(100);
  noTone(BUZ_PIN);
}
