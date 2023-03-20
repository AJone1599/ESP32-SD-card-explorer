#include <WiFi.h>
#include <WebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "htmlFile.h"


// SSID & Password
const char* ssid = ""; 
const char* password = "";

WebServer server(80);

//Create the directory to send to website
String printDirectory(File dir, int numTabs) {
  String list = "[{\"File Name\": \"\", \"Path\": \"\", \"File Type\" : \"\"}";
  int i = 0;
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      list = list + "]";
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      //Serial.print('\t');
    }
    String fileName = entry.name();
    if (entry.isDirectory()) {
      String file = "{\"File Name\": \"" + fileName + "\", \"Path\": \"" + dir.name() + "\", \"File Type\" : \"" + "DIR" + "\"}";
      list = list + "," + file;
    } else {
      String fileExtension = String(fileName.substring(fileName.lastIndexOf(".")));
      String file = "{\"File Name\": \"" + fileName + "\", \"Path\": \"" + dir.name() + "\", \"File Type\" : \"" + fileExtension + "\"}";
      list = list + "," + file;
    }
    entry.close();
  }
  return list;
}



/*
SCK         4
//spid
MOSI        6
//spiq
MISO        5
//SS
SD_CS       7
*/
void setup() {
  Serial.begin(115200);
  while(!Serial){
    ;
  }
  //Set up SD Card
  Serial.println(SCK);
  Serial.println(MOSI);
  Serial.println(MISO);
  Serial.println(SS);
  SPI.begin(SCK, MISO, MOSI, SS);

  SPIClass spi = SPIClass(SPI);
  if (!SD.begin(SS,spi,4000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
  String directoryList = printDirectory(SD.open("/"),0);
  Serial.println(directoryList);
  Serial.println("-----");
  //Connect to WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      ;
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  //Send Webpage
  server.on("/", sendWebpage);
  //Send the directory list when user requests it
  server.on("/list",HTTP_GET,[&, directoryList](){Test(directoryList);});
  server.begin();

}

void Test(String list){
  Serial.println("GET Request Received");
  Serial.println("Attempting to send result");
  Serial.println(list);
  server.sendContent(list);
  Serial.println("done");
}

void sendWebpage(){
  server.send(200, "text/html", index_html);
  Serial.println("Someone Connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
