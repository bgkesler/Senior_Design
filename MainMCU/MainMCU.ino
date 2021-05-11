// ArduCAM Mini demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Mini camera, and can run on any Arduino platform.
// This demo was made for ArduCAM_Mini_2MP_Plus.
// It needs to be used in combination with PC software.
// It can test OV2640 functions
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM_Mini_2MP_Plus
// and use Arduino IDE 1.6.8 compiler or above
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"
//This demo can only work on OV2640_MINI_2MP_PLUS platform.
#if !(defined OV2640_MINI_2MP_PLUS)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

// set pin 7 as the slave select for the digital pot:
#define CS 7
#define DATA_READY 8
#define LED_STOP 39
#define LED_CROSS 40
#define PWM_BUZZ 45
#define AUX_INT_PIN 3


#define BMPIMAGEOFFSET 66
#define THRESHOLD_SSD 1
#define THRESHOLD_SSD2 200000
#define THRESHOLD_SSD3 95000
#define tRows1 11
#define tCols1 27
#define tRows2 12
#define tCols2 26
#define tRows3 6
#define tCols3 12

#define pRows 24
#define pCols 32



byte temp1[tRows1][tCols1] = {34, 83, 115, 122, 126, 127, 54, 40, 43, 95, 143, 142, 93, 45, 58, 129, 130, 127, 124, 120, 52, 44, 97, 117, 101, 46, 25, 107, 137, 126, 127, 144, 157, 62, 42, 94, 166, 150, 152, 168, 90, 60, 142, 146, 165, 146, 135, 57, 113, 144, 114, 126, 123, 34, 134, 95, 34, 37, 102, 158, 62, 44, 149, 154, 58, 58, 161, 146, 49, 48, 71, 166, 87, 41, 41, 111, 86, 35, 53, 137, 62, 139, 73, 35, 37, 105, 161, 62, 63, 165, 121, 49, 49, 128, 166, 61, 48, 69, 167, 91, 42, 40, 38, 37, 36, 86, 138, 62, 138, 109, 54, 56, 117, 162, 63, 74, 171, 102, 51, 51, 108, 175, 72, 49, 68, 169, 96, 43, 41, 39, 56, 120, 144, 110, 32, 105, 146, 149, 154, 161, 162, 62, 82, 175, 99, 50, 51, 103, 179, 79, 50, 67, 170, 100, 44, 41, 56, 141, 143, 87, 35, 28, 32, 69, 94, 98, 137, 160, 60, 80, 175, 99, 49, 51, 105, 180, 77, 51, 64, 169, 103, 44, 41, 108, 150, 68, 33, 31, 28, 29, 33, 35, 37, 117, 159, 57, 70, 170, 115, 50, 51, 121, 173, 67, 49, 62, 167, 108, 43, 42, 128, 134, 35, 33, 43, 59, 29, 32, 35, 37, 120, 157, 56, 54, 161, 147, 51, 51, 151, 160, 52, 48, 60, 165, 110, 44, 40, 120, 140, 39, 33, 90, 129, 29, 32, 35, 37, 122, 154, 54, 45, 127, 171, 119, 122, 173, 119, 48, 47, 57, 163, 112, 42, 40, 80, 150, 117, 105, 135, 107, 29, 31, 34, 37, 125, 153, 51, 44, 60, 147, 170, 171, 143, 57, 48, 46, 52, 154, 109, 41, 39, 38, 93, 136, 135, 110, 43};
byte temp2[tRows2][tCols2] = {25,  11,  22,  39,  59,  37,  36,  43,  41,  17,  10,  9, 9, 9, 9, 10,  10,  18,  22,  23,  27,  20,  19,  26,  33,  28,
15,  30,  62,  59,  65,  77,  60,  17,  25,  38,  60,  59,  32,  9, 14,  31,  25,  20,  21,  36,  22,  17,  21,  31,  41,  38,
25,  57,  60,  21,  14,  51,  56,  31,  45,  73,  47,  46,  70,  22,  22,  63,  65,  65,  65,  45,  17,  42,  57,  49,  18,  6,
20, 64,  45,  16,  13,  60,  54,  31,  63,  64,  9, 10,  65,  43,  9, 11,  46,  63,  24,  26,  41,  61,  43,  54,  61,  8,
32, 126, 60,  34,  27,  64,  48,  19,  68,  44,  10,  9, 55,  54,  12,  10,  48,  44,  12,  38,  21,  22,  9, 21,  77,  43,
25, 56,  57,  70,  75,  78,  41,  16,  74,  33,  10,  9, 55,  56,  10,  10,  53,  40,  10,  15,  10,  9, 23,  61,  65,  13,
16, 14,  15,  15,  23,  75,  33,  17,  78,  32,  12,  10,  59,  50,  10,  11,  59,  35,  9, 11,  13,  51,  73,  62,  21,  17,
11, 11,  14,  12,  20,  76,  27,  16,  79,  32,  10,  11,  71,  42,  10,  12,  64,  31,  9, 9, 53,  67,  29,  9, 11,  36,
8, 9, 11,  9, 23,  77,  21,  11,  74,  45,  10,  26,  82,  26,  11,  14,  69,  27,  12,  15,  72,  34,  10,  16,  33,  31,
8, 9, 9, 10,  29,  73,  17,  10,  52,  75,  43,  70,  66,  13,  11,  18,  72,  23,  12,  18,  76,  26,  14,  35,  76,  23,
9, 9, 10,  10,  20,  38,  11,  10,  15,  58,  75,  63,  20,  12, 11,  23,  74,  20,  11,  11,  66,  60,  42,  65,  72,  13,
9, 10,  10,  10,  11,  10, 10,  10,  12,  11,  12,  10,  10,  10,  11,  15,  33,  12, 11,  12,  24,  61,  74,  66,  28,  13};

byte temp3[tRows3][tCols3] = {67, 116, 126, 101, 36, 105, 133, 131, 51, 116, 142, 117, 119, 51, 66, 112, 49, 134, 44, 108, 89, 40, 124, 63, 117, 52, 68, 118, 60, 125, 40, 79, 112, 42, 119, 68, 66, 118, 127, 126, 58, 126, 40, 74, 121, 42, 114, 75, 29, 32, 47, 127, 48, 134, 43, 83, 111, 41, 107, 80, 29, 31, 43, 129, 40, 115, 111, 131, 76, 39, 102, 84};
byte temp1_T[tCols1][tRows1] = {34,107,134,139,138,105,32,29,29,29,29,
83,137,95,73,109,146,69,33,32,32,31,
115,126,34,35,54,149,94,35,35,35,34,
122,127,37,37,56,154,98,37,37,37,37,
126,144,102,105,117,161,137,117,120,122,125,
127,157,158,161,162,162,160,159,157,154,153,
54,62,62,62,63,62,60,57,56,54,51,
40,42,44,63,74,82,80,70,54,45,44,
43,94,149,165,171,175,175,170,161,127,60,
95,166,154,121,102,99,99,115,147,171,147,
143,150,58,49,51,50,49,50,51,119,170,
142,152,58,49,51,51,51,51,51,122,171,
93,168,161,128,108,103,105,121,151,173,143,
45,90,146,166,175,179,180,173,160,119,57,
58,60,49,61,72,79,77,67,52,48,48,
129,142,48,48,49,50,51,49,48,47,46,
130,146,71,69,68,67,64,62,60,57,52,
127,165,166,167,169,170,169,167,165,163,154,
124,146,87,91,96,100,103,108,110,112,109,
120,135,41,42,43,44,44,43,44,42,41,
52,57,41,40,41,41,41,42,40,40,39,
44,113,111,38,39,56,108,128,120,80,38,
97,144,86,37,56,141,150,134,140,150,93,
117,114,35,36,120,143,68,35,39,117,136,
101,126,53,86,144,87,33,33,33,105,135,
46,123,137,138,110,35,31,43,90,135,110,
25,34,62,62,32,28,28,59,129,107,43};

byte temp2_T[tCols2][tRows2] = {25,15,25,20,32,25,16,11,8,8,9,9,
11,30,57,64,126,56,14,11,9,9,9,10,
22,62,60,45,60,57,15,14,11,9,10,10,
39,59,21,16,34,70,15,12,9,10,10,10,
59,65,14,13,27,75,23,20,23,29,20,11,
37,77,51,60,64,78,75,76,77,73,38,10,
36,60,56,54,48,41,33,27,21,17,11,10,
43,17,31,31,19,16,17,16,11,10,10,10,
41,25,45,63,68,74,78,79,74,52,15,12,
17,38,73,64,44,33,32,32,45,75,58,11,
10,60,47,9,10,10,12,10,10,43,75,12,
9,59,46,10,9,9,10,11,26,70,63,10,
9,32,70,65,55,55,59,71,82,66,20,10,
9,9,22,43,54,56,50,42,26,13,12,10,
9,14,22,9,12,10,10,10,11,11,11,11,
10,31,63,11,10,10,11,12,14,18,23,15,
10,25,65,46,48,53,59,64,69,72,74,33,
18,20,65,63,44,40,35,31,27,23,20,12,
22,21,65,24,12,10,9,9,12,12,11,11,
23,36,45,26,38,15,11,9,15,18,11,12,
27,22,17,41,21,10,13,53,72,76,66,24,
20,17,42,61,22,9,51,67,34,26,60,61,
19,21,57,43,9,23,73,29,10,14,42,74,
26,31,49,54,21,61,62,9,16,35,65,66,
33,41,18,61,77,65,21,11,33,76,72,28,
28,38,6,8,43,13,17,36,31,23,13,13};

byte temp3_T[tCols3][tRows3] = {67,119,117,66,29,29,
116,51,52,118,32,31,
126,66,68,127,47,43,
101,112,118,126,127,129,
36,49,60,58,48,40,
105,134,125,126,134,115,
133,44,40,40,43,111,
131,108,79,74,83,131,
51,89,112,121,111,76,
116,40,42,42,41,39,
142,124,119,114,107,102,
117,63,68,75,80,84};

//byte temp1_ram[tRows1][tCols1];
//byte temp2_ram[tRows2][tCols2];
//byte temp3_ram[tRows3][tCols3];

const char bmp_header[BMPIMAGEOFFSET] PROGMEM =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};
uint8_t rx_buf[pCols][pRows] = {0,};
int temp10[10] = {0,};



bool is_header = false;
bool AUX_result = false, Main_result = false;
int mode = 0;
uint8_t start_capture = 0;
#if defined (OV2640_MINI_2MP_PLUS)
  ArduCAM myCAM( OV2640, CS );
#else
  ArduCAM myCAM( OV5642, CS );
#endif
//uint8_t read_fifo_burst(ArduCAM myCAM);
void setup() {
  // put your setup code here, to run once:
  uint8_t vid, pid;
  uint8_t temp;
  Wire.begin();
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(AUX_INT_PIN), AUX_data_ready,CHANGE);
  Serial.println(F("ACK CMD ArduCAM Start! END"));
  // set the CS, DATA READY, LED_STOP, LED_CROSS as an output:
  pinMode(CS, OUTPUT);
  pinMode(DATA_READY, OUTPUT);
  pinMode(LED_CROSS, OUTPUT);
  pinMode(LED_STOP, OUTPUT);

  digitalWrite(LED_CROSS, LOW);
  digitalWrite(LED_STOP, LOW);
  digitalWrite(DATA_READY,LOW);
  digitalWrite(CS, HIGH);
  // initialize SPI:
  SPI.begin();
  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);
  while(1){
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55){
      Serial.println(F("ACK CMD SPI interface Error! END"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("ACK CMD SPI interface OK. END"));break;
    }
  }

  #if defined (OV2640_MINI_2MP_PLUS)
    while(1){
      //Check if the camera module type is OV2640
      myCAM.wrSensorReg8_8(0xff, 0x01);
      myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
      myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
      if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
        Serial.println(F("ACK CMD Can't find OV2640 module! END"));
        delay(1000);continue;
      }
      else{
        Serial.println(F("ACK CMD OV2640 detected. END"));break;
      } 
    }
  #endif
  //Change to BMP capture mode and initialize the OV5642 module
  myCAM.set_format(BMP);
  myCAM.InitCAM();
  #if defined (OV2640_MINI_2MP_PLUS)
    myCAM.OV2640_set_JPEG_size(OV2640_160x120);
  #endif
  delay(1000);
  myCAM.clear_fifo_flag();
  #if !(defined (OV2640_MINI_2MP_PLUS))
  myCAM.write_reg(ARDUCHIP_FRAMES,0x00);
  #endif
}

void fill_template(byte arr[tRows1][tCols1], byte sel) {
    int t_len = tRows1 * tCols1;
    Serial.println("Filling template");
    //Serial.println(t_len);
    if(sel == 0)
    {
      memcpy_P(arr, temp1, t_len);
    }
    else if(sel == 1)
    {
      memcpy_P(arr, temp2, t_len);
    }
    else
    {
      memcpy_P(arr, temp3, t_len);
    }
}

void print_template(byte arr[tCols1][tRows1]) {
    Serial.println("Printing template");
    for (byte i = 0; i < tRows1; i++) {
        for (byte j = 0; j < tCols1; j++) {
           // Serial.print("tmp(");
            //Serial.print(i);
            //Serial.print(", ");
            //Serial.print(j);
            //Serial.print(") = ");
            Serial.print(arr[j][i]);
            Serial.print(",");
        }
        Serial.println();
    }
}

void print_T(byte arr[tCols1][tRows1], byte arr_old[tRows1][tCols1]) {
    //Serial.println("Printing template");
    for (byte i = 0; i < tCols1; i++) {
        for (byte j = 0; j < tRows1; j++) {
          arr[i][j] = arr_old[j][i];
           // Serial.print("tmp(");
            //Serial.print(i);
            //Serial.print(", ");
            //Serial.print(j);
            //Serial.print(") = ");
            //Serial.print(arr[i][j]);
            //Serial.print(",");
        }
        //Serial.println();
    }
}

void print_T2(byte arr[tCols2][tRows2], byte arr_old[tRows2][tCols2]) {
    Serial.println("Printing template2T");
    for (byte i = 0; i < tCols2; i++) {
        for (byte j = 0; j < tRows2; j++) {
          arr[i][j] = arr_old[j][i];
           // Serial.print("tmp(");
            //Serial.print(i);
            //Serial.print(", ");
            //Serial.print(j);
            //Serial.print(") = ");
            Serial.print(arr[i][j]);
            Serial.print(",");
        }
        Serial.println();
    }
}

void print_T3(byte arr[tCols3][tRows3], byte arr_old[tRows3][tCols3]) {
    Serial.println("Printing template3T");
    for (byte i = 0; i < tCols3; i++) {
        for (byte j = 0; j < tRows3; j++) {
          arr[i][j] = arr_old[j][i];
           // Serial.print("tmp(");
            //Serial.print(i);
            //Serial.print(", ");
            //Serial.print(j);
            //Serial.print(") = ");
            Serial.print(arr[i][j]);
            Serial.print(",");
        }
        Serial.println();
    }
}

long int ssd(byte pic[pCols][pRows], byte temp1[tCols1][tRows1], byte temp2[tCols2][tRows2], byte temp3[tCols3][tRows3], byte sel) {
    //Serial.println("Start SSD");
    long int ssd_min = 0;
    //int match_x = 0;
    //int match_y = 0;
    if(sel == 1)
    {
      for (byte i = 0; i <= pCols - tCols1; i++) {
        for (byte j = 0; j <= pRows - tRows1; j++) {
            long int ssd = 0;
            for (byte ii = 0; ii < tCols1; ii++) {
                for (byte jj = 0; jj < tRows1; jj++) {
                    long int diff = temp1[ii][jj] - pic[i+ii][j+jj];
                    ssd = ssd + (diff * diff);
                }
            }
            /*
            Serial.print("SSD( ");
            Serial.print(i);
            Serial.print(", ");
            Serial.print(j);
            Serial.print(") = ");
            Serial.println(ssd);
            */
            if ((i == 0 && j ==0) || ssd < ssd_min) {
                /*Serial.println("New Min");
                Serial.print("SSD( ");
                Serial.print(i);
                Serial.print(", ");
                Serial.print(j);
                Serial.print(") = ");
                Serial.println(ssd);*/
                ssd_min = ssd;
                //match_x = i;
                //match_y = j;
            }
        }
    }
    }
    else if(sel ==2)
    {
      for (byte i = 0; i <= pCols - tCols2; i++) {
        for (byte j = 0; j <= pRows - tRows2; j++) {
            long int ssd = 0;
            for (byte ii = 0; ii < tCols2; ii++) {
                for (byte jj = 0; jj < tRows2; jj++) {
                    long int diff = temp2[ii][jj] - pic[i+ii][j+jj];
                    ssd = ssd + (diff * diff);
                }
            }
            /*
            Serial.print("SSD( ");
            Serial.print(i);
            Serial.print(", ");
            Serial.print(j);
            Serial.print(") = ");
            Serial.println(ssd);
            */
            if ((i == 0 && j ==0) || ssd < ssd_min) {
                //Serial.println("New Min");
                //Serial.print("SSD( ");
                //Serial.print(i);
                //Serial.print(", ");
                //Serial.print(j);
                //Serial.print(") = ");
                //Serial.println(ssd);
                ssd_min = ssd;
                //match_x = i;
                //match_y = j;
            }
        }
    }
    }
    else
    {
      for (byte i = 0; i <= pCols - tCols3; i++) {
        for (byte j = 0; j <= pRows - tRows3; j++) {
            long int ssd = 0;
            for (byte ii = 0; ii < tCols3; ii++) {
                for (byte jj = 0; jj < tRows3; jj++) {
                    long int diff = temp3[ii][jj] - pic[i+ii][j+jj];
                    ssd = ssd + (diff * diff);
                }
            }
            /*
            Serial.print("SSD( ");
            Serial.print(i);
            Serial.print(", ");
            Serial.print(j);
            Serial.print(") = ");
            Serial.println(ssd);
            */
            if ((i == 0 && j ==0) || ssd < ssd_min) {
                //Serial.println("New Min");
                //Serial.print("SSD( ");
                //Serial.print(i);
                //Serial.print(", ");
                //Serial.print(j);
                //Serial.print(") = ");
                //Serial.println(ssd);
                ssd_min = ssd;
                //match_x = i;
                //match_y = j;
            }
        }
    }
    }
    //Serial.println("Finish SSD");
    return ssd_min;
}


void loop() {
  // put your main code here, to run repeatedly:
  long int start_time = millis();
  uint8_t temp = 0xff, temp_last = 0;
  bool is_header = false;
  int i = 0, j = 0;
  for(i=0;i<24;i++){
    for(j=0;j<32;j++){
      rx_buf[j][i]=0;
    }
  }
  
  mode = 3;
  temp = 0xff;
  start_capture = 3;
  //Serial.println(F("ACK CMD CAM start single shoot. END"));
  temp = 0xff;
  myCAM.OV2640_set_JPEG_size(OV2640_160x120);
  myCAM.set_format(BMP);
  myCAM.InitCAM();
  #if !(defined (OV2640_MINI_2MP_PLUS))        
    myCAM.clear_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
  #endif
  myCAM.wrSensorReg16_8(0x3818, 0x81);
  myCAM.wrSensorReg16_8(0x3621, 0xA7);
  //myCAM.OV2640_set_Special_effects(BW);temp = 0xff;
  //Serial.println(F("ACK CMD Set to BW END"));    
  //delay(1000);
 

  //Serial.println("startcapture");
  //Flush the FIFO
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  //Start capture
  myCAM.start_capture();
  delay(100);
  start_capture = 0;

  if (myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
  {
    //Serial.println(F("ACK CMD CAM Capture Done. END"));delay(50);
    uint8_t temp, temp_last;
    uint32_t length = 0;
    length = myCAM.read_fifo_length();
    if (length >= MAX_FIFO_SIZE ) 
    {
      Serial.println(F("ACK CMD Over size. END"));
      myCAM.clear_fifo_flag();
      return;
    }
    if (length == 0 ) //0 kb
    {
      Serial.println(F("ACK CMD Size is 0. END"));
      myCAM.clear_fifo_flag();
      return;
    }
    myCAM.CS_LOW();
    myCAM.set_fifo_burst();//Set fifo burst mode
    
    //Serial.write(0xFF);
    //Serial.write(0xAA);
    for (temp = 0; temp < BMPIMAGEOFFSET; temp++)
    {
      //Serial.write(pgm_read_byte(&bmp_header[temp]));
    }
    
    // code to process image
    byte VH, VL;
    byte temp10Index = 0;
    byte lowResRow = 0;
    byte rowNum = 9;
    //byte lowResCol = 0;
    //Serial.print(convertRGBtoGrey(VH,VL));
    for (i = 0; i < 240; i++)
    {
      for (j = 0; j < 320; j++)
      {
        VH = SPI.transfer(0x00);
        VL = SPI.transfer(0x00);
        //Serial.print(VL);
        //delayMicroseconds(12);
        //Serial.print(" ");
        //delayMicroseconds(12);
        //Serial.print(VH);
        //delayMicroseconds(12);
        //Serial.print(" ");
        //delayMicroseconds(12);
        //Serial.print(" ");
        //delayMicroseconds(12);
        //Serial.println(" ");
        temp10[temp10Index++] = convertRGBtoGrey(VH,VL); // store grey pixel in temp array
        if (temp10Index >= 10)
        {
          temp10Index = 0;
          uint16_t sumTemp10 = 0;
          for (int x = 0; x < 10; x++)
          {
            sumTemp10 += temp10[x];
          }
          sumTemp10 /=10; // average 10 pixels
          // store 10 pixel average in rx_buf with moving weights.

          //Serial.println(((j+1)/10)-1,DEC);
          //delayMicroseconds(12);
          if((((j+1)/10)-1)==0){//new addition (trying to track num rows because curr method hav neg index ie:((i+1)/10)-1 = -1)
            rowNum++;
            if(rowNum>=10){
              rowNum=0;
              lowResRow++;
            }
          }
          //Serial.println(lowResRow-1,DEC);
          //delayMicroseconds(12);
          //Serial.println(rowNum,DEC);
          //delayMicroseconds(12);
          //
          //
          rx_buf[((j+1)/10)-1][lowResRow-1/*((i+1)/10)-1*/] = ((rx_buf[((j+1)/10)-1][lowResRow-1/*((i+1)/10)-1*/] * (rowNum)) + sumTemp10)/(rowNum+1);
          //Serial.println(rx_buf[((j+1)/10)-1][lowResRow-1],HEX);
        }
        
      }
      //Serial.print(";");
      //Serial.println();
    }
    long int mid2_time = millis();
    
    printRXBUF();
    //Serial.println("Template");
    //print_template(temp1_T);
    long int mid_time = millis();
    //Serial.println("Transpose Template");
    //print_T(temp1_T, temp1);
    //print_T2(temp2_T, temp2);
    //print_T3(temp3_T, temp3);
    //SSD CALC GOES HERE!!!!!!!!
    //int my_sel = 1;
    //fill_template(temp, my_sel);
    //long int result1 = ssd(rx_buf, temp1_T, temp2_T, temp3_T, 1);
    long int result2 = ssd(rx_buf, temp1_T, temp2_T, temp3_T, 2);
    long int result3 = ssd(rx_buf, temp1_T, temp2_T, temp3_T, 3);
    //Serial.println();
    Serial.println("SSD for the template 1,2,3");
    //Serial.println(result1);
    Serial.println(result2);
    Serial.println(result3);
    // RESULTS FOR MAIN MCU ARE HERE!!!!!!!
      if(/*result1 < THRESHOLD_SSD || result2 < THRESHOLD_SSD2 || */result3 < THRESHOLD_SSD3)
      {
        //Serial.println("STOP SIGN DETECTED");
  
        
        Main_result = true;
        
      }
      else
      {
        //Serial.println("NO STOP SIGN DETECTED");
  
        Main_result = false;
      }
  
      if (AUX_result) // Aux LED
      {
        digitalWrite(LED_CROSS, HIGH);
      }
      else
      {
        digitalWrite(LED_CROSS, LOW); 
      }
      if (Main_result) // Main LED
      {
        digitalWrite(LED_STOP, HIGH); 
      }
      else
      {
        digitalWrite(LED_STOP, LOW);
      }
      if (Main_result || AUX_result) // BUZZER
      {
        analogWrite(PWM_BUZZ,50); 
        delay (500);
        analogWrite(PWM_BUZZ,0); 
      }
      else
      {
        analogWrite(PWM_BUZZ,0);
        delay(500);  
      }
  
    
    long int end_time = millis();
    //Serial.println(result2);
    //Serial.println(result3);
    //Serial.println();
    //Serial.write(0xBB);
    //Serial.write(0xCC);
    myCAM.CS_HIGH();
    //Clear the capture done flag
    myCAM.clear_fifo_flag();
    myCAM.flush_fifo();
    long int end_time2 = millis();
    //Serial.println();
    //Serial.println(start_time);
    //Serial.println(mid2_time - start_time);
    //Serial.println(mid_time - mid2_time);
    //Serial.println(end_time - mid_time);
    //Serial.println(end_time2 - start_time);
    //Serial.println();

  }
}


void printRXBUF()
{

  digitalWrite(DATA_READY, HIGH); // signal that data is ready to send
  // print serially the buffer, write over I2C to AUX
  //Serial.println();
  for (int y = 0; y < 24; y++){
    Wire.beginTransmission(8); // transmit to device #8
    for(int x = 0; x < 32; x++){
       //Serial.print(F(" "));
       delayMicroseconds(12);
      // Serial.print(rx_buf[x][y],DEC); // UART to PC
       delayMicroseconds(12);
      // Serial.print(F(","));
       delayMicroseconds(12);
       Wire.write(rx_buf[x][y]); // I2C to AUX
       delayMicroseconds(12);
    }
    Wire.endTransmission();    // stop transmitting
    //Serial.println();
  }

  
  //delay(2000); // delay to let things settle
  AUX_data_ready();
  // sends five bytes
  //Wire.write(x);              // sends one byte
  // check if there is a result on the MCU
  //delay(500);
  digitalWrite(DATA_READY, LOW); // signal that data has been sent

}

uint16_t convertRGBtoGrey(byte high, byte low)
{
  double R, G, B; // initialize variables
  uint16_t result; // initialize result
  R = (double)((high & 0xF8) >> 3); // grab higher 5 bits of high
  B = (double)(low & 0x1F); // grab the lower 5 bits of low
  G = (double)((high & 0x07) << 3 | (low & 0xE0)>>5); // grab the lower 3 bits of high and higher 3 bits of low  
  result = 2.7419 * R +  2.5 * 2.0238 * G + 2* 1.37096 * B; // calculate greyscale value
  //result = 0.2980 * R + 0.5870 * G + 0.1140 * B;
  
  /* serial writing for debugging
  Serial.print("RGB ");
  Serial.print(R);
  Serial.print(" ");
  Serial.print(G);
  Serial.print(" ");
  Serial.print(B);
  Serial.print(" ");
  Serial.print(result);
  Serial.println(" ");
  */
  return (uint16_t)result ;
}

void AUX_data_ready()
{
  //noInterrupts();
  //Serial.println("REQUEST RESULT");
  Wire.requestFrom(8, 1);    // request 1 byte from slave device #8
  while (Wire.available()) { // slave may send less than requested
    AUX_result = Wire.read(); // receive a byte as character
    Serial.print(AUX_result);         // print the character
  }

  //Serial.println();
  //Serial.println("REQUESTED RESULT"); // serially write that the result has ben recieved
  //interrupts();
}
