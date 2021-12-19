int latchPin = 10;  
int clockPin = 13; 
int dataPin = 11;  
long int curr=0;
uint32_t col_var=0;
uint32_t mask=0;
uint8_t row_var=0;
byte leds = 0; 
uint8_t rows[5] = {0x40,0x20, 0x10, 0x08, 0x04};

/*
 *reset - All OFF 
 *On(delay) - All ON  
 *multiplexDemo() - Multiplex ON demo
 *testLeds - All LED one by one 
 *randLeds(delay) - Random LEDS ON and OFF 
 *shift25 - Sends 25 bits of data acccepts row value if not specified all rows are lit.
 *desings - all edges only
 *faces - All Faces shift (FB, BF, LR, RL)
 */
void randLeds(int del=100);
void shift25(uint32_t data, int row=-1);
void On(int del=1000);
void faces();
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
faces();
}
//multiplexDemo();
void designs(){
 //Cube Borders
  for(int i=0;i<5;i++){
  digitalWrite(latchPin, LOW);
  if(i==0 || i==4)
  shift25(0x1F8C63F,i);
  else
  shift25(0x1100011,i);
  digitalWrite(latchPin, HIGH);
  delay(2);
  }
}

void faces(){
//back to front
int count=3,del=100;
while(count>0){
  for(int i=0;i<5;i++){
  digitalWrite(latchPin, LOW);
  col_var=i==0?0x1f:(col_var<<5);
  shift25(col_var);
  digitalWrite(latchPin, HIGH);
  delay(del);}
  count--; 
}
//front to back
count=3;
while(count>0){
  for(int i=0;i<5;i++){
  digitalWrite(latchPin, LOW);
  col_var=i==0?(0x1F00000):(col_var>>5);
  shift25(col_var);
  digitalWrite(latchPin, HIGH);
  delay(del);}
  count--; 
}
//left to right
count=3;
while(count>0){
  for(int i=0;i<5;i++){
  digitalWrite(latchPin, LOW);
  col_var=i==0?(0x108421):(col_var<<1);
  shift25(col_var);
  digitalWrite(latchPin, HIGH);
  delay(del);}
  count--; 
}
//right to left
count=3;
while(count>0){
  for(int i=0;i<5;i++){
  digitalWrite(latchPin, LOW);
  col_var=i==0?(0x1084210):(col_var>>1);
  shift25(col_var);
  digitalWrite(latchPin, HIGH);
  delay(del);}
  count--; 
}
}

void shift25(uint32_t data, int row=-1){
  if(row == -1)
  shiftOut(dataPin, clockPin, LSBFIRST,((data&1)<<7)|0x7c);
  else
  shiftOut(dataPin, clockPin, LSBFIRST, ((data&1)<<7)|rows[row]);
 
  shiftOut(dataPin, clockPin, LSBFIRST, (data>>1)&0xFF);
  shiftOut(dataPin, clockPin, LSBFIRST, ((data>>1)&0xFF00)>>8);
  shiftOut(dataPin, clockPin, LSBFIRST, ((data>>1)&0xFF0000)>>16);  
}
void randLeds(int del=100){
  digitalWrite(latchPin, LOW);
  col_var=0;
  row_var=random(0,23);
  for(int i=0; i<row_var; i++)
  bitSet(col_var, random(0,23));
  row_var=random(0,4);
  shift25(col_var,row_var);
  digitalWrite(latchPin, HIGH);
  delay(del);
}
void On(int del=1000){
  
  for(int i =0 ; i<5;i++){
  digitalWrite(latchPin, LOW);
//  leds = rows[i] | 0x80;
//  shiftOut(dataPin, clockPin, LSBFIRST, leds);
//  leds=0xFF;
//  for(int i=0;i<3;i++)
//  {
//    shiftOut(dataPin, clockPin, LSBFIRST, leds);
//  }
  col_var=0x1FFFFFF;
  shift25(col_var,i);
  digitalWrite(latchPin, HIGH);
  delay(del);
 }
 
}

void testLeds(){
for(int j=0;j<5;j++){
for(int i=0;i<25;i++)
{
digitalWrite(latchPin, LOW);
col_var=0;
bitSet(col_var,i);
shift25(col_var, j);
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
