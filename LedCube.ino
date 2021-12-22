int latchPin = 10;  
int clockPin = 13; 
int dataPin = 11;  
long int delay_ctr_1=0,delay_ctr_2;
uint32_t col_var=0;
uint32_t mask=0;
uint8_t row_var=0;
byte leds = 0; 
uint8_t rows[5] = {0x40,0x20, 0x10, 0x08, 0x04};
uint8_t rand_cols[25];
uint32_t heli[8]={0x1084,0x1110,0x7000,0x1041000,0x421000,0x111000,0x1c00,0x1041};
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
void randLeds2(int del=100,int type=0);
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
  for(int i=0;i<25;i++)
  rand_cols[i]=i;
}


void loop() 
{

for(int i=0;i<8;i++)
{
digitalWrite(latchPin,LOW);
  shift25(heli[i]);
 digitalWrite(latchPin,HIGH);
  delay(250);
}
 
}

void designs(){
testLeds();
delay(100);
faces();
delay(100);
cubeBorder();
reset();
randLeds2(300,1);
delay(20);
randLeds2(300,0);
delay(20);
}
void cubeBorder(){
   //Cube Borders
  delay_ctr_1=millis();
  while(delay_ctr_1+2000>millis()){
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
}
void faces(){
int count=3,del=100;
//Bottom to Top
while(count>0){
  for(int i=0;i<5;i++){
  digitalWrite(latchPin, LOW);
  col_var=0x1FFFFFF;
  shift25(col_var,i);
  digitalWrite(latchPin, HIGH);
  delay(del);}
  count--; 
}
//Top To Bottom
count=3;
while(count>0){
  for(int i=4;i>=0;i--){
  digitalWrite(latchPin, LOW);
  col_var=0x1FFFFFF;
  shift25(col_var,i);
  digitalWrite(latchPin, HIGH);
  delay(del);}
  count--; 
}
//back to front
count=3;
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
  char num_leds=4;
  for(int count=0;count<7;count++){
  digitalWrite(latchPin, LOW);
  col_var=0;
  row_var=random(0,3);
  for(int i=0; i<num_leds; i++)
  bitSet(col_var, random(0,23));
  row_var=random(0,4);
  shift25(col_var,row_var);
  digitalWrite(latchPin, HIGH);
  delay(del);
  }
}
void randLeds2(int del=100, int type=0){
  uint8_t swap,pos;
  for(int i=0;i<25;i++)
  {  
     pos=random(0,24);
     swap=rand_cols[i];
     rand_cols[i]=rand_cols[pos];
     rand_cols[pos]=swap;
//     Serial.println(rand_cols[i]);
          
  }
  delay(2000);
  col_var=0;
    for(int i=0;i<25;i++){
  bitSet(col_var,rand_cols[i]);
  digitalWrite(latchPin, LOW);
  if(type!=0)
  shift25(col_var,random(0,4));
  else
  shift25(col_var);
  digitalWrite(latchPin, HIGH);
  delay(del);
  }
}

void On(int del=1000){
  
  for(int i =0 ; i<5;i++){
  digitalWrite(latchPin, LOW);
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
delay(30);
}
}  
}

void multiplexDemo(){
  int j=1,del=1000;
  while(j<9){
    del=del/2; // 1k 500 250 125 62 31 15 7
    delay_ctr_1=millis();
    while(millis()<delay_ctr_1+3000) 
    On(del);
    j++;
  }
  reset();
  delay(200);
}


void helicopter(){
digitalWrite(latchPin,LOW);

digitalWrite(latchPin,HIGH);  
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
