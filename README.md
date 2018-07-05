## サーバ
- github.com/HAL-RO-Developer/caseTeamB_web

## HTTPS化
- ngrok を使用
- ngrokウェブサイトでZIPをダウンロードし、展開する。
- サーバを起動後、
    - ngrok http 8000 でHTTPS作成される
	- "...".ngrok.io

## コード上
- config.h
    - host = "... .ngrok.io";

## ユーザ
- APモードの切り替えは、マイコンが起動して３秒以内にFLASHボタンを押してください。
- ssid : caseSTUDY2018 , pass : password
- URL は　192.168.4.1
    - WiFi Settings : WiFi設定
    - Device Registration : アプリに表示されるＰＩＮを入力し、マイコン情報を登録するため。
