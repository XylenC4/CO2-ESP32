extern int iMenu;
extern String LocalWifiIP;
#define OLED_LOG_LINES 6
String sDataLines[OLED_LOG_LINES];
int iActDataLine = 0;

void setup_OLED() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0);
}

void OledDisplayLogAdd(String sValue, int iDelay) {
  if(iActDataLine<OLED_LOG_LINES) {
    sDataLines[iActDataLine] = sValue;
    iActDataLine++;
  }
  else {
    for(int i=1; i<OLED_LOG_LINES;i++) {
     sDataLines[i-1] = sDataLines[i];  
    }
    sDataLines[OLED_LOG_LINES-1] = sValue;
  }
  if(iDelay>0) {
    OledDisplayLog();
    delay(iDelay);
  }
}

void OledDisplayLogEditCurrent(String sValue, int iDelay) {
  sDataLines[iActDataLine-1] = sValue;
  OledDisplayLog();
  //OledDisplayLogAdd(String("Act:") + iActDataLine + String(";") + sValue,500);
  delay(iDelay);
}

void OledDisplayLog(){
  OledDisplayHeader("                  Log");
  for(int i=0;i<OLED_LOG_LINES;i++) {
    display.println(sDataLines[i]);
  }
    display.display();     
}

void OledDisplayLogClear() {
  OledDisplayHeader("                  Log");
  
  for(int i=0;i<OLED_LOG_LINES;i++) {
    sDataLines[i] = "";
  }
  display.display();      
  iActDataLine = 0;
}

void OledDisplayHeader(String sHeader) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextWrap(0);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(PROJECT_NAME);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextSize(1);
  display.println(sHeader);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void OledDisplayLogWhait(String sText, int iDelayTime) {
  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();
  unsigned long delaytime = 0;
  
  OledDisplayLogAdd(sText,0);
    
  while(delaytime < iDelayTime) {
    currentMillis = millis();
    delaytime =  (currentMillis - previousMillis) / 1000;
    OledDisplayLogEditCurrent(sText + String(": ") + String(iDelayTime-delaytime),250);
  }
}

void showDisplay() {
  switch (iMenu) {
  case 0:
   MenuMain();
    break;
  case 1:
   MenuCO2();
    break;
  case 2:
   OledDisplayLog();
    break;
  case 3:
   MenuDebugBME280();
    break;
  case 4:
   MenuDebugBME280OriginalData();
    break;
  case 5:
   MenuDebugMHZ19B();
    break;
  default:
   MenuMain();
  }
}

void MenuMain() {
  display.setTextWrap(0);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(PROJECT_NAME);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextSize(1);
  display.println("                  cpm");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println(uiCo2Ppm); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println(String("Temperature:  ") + String(fBME280Temperature,1) + String(" c") );
  display.println(String("Humidity:     ") + String(fBME280Humidity,1) + String(" %") );
  display.println(String("Pressure: ") + String(fBME280Pressure,1) + String(" hpa") );
  display.display();
}

void MenuCO2() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(PROJECT_NAME);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextSize(1);
  display.println("                  cpm");
  display.println("");
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.println(uiCo2Ppm); 
  display.display();
}

void MenuDebugBME280() {
  OledDisplayHeader("          BME280 Data");
  display.println(String("Temperature:") + String(fBME280Temperature,1) + String(" c"));
  display.println(String("Humidity   :") + String(fBME280Humidity,1) + String(" %"));
  display.println(String("Pressure   :") + String(fBME280Pressure,1) + String(" hpa"));
  display.println(String("HumidityAbs:") + String(fBME280AbsoluteHumidity*1000,1) + String(" g/m^3"));
  display.display();
}

void MenuDebugBME280OriginalData() {
  OledDisplayHeader(" BME280 Original Data");
  display.println(String("Temperature:") + String(fBME280OriginalTemperature,1) + String(" c"));
  display.println(String("Humidity   :") + String(fBME280OriginalHumidity,1) + String(" %"));
  display.println(String("Pressure   :") + String(fBME280OriginalPressure,1) + String(" hpa"));
  display.display();

}

void MenuDebugMHZ19B() {
  OledDisplayHeader("          MHZ19B Data");
  display.println(String("ppm        :") + String(uiCo2Ppm) + String(" ppm"));
  display.println(String("Temperature:") + String(uiCo2Temp) + String(" c"));
  display.println(String("U          :") + String(uiCo2UhUl));
  display.println(String("State      :") + String(uiCo2state));
  display.display();
}
