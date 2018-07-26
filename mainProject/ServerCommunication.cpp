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
  if( !client.connected() ){
    connectRouter();
    connect( host, port );
  }

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
 
  Serial.println("Posted");
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

  /* body位置検索 */
  int bodyPos = response.indexOf("\r\n\r\n") + 4;
  String res  = response.substring(bodyPos);
  
  StaticJsonBuffer<1024> JSONBuffer;
  JsonObject& root = JSONBuffer.parseObject(res);

  delay(100);
  const char * device_id = root["device_id"];
  *data = String(device_id);
  
  Serial.println(" Responsed");
  if(client.connected()){
    client.stop();
  }
}
void ServerCommunication::setUrl(String url)
{
  this->url = url;
}

SINT ServerCommunication::getStatus()
{
  return status;
}
