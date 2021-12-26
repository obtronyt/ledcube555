#include <SPI.h>
#define POV_DELAY = 64 //260us 3846Hz
uint32_t col_var=0;
volatile uint8_t curr_row=0;
uint8_t rows[5] = {0x40,0x20, 0x10, 0x08, 0x04};
uint8_t rand_cols[25];
uint32_t heli[8]={0x1084,0x1110,0x7000,0x1041000,0x421000,0x111000,0x1c00,0x1041};
volatile uint32_t shiftReg[5];
uint32_t numbers[10][5]={
                        {0xe,0xa,0xa,0xa,0xe},
                        {},
                        {},
                        {},
                        {},
                        {},
                        {},
                        {},
                        {},
                        {}
                      };


//x,y,z based
// Timer 1 PreScale - 64, Freq = 3846
/*
 *************UTILITY FUNCTIONS*******************
 *shift25 - Sends 25 bits of data acccepts row value if not specified all rows are lit.
 *reset - All OFF   
 *On(delay) - All ON  
 *pixelSet, pixelDel, pixelVal - set pixel value based on either xyz co-ord o led pos(0-24) and z value
 *setAllRows - Set same value to all rows(z)
 ************ANIMATIONS***************************
 *animateAll - all animations
 *testLeds - All LED one by one 
 *randLeds(delay) - Random LEDS ON and OFF 
 *cubeBorder - all edges only (5x5x5, 3x3x3, 1x1x1)
 *faces - All Faces shift (FB, BF, LR, RL)
 *helicopter - FAN animation
 *rainFall - drop falling effect
 */

//x y z co-ord based pixel on
void pixelSet(uint8_t, uint8_t, uint8_t); 
void pixelDel(uint8_t, uint8_t, uint8_t);
//led pos(x*5+y), z co-rd based pixel on. (Used during generating non repeating random columns)
void pixelSet(uint8_t, uint8_t);
void pixelDel(uint8_t, uint8_t);
//Get current pixel val (x,y,z)
uint8_t pixelVal(uint8_t, uint8_t, uint8_t);

//Functions
void randLeds(int del=100);
void shift25(uint32_t data, int row=-1);
void setup() 
{
  //8Mhz SPI for shift reg communication
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, LSBFIRST, SPI_MODE0));
  for(int i=0; i<25;i++)
    rand_cols[i]=i;
  // Setting up Timer 1
  cli();
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  TCNT1  = 0; // Initialize counter value to 0
  // Set compare match register for 3846 Hz increments
  //15624, CS12 CS10 1Hz | 64, CS11 CS10 3846 Hz
  //OCR1A = 15624; // This is used for testing 
  OCR1A = 64 ;// = [(16*10^6) / (3846*64)] - 1 (must be <65536)
  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // Prescale 1024
  //TCCR1B |= (1 << CS12) | (1 << CS10); //Testing 1 Hz
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
  reset();
  //Serial.begin(115200);
}

//LED CUBE Refresh row by row
ISR(TIMER1_COMPA_vect){//timer1 triggers every 260us
  digitalWrite(SS,LOW);
  shift25(shiftReg[curr_row],curr_row);
  digitalWrite(SS,HIGH);
  curr_row++;
  if(curr_row == 5)
    curr_row=0;
}

void loop() 
{
  animateAll();
}

//Convert input data to covert 8bit chunks based on row values
void shift25(uint32_t data, int row=-1){
  if(row == -1)
  SPI.transfer(((data&1)<<7)|0x7c);
  else
  SPI.transfer( ((data&1)<<7)|rows[row]);
  SPI.transfer((data>>1)&0xFF);
  SPI.transfer(((data>>1)&0xFF00)>>8);
  SPI.transfer(((data>>1)&0xFF0000)>>16);  
}

void On(){
  for(int i =0 ;i<5;i++)
    shiftReg[i]=0x1ffffff;
}

void reset(){
  for(int i=0;i<5;i++)
      shiftReg[i]=0;
}

void pixelSet(uint8_t x, uint8_t y, uint8_t z){
    shiftReg[z] = shiftReg[z] | 1L<<((x*5)+y);
    //Serial.println(tmp,HEX);
}
void pixelDel(uint8_t x, uint8_t y, uint8_t z){
    shiftReg[z] = shiftReg[z] & ((1L<<(y+(x*5))) ^ 0x1ffffff);
}
int pixelVal(int x, int y, int z){
    return ((shiftReg[z] & (1L<<(y+(x*5)))) ==  (1L<<(y+(x*5))));
}
void pixelSet(uint8_t ledPos, uint8_t z){
    shiftReg[z] = shiftReg[z] | 1L<<ledPos;
    //Serial.println(tmp,HEX);
}
void pixelDel(uint8_t ledPos, uint8_t z){
    shiftReg[z] = shiftReg[z] & ((1L<<ledPos) ^ 0x1ffffff);
}

void setAllRows(uint_fast32_t value){
  for(int j=0;j<5;j++)
    shiftReg[j]=value;
}

void animateAll(){
  reset();
  delay(500);
  uint8_t c=3;
  testLeds();
  faces();
  while(c>0){
    cubeBorder();
    c--;
  }
  randLeds();
  c=3;
  while(c>0){
    helicopter();
    c--;
  }
  c=2;
  while(c>0){
    rainFall();
    c--;
  }
  for(int i=0;i<5;i++)
  {
    shiftReg[i]=0x1ffffff;
    delay(500);
  }
  delay(2000);
  }

  void cubeBorder(){
    //5x5x5 cube
    reset();
    for(int i=0;i<5;i++)
    {
    if(i==0||i==4)
    shiftReg[i]=0x1F8C63F;
    else
    shiftReg[i]=0x1100011;
    }
    delay(500);
    //3x3x3 cube | Value Generated from Py Program
    reset();
    shiftReg[1]=0x729c0;
    shiftReg[3]=0x729c0;
    shiftReg[2]=0x50140;
    delay(500);
    //1x1x1
    reset();
    pixelSet(2,2,2);
    delay(500);
}
void faces(){
  int count=3,del=100;
  //Bottom to Top
  while(count>0){
    for(int i=0;i<5;i++){
    reset();
    shiftReg[i]=0x1ffffff;
    delay(del);} 
    count --;
  }
  //Top To Bottom
  count=3;
  while(count>0){
    for(int i=4;i>=0;i--){
    reset();
    shiftReg[i]=0x1ffffff;
    delay(del);} 
    count --;
  }
  // //back to front
  count=3;
  while(count>0){
    for(int i=0;i<5;i++){
    col_var=i==0?0x1f:(col_var<<5);
    setAllRows(col_var);
    delay(del);}
    count--; 
  }
  //front to back
  count=3;
  while(count>0){
    for(int i=0;i<5;i++){
    col_var=i==0?(0x1F00000):(col_var>>5);
    setAllRows(col_var);
    delay(del);}
    count--; 
  }
  reset();
  //left to right
  count=3;
  while(count>0){
    for(int i=0;i<5;i++){
    col_var=i==0?(0x108421):(col_var<<1);
    setAllRows(col_var);
    delay(del);}
    count--; 
  }
  //right to left
  count=3;
  while(count>0){
    for(int i=0;i<5;i++){
    col_var=i==0?(0x1084210):(col_var>>1);
    setAllRows(col_var);
    delay(del);}
    count--; 
  }
}

void randLeds(int del=100){
  reset();
  for(int numLed=0; numLed<80; numLed++){
      pixelSet(random(0,4),random(0,4),random(0,4));
      delay(del);
      }
}

void rainFall(){
  reset();
  uint8_t swap,pos,z=4;
  for(int i=0;i<25;i++)
    { pos=random(0,24);
      swap=rand_cols[i];
      rand_cols[i]=rand_cols[pos];
      rand_cols[pos]=swap;
    }
  shiftReg[z]=0x1ffffff; 
  for(int numLeds=0; numLeds<25; numLeds++){
    z=4;
    delay(300);
    pixelDel(rand_cols[numLeds], z);
    for(z=3;z>=1;z--){
      pixelSet(rand_cols[numLeds], z);
      delay(20);
      pixelDel(rand_cols[numLeds] ,z);
      }
    pixelSet(rand_cols[numLeds] ,z);
    delay(100);
  }
  delay(700);
}


void testLeds(){
  reset();
  for(int i=0;i<5;i++)
  {
    for(int j=0;j<5;j++)
        for(int m=0;m<5;m++){
          pixelSet(j,m,i);
          delay(100);
          }
  }
}

void helicopter(){
  for(uint8_t i = 0; i<8;i++)
  {
    setAllRows(heli[i]);
    delay(250);
  }
}

void disp(){
  for(int i=0;i <5; i++)
      Serial.println(shiftReg[i]);
}
