#ifndef __FUNCTION_H__
#define __FUNCTION_H__

/* --- includeファイル --- */
#include <string.h>
#include "ArduinoLibrary.h"
#include "System.h"                 /* システム共通データ定義ヘッダ */
#include "ServerCommunication.h"    /* サーバー通信クラス */
#include "InfoStruct.h"

/* --- Prototype --- */
SINT upload         ( String );
SINT registerDevice ( void );
void setupWifi      ( void );
void getWiFiConfig  ( void );
void getDeviceConfig( void );
void connectRouter  ( void );
String MFRCTake     ( void );
void Bip            ( void );

/* Root */
void handleRootMain ( void );
void handleGetWifi  ( void );
void handlePostWifi ( void );
void handleGetPin   ( void );
void handlePostPin  ( void );

#endif /* __FUNCTION_H__ */
