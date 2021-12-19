int latchPin = 10;  
int clockPin = 13; 
int dataPin = 11;  
long int curr=0;
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

/*
 * loop() - this function runs over and over again
 */
void loop() 
{
multiplexDemo();
}

/*
 * updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
 */
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
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
