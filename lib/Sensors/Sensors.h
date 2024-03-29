/*
[Part of ADVANCED COOLING CONTROL ALGORITHM]

Temperature control library
---------------------------

Sensor control library for all types of temperature sensors. Contain several 
functions for converting analog signals and converting it into temperature 
values. Recieves analog voltages from different sensors [NTC10k, DS18B20], 
This library also supports with one-wire protocol sensors. Respective functions
returns temperature(in C) based on respective analog signals recieved. 

[vishnus_technologies (C) 2022]
--------------------------------------------------------------------------------
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS18B20_AddrDir.h>

#define ONEWIRE_PORT 2  // for DS18B20 devices
#define RT0 10000
#define B 3977 
#define VCC 5
#define R 10000

// sensor address mapping:
// (X_MOT_SA -> x motor sensor address)
#define X_MOT_SA x_addr
#define Y_MOT_SA y_addr
#define Z_MOT_SA z_addr
#define E_MOT_SA e_addr

OneWire oneWire(ONEWIRE_PORT);	
DallasTemperature sensors(&oneWire);

bool INIT_FLAG = 0;
double T0 = 25 + 273.15; 


// NTC10k sensor class:
class ntc10k {
  public : bool init_sensor() {
    return EXIT_SUCCESS;
  }

  public : float GetTemperature(float Tdata) {
    float Ctemp, RT, VR, ln;
    Tdata = (5.00/1023.00)*Tdata;
    VR = VCC-Tdata;
    RT = Tdata/(VR/R);
    ln = log(RT/RT0);
    Ctemp = (1/((ln/B)+(1/T0)));
    Ctemp = Ctemp-273.15;  
    return Ctemp;
  }
};


// DS18B20 1-wire sensor class:
class ds18b20 {
  public : bool init_sensor() {
    sensors.begin();
    INIT_FLAG = 1;
    return EXIT_SUCCESS;
  }

  public : void GetTempByAddr() {
    if(INIT_FLAG) {
      sensors.requestTemperatures();
      Serial.print(F("1: "));
      // getTemp({SENSOR ADDRESS})
      getTemp(X_MOT_SA);
      Serial.print(F("2: "));
      getTemp(Y_MOT_SA);
      Serial.print(F("3: "));
      getTemp(Z_MOT_SA);
      Serial.print(F("4: "));
      getTemp(E_MOT_SA);

      Serial.println();
      delay(1000);
    }
    else {
      init_sensor();
      Serial.println(F("auto initialised DS18B20"));
    }
  }

  private : void getTemp(DeviceAddress deviceAddress) {
    float tempC = sensors.getTempC(deviceAddress);
    Serial.print(tempC);
    Serial.print(F("C"));
}
};
