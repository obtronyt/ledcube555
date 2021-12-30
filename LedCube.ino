/*
Author: Obtron
Use this as you please
*/


#include <SPI.h>
#include <math.h>
uint32_t col_var=0;
volatile uint8_t curr_row=0;
uint8_t rows[5] = {0x40,0x20, 0x10, 0x08, 0x04};
uint8_t rand_cols[25];
uint32_t heli[8]={0x1084,0x1110,0x7000,0x1041000,0x421000,0x111000,0x1c00,0x1041};
uint32_t heliFull[4]={0x421084,0x111110,0x7c00,0x1041041};
volatile uint32_t shiftReg[5];
uint32_t numbers[10][5]={
                        {0xe,0xa,0xa,0xa,0xe}, //0
                        {0xe,0x4,0x4,0x4,0x6}, //1
                        {0xe,0x2,0xe,0x8,0xe}, //2
                        {0xe,0x8,0xe,0x8,0xe}, //3
                        {0x8,0x8,0xe,0xa,0xa}, //4
                        {0xe,0x8,0xe,0x2,0xe}, //5
                        {0xe,0xa,0xe,0x2,0xe}, //6
                        {0x8,0x8,0x8,0x8,0xe}, //7
                        {0xe,0xa,0xe,0xa,0xe}, //8
                        {0xe,0x8,0xe,0xa,0xe}  //9
                      };
uint32_t alpha[26][5]={
                        {0x11,0x11,0x1f,0x11,0xe}, //a
                        {0xf,0x11,0x1f,0x11,0xf}, //b
                        {0x1f,0x1,0x1,0x1,0x1f}, //c
                        {0xf,0x11,0x11,0x11,0xf}, //d
                        {0x1f,0x1,0x1f,0x1,0x1f}, //e
                        {0x1,0x1,0x1f,0x1,0x1f}, //f
                        {0xf,0x10,0x1f,0x11,0x1f}, //g
                        {0x11,0x11,0x1f,0x11,0x11}, //h
                        {0x1f,0x4,0x4,0x4,0x1f}, //i
                        {0x7,0x4,0x4,0x4,0x1f},  //j
                        {0x11,0x9,0x7,0x9,0x11},  //k
                        {0x1f,0x1,0x1,0x1,0x1},  //l
                        {0x11,0x11,0x15,0x1b,0x11},  //m
                        {0x11,0x19,0x15,0x13,0x11},  //n
                        {0xe,0x11,0x11,0x11,0xe},  //o
                        {0x1,0x1,0x1f,0x11,0x1f},  //p
                        {0x1f,0x19,0x15,0x11,0x1f},  //q
                        {0x11,0x9,0x1f,0x11,0x1f},  //r
                        {0x1f,0x10,0x1f,0x1,0x1f},  //s
                        {0x4,0x4,0x4,0x4,0x1f},  //t
                        {0x1f,0x11,0x11,0x11,0x11},  //u
                        {0x4,0xa,0x11,0x11,0x11},  //v
                        {0x11,0x1b,0x15,0x11,0x11},  //w
                        {0x11,0xa,0x4,0xa,0x11},  //x
                        {0x4,0x4,0x4,0xa,0x11},  //y
                        {0x1f,0x2,0x4,0x8,0x1f},  //z
                      };
uint32_t sine2d[5][5]={
                        {0x8,0x14,0x15,0x5,0x2},
                        {0x10,0x9,0xb,0xa,0x4},
                        {0x1,0x12,0x16,0x14,0x8},
                        {0x2,0x5,0xd,0x9,0x10},
                        {0x4,0xa,0x1a,0x12,0x1}
                      };
char allstr[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char str[]="HAPPYNEWYEAR";
char ytname[]="OBTRON";
uint8_t numArray[4]={2,0,2,2};
//x,y,z based
// Timer 1 PreScale - 64, Freq = 3846
/*
 *************UTILITY FUNCTIONS*******************
 *shift25 - Sends 25 bits of data acccepts row value if not specified all rows are lit.
 *reset - All OFF   
 *On(delay) - All ON  
 *pixelSet, pixelDel, pixelVal - set pixel value based on either xyz co-ord o led pos(0-24) and z value
 *setAllRows - Set same value to all rows(z)
 *setAllFaces - Set all Faces with same value
 ************ANIMATIONS***************************
 *animateAll - all animations
 *testLeds - All LED one by one 
 *randLeds(delay) - Random LEDS ON and OFF 
 *cubeBorder - all edges only (5x5x5, 3x3x3, 1x1x1)
 *faces - All Faces shift (FB, BF, LR, RL)
 *sinewave - Sine wave animation
 *sinewave3d - 3d Sinewave
 *helicopter - FAN animation
 *rainFall - drop falling effect
 *disp_num, disp_alpha, disp_str, disp_numArray - Display user specified alpha and numbers
 *allNum - Display all numbers 
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
void shift25(uint32_t data, int row);
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
void shift25(uint32_t data, int row){
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

void setAllRows(uint32_t value){
  for(int j=0;j<5;j++)
    shiftReg[j]=value;
}

void setAllFaces(uint32_t value, uint8_t reg){
    shiftReg[reg] = value<<(0) | value<<(5) | value<<(10) | value<<(15) | value<<(20) ; 
}


void disp_string(char * st){
  uint8_t c=0;
  while(st[c]!='\0'){
    disp_alpha(st[c]);
    c++;
  }
}

void disp_alpha(char c){
  uint8_t temp = c - 'A'; 
  for(int i=0;i<5;i++)
  { 
    for(int j=0; j<5;j++)
      shiftReg[j]=alpha[temp][j]<<(i*5);
    delay(200);    
  }
}

void disp_numArr(uint8_t * arr, uint8_t len){
  for(uint8_t i=0; i<len; i++)
    disp_num(arr[i]);
}

void disp_num(uint8_t num){
  for(int i=0;i<5;i++)
  { 
    for(int j=0; j<5;j++)
      shiftReg[j]=numbers[num][j]<<(i*5);
    delay(200);    
  }
}

void allNum(){
  for(int p=0;p<10;p++){
  disp_num(p);
  delay(10);
  }
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
  sineWave3d();
  reset();
  helicopter();
  c=7;
  while(c>0){
    sinewave();
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
  disp_string(str);  
  disp_numArr(numArray,4);
  reset();
  delay(500);
  disp_string(ytname);
  reset();
  delay(3000);
  }

void sinewave(){
  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++)
    {
      setAllFaces(sine2d[i][j],j); 
    }
    delay(180);
  }
}

// Z=Phi+sin(sqrt(x^2+y^2))
void sineWave3d(){
  int   size=5;
  float phase = -1.5;
  float z=0;
    for(int t=0; t<15; t++){
      for(int i=0; i<8; i++)
      {
          phase=phase + 0.5;
          phase = millis() /1000.0 * PI * 2 * 1;
          for(int x = 0; x < size; x++){
              for(int y = 0; y < size; y++){
                  z = sin(phase + sqrt(pow(map(x,0,size-1,-PI,PI),2) + pow(map(y,0,size-1,-PI,PI),2)));
                  //Serial.println(Z);
                  z=map(z*100,-100,100,0,size-1);
                  pixelSet(x,y,(int)z);
              }
          }
          delay(85);
          reset();
      }
    }
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
    delay(80);
  }
  for(int numLeds=0; numLeds<25; numLeds++){
    z=0;
    pixelDel(rand_cols[numLeds], z);
    for(z=1;z<4;z++){
      pixelSet(rand_cols[numLeds], z);
      delay(20);
      pixelDel(rand_cols[numLeds] ,z);
      }
    pixelSet(rand_cols[numLeds] ,z);
    delay(80);
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
          delay(50);
          }
  }
  delay(500);
}

void helicopter(){
  for(uint8_t i=0;i<5;i++){
    shiftReg[i]=heli[0];
    delay(150);
  }
  for(uint8_t i = 0; i<24;i++)
  {
    setAllRows(heli[i%8]);
    delay(250);
  }
  setAllRows(heli[0]);
  delay(500);
  for(uint8_t i=0;i<5;i++){
    shiftReg[i]=heliFull[0];
    delay(150);
  }
  for(uint8_t i=0;i<25;i++){
    setAllRows(heliFull[i%4]);
    delay(250);
  }
}

void disp(){
  for(int i=0;i <5; i++)
      Serial.println(shiftReg[i]);
}
