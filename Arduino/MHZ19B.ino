extern void OledDisplayLogAdd(String sValue, int iDelay);
HardwareSerial mySerial(1); // A0 - к TX сенсора, A1 - к RX
unsigned long lMHZ19BLoopPreviousTime = 0;

byte cmdReadCo2[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
byte cmdPPM1000[9] = {0xFF,0x01,0x99,0x00,0x00,0x00,0x07,0xE8,0x7B}; 
byte cmdPPM2000[9] = {0xFF,0x01,0x99,0x00,0x00,0x00,0x07,0xD0,0x8F}; 
byte cmdPPM3000[9] = {0xFF,0x01,0x99,0x00,0x00,0x00,0x07,0xB8,0xA3}; 
byte cmdPPM5000[9] = {0xFF,0x01,0x99,0x00,0x00,0x00,0x13,0x88,0xCB}; 
byte cmdDisableABC[9] = {0xFF,0x01,0x79,0x00,0x00,0x00,0x00,0x00,0x86};
byte cmdEnableABC[9] = {0xFF,0x01,0x79,0xA0,0x00,0x00,0x00,0x00,0x86};
unsigned char response[9];
unsigned int uiCo2Ppm = 0;
unsigned int uiCo2Temp = 0;
unsigned int uiCo2UhUl = 0;
unsigned int uiCo2state = 0;

void setup_MHZ19B() {
  
  OledDisplayLogAdd("MHZ19B start serial", 100);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);

  OledDisplayLogAdd("MHZ19B set PPM2000", 100);
  mySerial.write(cmdPPM2000, 9);
  memset(response, 0, 9);
  mySerial.readBytes(response, 9);

  OledDisplayLogAdd("MHZ19B enable ABC", 100);
  mySerial.write(cmdEnableABC, 9);
  memset(response, 0, 9);
  mySerial.readBytes(response, 9);
  
  // Set Time
  lMHZ19BLoopPreviousTime = millis();
}

void loop_MHZ19B() 
{
  unsigned long lLoopActual = millis();
  unsigned long lLoopDelayTime = (lLoopActual - lMHZ19BLoopPreviousTime) / 1000;

  if(lLoopDelayTime > DELAY_TIME_BETWEEN_MHZ19B_UPDATE) {
    OledDisplayLogAdd("MHZ19B Update", 0);
    lMHZ19BLoopPreviousTime = millis();
    mySerial.write(cmdReadCo2, 9);
    memset(response, 0, 9);
    mySerial.readBytes(response, 9);
    int i;
    byte crc = 0;
    for (i = 1; i < 8; i++) crc+=response[i];
    crc = 255 - crc;
    crc++;
  
    if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc)) {
      OledDisplayLogAdd("MHZ19B CRC error",0);  
      OledDisplayLogWhait("Restarting", 10);
      ESP.restart();
    } else {
      uiCo2Ppm = convHiLo(response[2], response[3]);
      uiCo2Temp = (int) response[4] -40;
      uiCo2state = (unsigned int) response[5]; 
      uiCo2UhUl = convHiLo(response[6], response[7]);
    }
  }

}

unsigned int  convHiLo(char High, char Low) {
    unsigned int responseHigh = (unsigned int) High;
    unsigned int responseLow = (unsigned int) Low;
    return (256*responseHigh) + responseLow;
}
