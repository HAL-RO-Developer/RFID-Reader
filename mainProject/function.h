/*
    function.h
    共通関数ヘッダー定義

    created 2018/06/28
    by Nakajim Yam
*/
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

/* --- includeファイル --- */
#include <string.h>
#include "ArduinoLibrary.h"
#include "System.h"                 /* システム共通データ定義ヘッダ */
#include "ServerCommunication.h"    /* サーバー通信クラス */
#include "InfoStruct.h"

/* --- Prototype --- */
void upload         ( void );
void registerDevice ( void );
void setupWifi      ( void );
void getWiFiConfig  ( void );
void getDeviceConfig( void );
void connectRouter  ( void );
String MFRCTake     ( void );
void bip            ( void );
void bibip            ( void );

/* LED Blink */
void waitBlink      ( void );

/* Root */
void handleRootMain ( void );
void handleGetWifi  ( void );
void handlePostWifi ( void );
void handleGetPin   ( void );
void handlePostPin  ( void );
void handleGetHost  ( void );
void handlePostHost ( void );

#endif /* __FUNCTION_H__ */
