#include <OLED.h>
#include <OneWire.h>
#include <math.h>
 
OneWire  ds(4);  // on pin 5 (a 4.7K resistor is necessary)

int timer;

void setup(){
  disp.begin();
}

void loop(){
  byte i;
  byte data[12];
  byte addr[8];
  int celsiusC;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];

  celsiusC = round(raw / 16);
  
  disp.setCursor(0, 0);
  
  disp.print(",/ ");
  if(abs(celsiusC)<10){
    disp.print(" ");
  }
  if(celsiusC>=0){
    disp.print("+");
  }
  disp.print(celsiusC);
  disp.print(": ");
}
