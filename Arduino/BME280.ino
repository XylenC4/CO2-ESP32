/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

Adafruit_BME280 bme; // I2C

float fBME280Temperature, fBME280Pressure, fBME280Humidity, fBME280AbsoluteHumidity, fBME280CorrectedRelativeHumidity;
float fBME280OriginalTemperature, fBME280OriginalHumidity, fBME280OriginalPressure;

void setup_BME280() {
    bool status;
    
    OledDisplayLogAdd("BME280 connecting", 0);
    status = bme.begin();  
    if (!status) {
        OledDisplayLogAdd("BME280 not found", 0);
        OledDisplayLogAdd("check wiring!", 5000);
        ESP.restart();
    }
    OledDisplayLogAdd("BME280 connected", 0);
    OledDisplayLogAdd("BME280 setup", 100);
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1,   // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1,   // humidity
                    Adafruit_BME280::FILTER_OFF );                   
}

void loop_BME280() { 
    bme.takeForcedMeasurement(); // has no effect in normal mode
    //Uncompensated values from sensor
    fBME280OriginalTemperature = bme.readTemperature();
    fBME280OriginalPressure = bme.readPressure() / 100.0F;
    fBME280OriginalHumidity = bme.readHumidity();
    fBME280AbsoluteHumidity = getAbsoluteHumidity(fBME280OriginalHumidity, fBME280OriginalTemperature);
 
    //Compensation if neccessarily
    if(COMPENSATION_TEMP != 0) {
      fBME280Temperature = fBME280OriginalTemperature + COMPENSATION_TEMP;
      fBME280Pressure = fBME280OriginalPressure;
      fBME280Humidity = getRelativeHumidity(fBME280AbsoluteHumidity, fBME280Temperature, fBME280Pressure);
    }
    else {
      fBME280Temperature = fBME280OriginalTemperature;
      fBME280Pressure = fBME280OriginalPressure;
      fBME280Humidity = fBME280OriginalHumidity;
    }
}

//https://forum.arduino.cc/index.php?topic=420975.0
float getAbsoluteHumidity(float Humidity, float Temperature ) {
  return (6.112 * pow(2.71828,((17.67 * Temperature)/(Temperature + 243.5))) * Humidity * 2.1674) / (273.15 + Temperature)/1000; // in g/m^3
}

//https://planetcalc.com/2167/
float getRelativeHumidity(float AbsoluteHumidity, float Temperature, float Pressure) {
  float A = AbsoluteHumidity;
  float tp = Pressure;
  float t = Temperature;
  float et = 6.112*exp((17.62*t)/(243.12+t));
  float fp = 1.0016 + 3.15 * pow(10,-6) * tp - 0.074/tp;
  float trh = (A*461.5*(t + 273.15))/(fp*et);
  return trh;
}
