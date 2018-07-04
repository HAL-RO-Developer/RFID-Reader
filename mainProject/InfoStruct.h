/*
    InfoStruct.h

    構造体の定義

    Created 2017/09/22
    By Shogo Tanaka
*/

#ifndef __INFO_STRUCT_H__
#define __INFO_STRUCT_H__

#include "ArduinoLibrary.h"
#include "System.h"

/* --- 情報管理構造体 --- */
typedef struct{
  String ssid;        /* 接続ルーターSSID        */
  String pass;        /* 接続ルーターパスワード  */
  String pin;         /* デバイスからもらったPIN */
  String device_id;   /* デバイスID              */
} WIFICONFIG;

#endif  /* __INFO_STRUCT_H__ */

/* Copyright HAL College of Technology & Design */
