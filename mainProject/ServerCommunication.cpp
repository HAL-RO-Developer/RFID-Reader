#include "ServerCommunication.h"

ServerCommunication::ServerCommunication(String host):
  host(host)
{}

SSHT ServerCommunication::connect(String host, String port )
{
  this->host = host;
  this->port = port;
  if( !client.connect(host.c_str(), port.toInt()) ){
    Serial.println("connection failed");
    return SERVER_CONNECT_ERROR;
  }
  return SERVER_SUCCESS;
}

void ServerCommunication::post(String url, String data, String host)
{
  const char* fingerprint = "45 71 78 9E 29 A8 36 8F D8 F8 43 3E 28 C4 56 E8 11 63 69 D8";
  if (client.verify(fingerprint, host.c_str())) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
  
  if( !client.connected() ){
    connect( host, port );
  }

  Serial.println("func: POST-ing");
  this->url = url;
  String body = data;
  /* HTTP REQUEST */
  client.println("POST "+ url +" HTTP/1.1");
  /* HTTP HEADER */
  client.println("Host:" + host);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(body.length());
  client.println();
  /* HTTP BODY */
  client.print(body);
 
  delay(10);
}

void ServerCommunication::get(String url, String data)
{
    // GET用  
}

SINT ServerCommunication::response(String* data)
{
  String response = client.readString();
  int statusPos = response.indexOf("HTTP/1.1 ") + 9;
  String statusString = response.substring(statusPos, statusPos + 3);
  status = statusString.toInt();

  /* body位置検索 */
  int bodyPos =  response.indexOf("\r\n\r\n") + 4;
  *data = response.substring(bodyPos);

  Serial.println("Taken response");
  if(client.connected()){
    client.stop();
  }
  
  return status;
}

void ServerCommunication::setUrl(String url)
{
  this->url = url;
}

SINT ServerCommunication::getStatus()
{
  return status;
}
