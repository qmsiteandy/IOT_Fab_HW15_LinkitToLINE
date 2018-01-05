#include <ArduinoJson.h>
#include <LWiFi.h>

const char* ssid     = "406-1";           // insert your SSID
const char* pass     = "ntuedorm";   // insert your password

WiFiClient client;

const char* server1 = "opendata.epa.gov.tw";  
const char* resource1 = "/ws/Data/ATM00625/?$format=json"; 
const char* server2 = "maker.ifttt.com";
const char* resource2 = "/trigger/LINE_message/with/key/bB-ei9ZVYoWOVOo8-16w9d";
char json[12000]; 

void setup(){
  Serial.begin(115200);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
}

void loop(){
  // connect to server  
  while(!client.connect(server1, 80)){
    // wait for connecting server...
Serial.println("server1 connecting");
  }
Serial.println("server1 connected");

  //Send request to resource
  client.print("GET ");
  client.print(resource1);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server1);
  client.println("Connection: close");
  client.println();
  delay(100);
  
Serial.println("getting datas ...");
  //Reading stream
  client.setTimeout(10000);

  //Checking for the end of HTTP stream
  while(!client.find("\r\n\r\n")){
    // wait for finishing header stream reading ...
Serial.println("finding data's end");    
  }

  // Reading the response string
  client.readBytes(json, 12000);
  
  // process JSON from response string
  DynamicJsonBuffer jsonBuffer;
  JsonArray& rootArray = jsonBuffer.parseArray(json);
  JsonObject& xinYingSite = rootArray[33];
  
  const char* site0 = xinYingSite["Site"];
  double pm250 =xinYingSite["PM25"];

  
  
  while(!client.connect(server2, 80)){
    // wait for connecting server...
Serial.println("server2 connecting");
  }
Serial.println("server2 connected");

  client.print("GET ");
  client.print(resource2);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server2);
  client.println("Connection: close");
  client.println();
  delay(100);
 
  /*if(pm250==0.0){
    client.print("https://");
    client.print(server2);
    client.print(resource2);
    //client.print("?value1=");
    client.print("正在取得資料...");
 Serial.println("message send finding");   
  }else{
  //Send request to resource
    client.print("https://");
    client.print(server2);
    client.print(resource2);
    client.print("?value1=");
    client.print("資料取得:");
    client.print("&value2=");
    client.print(site0);
    client.print("&value3=");
    client.print(pm250);
 Serial.println("message send data");
  }
Serial.println("message is sent");*/

  delay(1000);

 
  // Print data to Serial
  Serial.print("*** ");
  Serial.print(site0);
  Serial.println(" ***");
  Serial.print("PM25: ");
  Serial.println(pm250);
  Serial.println("--------------------"); 



  client.stop(); // disconnect from server
  
  delay(10000); // the OWM free plan API does NOT allow more then 60 calls per minute
}


