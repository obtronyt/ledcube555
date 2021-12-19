int latchPin = 10;  
int clockPin = 13; 
int dataPin = 11;  
long int curr=0;
uint32_t tmp=0;
uint32_t mask=0;
uint8_t slice=0;
byte leds = 0; 
byte rows[5] = {0x40,0x20, 0x10, 0x08, 0x04};

void setup() 
{
  // Set all the pins of 74HC595 as OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  leds=0;
  reset();
  delay(100);
}

void loop() 
{
testLeds();
}

void On(int del){
  for(int i =0 ; i<5;i++){
  digitalWrite(latchPin, LOW);
  leds = rows[i] | 0x80;
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  leds=0xFF;
  for(int i=0;i<3;i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, leds);
  }
  digitalWrite(latchPin, HIGH);
  delay(del);
  }
}

void testLeds(){
for(int j=0;j<5;j++){
for(int i=0;i<25;i++)
{
digitalWrite(latchPin, LOW);
tmp=0;
bitSet(tmp,i);
shiftOut(dataPin,clockPin, LSBFIRST,rows[j]|((tmp&1)<<7));
tmp=tmp>>1;
shiftOut(dataPin, clockPin, LSBFIRST, tmp&0xFF);
shiftOut(dataPin, clockPin, LSBFIRST, (tmp&0xFF00)>>8);
shiftOut(dataPin, clockPin, LSBFIRST, (tmp&0xFF0000)>>16);  
  digitalWrite(latchPin, HIGH);
  delay(100);
}
}  
}

void multiplexDemo(){
  int j=1,del=1000;
  while(j<9){
    del=del/2; // 1k 500 250 125 62 31 15 7
    curr=millis();
    while(millis()<curr+3000) 
    On(del);
    j++;
  }
  reset();
  delay(200);
}
void reset(){
  leds=0;
  digitalWrite(latchPin, LOW);
  for(int i=0;i<4;i++)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, leds);
  }
  digitalWrite(latchPin, HIGH);
  }
