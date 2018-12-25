#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <HardwareSerial.h>

/*********************************************************************
  General
*********************************************************************/
extern void OledDisplayLogAdd(String sValue, int iDelay);
extern unsigned int ppm;

#define PROJECT_NAME "CO2 Sensor"
#define DELAY_TIME_BETWEEN_SENSOR_READING 250
#define DELAY_TIME 100
#define STARTUP_TIME 3

/*********************************************************************
  Wifi / Thingspeak
*********************************************************************/
const char* ssid     = ""; // Your SSID (Name of your WiFi)
const char* password = ""; //Your Wifi password

const char* host = "api.thingspeak.com";
String api_key = ""; // Your API Key provied by thingspeak

#define DELAY_TIME_BETWEEN_THINGSPEAK_UPDATE 300 // in s

/*********************************************************************
  Monochrome OLEDs based on SSD1306 drivers
*********************************************************************/
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/*********************************************************************
  Touch-Sensors
*********************************************************************/
#define TOUCH_PLUS 21
#define TOUCH_MINUS 22
#define TOUCH_SET 23

/*********************************************************************
  BME280 Sensor Data
*********************************************************************/
#define COMPENSATION_TEMP -3
extern float fBME280Temperature, fBME280Pressure, fBME280Humidity, fBME280AbsoluteHumidity, fBME280CorrectedRelativeHumidity;
extern float fBME280OriginalTemperature, fBME280OriginalHumidity, fBME280OriginalPressure;

/*********************************************************************
  MHZ19B CO2 Sensor Data
*********************************************************************/
#define WARMUP_TIME 5
extern unsigned int uiCo2Ppm,uiCo2Temp,uiCo2UhUl,uiCo2state;

/*********************************************************************
  Main loop
*********************************************************************/
int iAction = -1;
unsigned long lMainLoopPreviousTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(18,19);
  setup_OLED();
  OledDisplayLogClear();
  OledDisplayLogWhait("Startup delay", STARTUP_TIME);
  setup_MHZ19B();
  OledDisplayLogWhait("MHZ19B warmup", WARMUP_TIME);
  setup_BME280();

  loop_MHZ19B();
  loop_BME280();
  loop_Touch();
  setup_Thingspeak();
  showDisplay();

  // Set Time
  lMainLoopPreviousTime = millis();
}


void loop() {
  static unsigned int SensorLoop = 0;
  unsigned long lMainLoopActual = millis();
  unsigned long lMainLoopDelayTime = (lMainLoopActual - lMainLoopPreviousTime);

  if(lMainLoopDelayTime > DELAY_TIME_BETWEEN_SENSOR_READING) {
    SensorLoop += 1;
    lMainLoopPreviousTime = millis();

    if(SensorLoop > 2) {
      SensorLoop = 0;
    }
    
    iAction = SensorLoop;
  }

  switch (iAction) {
  case 0:
      loop_MHZ19B();
    break;
  case 1:
      loop_BME280();
    break;
  case 2:
      showDisplay();
      loop_Thingspeak();
    break;
 }
 iAction = -1;

 loop_Touch();
 delay(DELAY_TIME);
  
}
