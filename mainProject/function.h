#ifndef __FUNCTION_H__
#define __FUNCTION_H__

/* --- includeファイル --- */
#include <string.h>
#include "ArduinoLibrary.h"
#include "System.h"                 /* システム共通データ定義ヘッダ */
#include "ServerCommunication.h"    /* サーバー通信クラス */

/* --- Prototype --- */
void WiFiSetup( void );
SINT upload( String );

#endif /* __FUNCTION_H__ */
