
#include "CBL2.h"
#include "TIVar.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
CBL2 cbl;


String apiKey = "AIzaSyBkbm0izHAZVO3UIkFp0vtzJfZa6jDe2GQ";  


 String message = "lol";
 int sigma = 0;
int jikan = 0;
int numChunks = 0;
bool connected = false;
bool ai = false;
const int tip = 0;
const int ring = 1;
size_t chunk_size = 225;
String Aimsg[10]; 
String wifiNamae;
String wifiPasswado;
#define MAXDATALEN 255
uint8_t header[16];
uint8_t data[MAXDATALEN];
int conectoshitai = 0;
String sugoiString(String input); 
void connectWifi(String ssid,String password);
String sendToGemini(String inputMessage);
int onReceived(uint8_t type, enum Endpoint model, int datalen);
int onRequest(uint8_t type, enum Endpoint model, int* headerlen,
              int* datalen, data_callback* data_callback);

void setup() {
    Serial.begin(115200);
    cbl.setLines(tip, ring);
    cbl.resetLines();
    
    
    cbl.setupCallbacks(header, data, MAXDATALEN, onReceived, onRequest);

 

  



}

void loop() {
    cbl.eventLoopTick();
}

int onReceived(uint8_t type, enum Endpoint model, int datalen) {
    if (type != VarTypes82::VarString) {
        Serial.println("Received invalid data type");
        return -1;
    }

    String str = TIVar::strVarToString8x(data, model);
    Serial.print("Received: ");
	Serial.println(str);
   if (str == "WIFI") {
      conectoshitai = 1;
      Serial.print("Requested connection to wifi");
   } else {
      
      if (conectoshitai == 2){
       wifiPasswado = sugoiString(str);
       Serial.println(sugoiString(str));
       conectoshitai = 0;
       connectWifi(wifiNamae,wifiPasswado);
      }
      if (conectoshitai == 1){
       wifiNamae = sugoiString(str);
       Serial.println(sugoiString(str));
       conectoshitai = 2;
      }
   }

if (ai){
    
   message = sendToGemini(str);
    Serial.print(message);
   numChunks = (message.length() + chunk_size - 1) / chunk_size;
   
     for (int i = 0; i < numChunks && i < 10; i++) {
    int start = i * chunk_size;
    int end = (i + 1) * chunk_size;
    if (end > message.length()) end = message.length(); 
    Aimsg[i] = message.substring(start, end); 
    Serial.print("Div ");
    Serial.print(i);
    Serial.println(":");
    Serial.println(Aimsg[i]);
  }

  Serial.println(numChunks);
}
  
if (str == "AI"){
   if (connected) {
      Serial.println("Sigma");
  ai = true;
   } else {
      Serial.println("No WIFI");
      message = "No WIFI";
   }
}
   

    return 0;
}

int onRequest(uint8_t type, enum Endpoint model, int* headerlen,
              int* datalen, data_callback* data_callback)
{

   if(ai){
    
       
       message = Aimsg[sigma];
       if (sigma == numChunks){
        
        sigma = 0;
         ai = false; 
         
     message = "End";  
         
        
       } else {
sigma++;
       }
       
       
       
   }
   Serial.print("sending:");
   Serial.println(message);
    if (type != VarTypes82::VarString) {
        Serial.println("Received request for invalid data type");
        return -1;
    }
    

   
    int rval = TIVar::stringToStrVar8x(message, data, model);
   
    if (rval < 0) {
        return -1;
    }
    *datalen = rval;

    memset(header, 0, sizeof(header));
    TIVar::intToSizeWord(rval, header);
    header[2] = VarTypes82::VarString; 
    header[3] = 0xAA; 
    header[4] = 0x00; 
    *headerlen = 13;

   
 
    return 0;
}

void connectWifi(String ssid,String password){
   
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while ((WiFi.status() != WL_CONNECTED) && ( jikan != 15) ) {
      Serial.print('.');
      delay(1000);
      jikan++;
  } 
  jikan = 0;
  if (WiFi.status() == WL_CONNECTED){
  Serial.println(WiFi.localIP());
  Serial.print("\nConnected :)");
  message = "Connected ip:" + WiFi.localIP().toString();
  connected = true;
  }else {
   Serial.print("\nTimed Out");
   message = "Timed Out";
  }
  
}

String sugoiString(String input) {
    String output = "";

    for (int i = 0; i < input.length(); i++) {
        char letter = input[i];

      
        if ((letter == '1' || letter == '0') && i > 0 && isalpha(input[i - 1])) {
            
            if (letter == '1') {
                output[output.length() - 1] = toupper(output[output.length() - 1]);
            } else {
                output[output.length() - 1] = tolower(output[output.length() - 1]);
            }
        } 
        else {
           
            output += letter;
        }
    }

    return output;
}











String sendToGemini(String inputMessage) { 
  
  String endpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + apiKey;

  HTTPClient http;
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");

  
  DynamicJsonDocument doc(1024);
  JsonArray contents = doc.createNestedArray("contents");
  JsonObject content = contents.createNestedObject();
  JsonArray parts = content.createNestedArray("parts");
  JsonObject part = parts.createNestedObject();
  part["text"] = inputMessage;

  String requestBody;
  serializeJson(doc, requestBody);

  
  int httpCode = http.POST(requestBody);

  String response = "";
  if (httpCode == HTTP_CODE_OK) {
    
    DynamicJsonDocument responseDoc(2048);
    deserializeJson(responseDoc, http.getString());

    
    String generatedText = responseDoc["candidates"][0]["content"]["parts"][0]["text"].as<String>();

    response = generatedText;
  } else {
    response = "Error: " + String(httpCode);
  }

  http.end();
  return response;
}
