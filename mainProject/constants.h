/*
    constants.h

    デバイスピンアサイン

    created 2018/06/28
    by Nakajim Yam
*/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/* OTHER */
#define LED_PIN ( 4    )  /* PIN 4 as LED                 */
#define BUZ_PIN ( 5    )  /* PIN 5 as Buzzer              */
#define APSWT   ( 0    )  /* PIN 0 as WiFiSetting(Server) */
#define TIMEOUT ( 30000)  /* WiFi Timeout                 */
constexpr uint8_t RST_PIN = 0;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 2;         // Configurable, see typical pin layout above

#endif /* __CONSTANTS_H__ */
