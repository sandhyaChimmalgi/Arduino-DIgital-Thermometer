#include <OneWire.h> 

int DS18S20_Pin = 12; //Probe connected to pin 12
char tmpstring[10];
int tmp;

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2
#define Button  11

int Units = 9;
int Tens = 10;
int Counter = 0;
unsigned long Timer = 0;

int Zero[7] = {1,1,1,0,1,1,1};
int One[7] = {0,0,1,0,0,0,1};
int Two[7] = {1,1,0,1,0,1,1};
int Three[7] = {0,1,1,1,0,1,1};
int Four[7] = {0,0,1,1,1,0,1};
int Five[7] = {0,1,1,1,1,1,0};
int Six[7] = {1,1,1,1,1,1,0};
int Seven[7] = {0,0,1,0,0,1,1};
int Eight[7] = {1,1,1,1,1,1,1};
int Nine[7] = {0,1,1,1,1,1,1};

void Units_DisplayNumber(int Number[7]){
  digitalWrite(Units,LOW);
  digitalWrite(Tens,HIGH);
  for (int i = 0; i < 7; i++){
    if(Number[i] == 1){
      digitalWrite(i+2,HIGH);
    }
    else{
      digitalWrite(i+2,LOW);
    }
  }
  delay(5);
}

void Tens_DisplayNumber(int Number[7]){
  digitalWrite(Units,HIGH);
  digitalWrite(Tens,LOW);
  for (int i = 0; i < 7; i++){
    if(Number[i] == 1){
      digitalWrite(i+2,HIGH);
    }
    else{
      digitalWrite(i+2,LOW);
    }
  }
  delay(5);
}


void setup() {
  Serial.begin(9600);
  for(int i = 2; i < 9; i++){
    pinMode(i,OUTPUT);
  }
  pinMode(Units, OUTPUT);
  pinMode(Tens, OUTPUT);
  pinMode(Button,INPUT_PULLUP);
}

void DisplayNumber(int Number){
  int Units_Value = Number % 10;
  int Tens_Value = Number / 10;
    if(Units_Value == 0)   Units_DisplayNumber(Zero);
    else if(Units_Value == 1)  Units_DisplayNumber(One);
    else if(Units_Value == 2)  Units_DisplayNumber(Two);
    else if(Units_Value == 3)  Units_DisplayNumber(Three);
    else if(Units_Value == 4)  Units_DisplayNumber(Four);
    else if(Units_Value == 5)  Units_DisplayNumber(Five);
    else if(Units_Value == 6)  Units_DisplayNumber(Six);
    else if(Units_Value == 7)  Units_DisplayNumber(Seven);
    else if(Units_Value == 8)  Units_DisplayNumber(Eight);
    else if(Units_Value == 9)  Units_DisplayNumber(Nine);

    if(Tens_Value == 0) Tens_DisplayNumber(Zero);
    else if(Tens_Value == 1)  Tens_DisplayNumber(One);
    else if(Tens_Value == 2)  Tens_DisplayNumber(Two);
    else if(Tens_Value == 3)  Tens_DisplayNumber(Three);
    else if(Tens_Value == 4)  Tens_DisplayNumber(Four);
    else if(Tens_Value == 5)  Tens_DisplayNumber(Five);
    else if(Tens_Value == 6)  Tens_DisplayNumber(Six);
    else if(Tens_Value == 7)  Tens_DisplayNumber(Seven);
    else if(Tens_Value == 8)  Tens_DisplayNumber(Eight);
    else if(Tens_Value == 9)  Tens_DisplayNumber(Nine);
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return (((TemperatureSum * 18 + 5)/10))*5/9;
}

void loop(void) {
  if(millis() > Timer+2000){
    float temperature = getTemp();
    tmp = (int) temperature;
    if(tmp > 99)  tmp = 99;
    Timer = millis();
  }
  DisplayNumber(tmp);
}

