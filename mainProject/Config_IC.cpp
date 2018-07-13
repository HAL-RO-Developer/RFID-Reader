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
void waitBlink()
{
  digitalWrite(LED_PIN,HIGH);
  delay(50);
  digitalWrite(LED_PIN,LOW);
  delay(750);
}
void apBlink()
{
  for(int i = 0; i < 4; i++){
    digitalWrite(LED_PIN, HIGH);/* LED ON      */
    delay(100);
    digitalWrite(LED_PIN, LOW); /* LED OFF     */
    delay(100);
  }
}

void runBlink()
{
  for(int i = 0; i < 4; i++){
    digitalWrite(LED_PIN, HIGH);/* LED ON      */
    delay(500);
    digitalWrite(LED_PIN, LOW); /* LED OFF     */
    delay(100);
  }
}
void regBlink()
{
  for(int i = 0; i < 2; i++){
    digitalWrite(LED_PIN, HIGH);/* LED ON      */
    delay(100);
    digitalWrite(LED_PIN, LOW); /* LED OFF     */
    delay(100);
  }
}
