#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

extern String LocalWifiIP;
unsigned long lThingspeakLoopPreviousTime = 0;

void setup_Thingspeak(){ 
  Connect_to_Wifi(); 
}

void loop_Thingspeak(){
  static unsigned int SensorLoop = 0;
  unsigned long lLoopActual = millis();
  unsigned long lLoopDelayTime = (lLoopActual - lThingspeakLoopPreviousTime) / 1000;

  if(lLoopDelayTime > DELAY_TIME_BETWEEN_THINGSPEAK_UPDATE) {
    Send_Data();
    lThingspeakLoopPreviousTime = millis();
  }
}

void Connect_to_Wifi()
{
    delay(10);
    
    if(!Check_Wifi_Available()) {return;} 

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, password);
 
    OledDisplayLogAdd("Connecting to Wifi", 0);
    
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(500);
    }

    OledDisplayLogAdd("Connected to Wifi:", 0);
    OledDisplayLogAdd(String("SSID: ") + ssid, 0);
    OledDisplayLogAdd(String("IP: ") + ipToString(WiFi.localIP()), 3000);  
}

bool Check_Wifi_Available() {

    OledDisplayLogAdd("Scanning Wifi...", 0);
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    OledDisplayLogAdd("Scanning done", 500);
    if (n == 0) {
            OledDisplayLogAdd("No networks found", 500);
            delay(500);
    } else {

        for (int i = 0; i < n; ++i) {
            OledDisplayLogAdd(WiFi.RSSI(i) + String(";") + WiFi.SSID(i), 100);
        }
        
        for (int i = 0; i < n; ++i) {
            if(WiFi.SSID(i) == ssid) { 
              if(WiFi.RSSI(i) > -85) {
                return true;
              }
              else {
               OledDisplayLogAdd("Network signal weak", 0);
               OledDisplayLogAdd("Must be > -85", 0);
              }
            }
        }
    }
    
    OledDisplayLogAdd("Exit without connecting", 5000);
    delay(5000);
    return false;
}

void Send_Data()
{
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    OledDisplayLogAdd("Thingiverse conn failed", 500);
    delay(5000);
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(uiCo2Ppm);
    data_to_send += "&field2=";
    data_to_send += String(fBME280Temperature);
    data_to_send += "&field3=";
    data_to_send += String(fBME280Humidity);
    data_to_send += "&field4=";
    data_to_send += String(fBME280Pressure);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);
  }
  client.stop();
}

// https://www.esp8266.com/viewtopic.php?p=25466
String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
