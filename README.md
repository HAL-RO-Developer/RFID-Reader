## サーバ
- github.com/HAL-RO-Developer/test_server
- github.com/HAL-RO-Developer/caseTeamB_web

## HTTPS化
- ngrok を使用
- ウェブサイトでZIPをダウンロードし、展開する。
- サーバを起動後、
    - ngrok https 443 でHTTPS作成される
	- "...".ngrok.io

## HTTPS の　フィンガープリント(Fingerprint)を取る
- Mozilla 
    - URLの左にある緑鍵の詳細を表示,
        セキュリティーの証明書を表示の中,
        SHA1 フィンガープリント　をコピペ,
        コロン を スペース　に置き換え、コードに移す.
- Chrome　
    - URLの右にある　その他のツール　で　デベロッパツール,
	　　証明書を見て, SHA1 フィンガープリントを探す.

## コード上
- function.cpp 
    - WiFiSetup = ssid ・ pass
	- upload	   = host ・ reqData ・ url 

- ServerCommunication.cpp
    - post	   = fingerprint



