// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#define RESULT_READY 7

#define IM_ROWS 32
#define IM_COLS 24
#define TMP_ROWS 15
#define TMP_COLS 11
#define THRESHOLD_SSD 40000
#define ISON 8


const uint8_t stop_template[] PROGMEM = {252, 255, 252, 245, 94, 96, 95, 95, 95, 95, 96, 93, 248, 252, 252, 255, 255, 255, 245, 94, 95, 110, 109, 109, 109, 109, 110, 95, 95, 245, 255, 255, 252, 248, 94, 95, 110, 107, 106, 106, 106, 106, 107, 110, 96, 89, 244, 255, 243, 94, 95, 110, 107, 106, 106, 106, 106, 107, 109, 109, 102, 81, 80, 244, 94, 96, 108, 107, 106, 106, 106, 106, 106, 107, 107, 96, 90, 92, 81, 80, 96, 104, 124, 105, 111, 118, 118, 115, 103, 122, 98, 90, 104, 101, 88, 80, 95, 207, 215, 235, 158, 235, 241, 172, 187, 218, 217, 93, 243, 204, 190, 80, 95, 210, 196, 135, 103, 167, 185, 99, 244, 83, 190, 136, 228, 109, 236, 80, 95, 148, 158, 246, 104, 168, 188, 92, 236, 84, 190, 135, 238, 192, 132, 80, 95, 209, 213, 235, 104, 169, 180, 85, 170, 212, 216, 93, 233, 85, 92, 80, 96, 104, 118, 105, 107, 102, 95, 90, 87, 102, 89, 90, 97, 90, 95, 80, 93, 95, 108, 109, 96, 90, 90, 90, 90, 90, 90, 90, 90, 94, 81, 82, 247, 95, 95, 101, 90, 90, 90, 90, 90, 90, 90, 92, 94, 81, 80, 248, 252, 250, 88, 81, 94, 92, 90, 90, 90, 90, 92, 94, 81, 80, 248, 255, 252, 255, 248, 80, 81, 94, 92, 92, 92, 92, 94, 81, 80, 245, 255, 255, 252, 255, 255, 245, 80, 80, 80, 80, 80, 80, 80, 81, 248, 254, 252, 255};
const uint8_t merge_template[] PROGMEM = {253, 253, 253, 253, 253, 255, 251, 173, 173, 250, 255, 253, 253, 253, 253, 253, 253, 253, 253, 253, 255, 252, 174, 128, 125, 176, 251, 255, 253, 253, 253, 253, 253, 253, 253, 255, 252, 172, 130, 198, 191, 125, 174, 251, 255, 253, 253, 253, 253, 253, 255, 252, 172, 131, 177, 151, 200, 190, 125, 174, 251, 255, 253, 253, 253, 255, 252, 173, 130, 181, 54, 10, 136, 199, 190, 126, 175, 251, 255, 253, 255, 252, 173, 133, 196, 97, 21, 6, 46, 170, 198, 191, 130, 174, 251, 255, 251, 175, 133, 193, 194, 191, 93, 15, 172, 190, 193, 194, 191, 130, 176, 250, 171, 134, 194, 194, 193, 202, 95, 6, 163, 199, 193, 193, 194, 192, 131, 173, 165, 132, 195, 194, 193, 201, 96, 0, 57, 191, 197, 193, 194, 194, 129, 171, 249, 167, 132, 194, 194, 201, 96, 2, 0, 66, 175, 195, 192, 129, 173, 250, 255, 250, 164, 132, 194, 202, 94, 17, 99, 0, 75, 195, 129, 171, 251, 255, 253, 255, 248, 160, 132, 202, 90, 6, 181, 148, 169, 131, 171, 251, 255, 253, 253, 253, 255, 247, 160, 137, 135, 87, 185, 199, 131, 171, 251, 255, 253, 253, 253, 253, 253, 255, 247, 159, 135, 202, 194, 129, 171, 251, 255, 253, 253, 253, 253, 253, 253, 253, 255, 247, 161, 131, 130, 173, 251, 255, 253, 253, 253, 253, 253, 253, 253, 253, 253, 255, 245, 159, 170, 250, 255, 253, 253, 253, 253, 253};
const uint8_t old_cross_template[] PROGMEM = {255, 255, 255, 255, 255, 255, 232, 123, 123, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 234, 135, 183, 183, 135, 234, 255, 255, 255, 255, 255, 255, 255, 255, 255, 234, 133, 185, 201, 199, 181, 133, 234, 255, 255, 255, 255, 255, 255, 255, 234, 133, 181, 163, 180, 198, 199, 181, 133, 234, 255, 255, 255, 255, 255, 234, 133, 182, 195, 27, 99, 211, 199, 199, 181, 133, 234, 255, 255, 255, 234, 133, 181, 199, 203, 109, 19, 91, 168, 198, 199, 181, 133, 234, 255, 232, 135, 181, 199, 197, 195, 55, 0, 10, 70, 202, 196, 199, 181, 135, 232, 124, 183, 199, 197, 193, 82, 92, 37, 24, 112, 198, 196, 196, 199, 183, 124, 124, 183, 199, 196, 195, 188, 179, 13, 3, 171, 201, 197, 196, 199, 183, 124, 232, 135, 181, 198, 187, 195, 95, 24, 19, 116, 194, 186, 198, 181, 135, 232, 255, 234, 134, 172, 143, 141, 34, 85, 79, 27, 138, 142, 170, 134, 234, 255, 255, 255, 234, 134, 185, 202, 57, 158, 185, 66, 193, 186, 134, 234, 255, 255, 255, 255, 255, 234, 135, 157, 126, 132, 135, 134, 155, 136, 234, 255, 255, 255, 255, 255, 255, 255, 234, 133, 181, 198, 198, 180, 132, 234, 255, 255, 255, 255, 255, 255, 255, 255, 255, 234, 135, 183, 183, 135, 234, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 232, 123, 123, 232, 255, 255, 255, 255, 255, 255};

const uint8_t cross_template[] PROGMEM = {36,  16,  20,  26,  27,  25,  19,  23,  20,  13,  32,
15,  21,  27,  28,  27,  19,  2, 13,  26,  21,  13,
22,  28,  28,  27,  27,  19,  10,  21,  26,  25,  21,
30,  29,  29, 21,  7, 1, 4, 25,  25,  25,  25,
31,  31,  21,  3, 1, 1, 1, 19,  26,  25,  27,
33,  31,  22,  13,  1, 2, 2, 6, 23,  27,  26,
35,  34,  22,  11,  1, 1, 18,  16,  6, 21,  26,
36,  36,  30,  19,  1, 0, 21,  32,  30,  28,  27,
40,  37,  34,  22,  1, 1, 8, 32,  33,  31,  28,
35,  36,  35,  13,  2, 18,  2, 22,  34,  32,  30,
38,  35,  35,  4, 14,  36,  5, 13,  34,  33,  31,
37,  38,  30,  1, 26,  34,  15,  6, 34,  33,  32,
21,  37,  25,  5, 35,  34,  25,  2, 31,  33,  17,
31,  20,  33,  30,  35,  34,  34,  30,  32,  18,  31,
53,  32,  19,  34,  34,  34,  35,  33,  18,  32,  52};

const uint8_t cross_template2[] PROGMEM = {58, 47,  35,  44,  49,  50,  50,  51,  51,  43,  51,
41,  39,  48,  50,  51,  51,  51,  52,  52,  54,  41,
43,  50,  50,  50,  52,  53,  44,  28,  53,  55,  52,
49,  51,  51, 53, 52,  47,  27,  14,  53,  56,  56,
51,  52,  52,  39,  12,  3, 2, 42,  57,  57,  58,
52,  53,  54,  24,  22,  1, 1, 33,  58,  57,  58,
53,  54,  55,  31,  8, 1, 28,  13,  48,  58,  59,
54,  54,  55,  50,  4, 8, 54,  48,  40,  58,  58,
54,  54,  55,  26,  4, 1, 45,  59,  59,  58,  59,
40,  42,  39,  7, 39,  19,  32, 57,  59,  58,  59,
54,  53,  23,  27,  46,  23,  22,  43,  45,  49,  53,
52,  53,  29,  53,  57,  34,  28,  57,  56,  52,  49,
40,  39,  41,  44,  49,  47,  46,  58,  58,  58,  60,
51,  56,  55,  53,  50,  47,  44,  42,  44,  45,  49,
42,  53,  56,  57,  56,  57,  56,  58,  57,  48,  48};

uint8_t rx_buf[IM_ROWS][IM_COLS] = {0,};
uint8_t row = 0, col = 0;
bool full = false;
bool result = false;
bool waitingResult = false;


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  Serial.println(F("Slave Ready"));
  pinMode(RESULT_READY, OUTPUT);
  pinMode(ISON, OUTPUT);
  digitalWrite(ISON,HIGH);
  digitalWrite(RESULT_READY, LOW);
  digitalWrite(RESULT_READY, LOW);
  
}

void loop() {
  if (full)
  {
    full = false;
    //printRXBUF();
    uint8_t temp[TMP_ROWS][TMP_COLS];
    
    load_template(&temp[0][0], 2);
    long int ssd_result = ssd(rx_buf, temp);
    
    load_template(&temp[0][0], 1);
    long int ssd_result2 = ssd(rx_buf, temp);

    Serial.println(ssd_result);
    Serial.println(ssd_result2);
    
    if(ssd_result2<ssd_result)
    {
      ssd_result = ssd_result2;
    }
    
    
    if (ssd_result < THRESHOLD_SSD) {
      Serial.println("Pedestrian Crossing Sign");
      result = true;      
    }
    else {
      Serial.println("NOT Pedestrian Crossing Sign");
      result = false;
      delay(10);
    }
    digitalWrite(RESULT_READY, HIGH);
    delay(10);
    digitalWrite(RESULT_READY, LOW);
    Serial.println(F("printing result"));    
  }
  delay(100);

  //Run this when recieving an image
  
}

void printRXBUF()
{
  Serial.println(F("RXBUFF FULLL!!!!"));
  for (int y = 0; y < IM_COLS; y++){   
    for(int x = 0; x < IM_ROWS; x++)
    {
      Serial.print(F(" "));
      delayMicroseconds(12);
      Serial.print(rx_buf[x][y],DEC);
      delayMicroseconds(12);
      Serial.print(F(" "));
      delayMicroseconds(12);
    }
      delayMicroseconds(12);
      Serial.println();
  }
  Serial.println(F("RXBUFF Empty"));
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  if (!waitingResult)
  {
    while (0 < Wire.available()) { // loop through all but the last
      byte c = Wire.read(); // receive byte as a character
      //Serial.print(c);         // print the character
      //Serial.print(F(" "));
      rx_buf[col++][row] = c;
      if (col >= IM_ROWS)
      {
        col = 0;
        row++;
      }
      if (row >= IM_COLS)
      {
        row = 0;
        full = true; 
        Serial.println("Recieved data"); 
      }
    
    }
    //int x = Wire.read();    // receive byte as an integer
    //Serial.println();         // print the integer
  }

}


// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  
  Wire.write(result); // respond with message of 6 bytes
  // as expected by master
}

void load_template(uint8_t *arr, uint8_t sel) {
  if (sel == 0) {
     memcpy_P(arr, stop_template, TMP_ROWS * TMP_COLS);
  }
  else if (sel == 1) {
     memcpy_P(arr, cross_template2, TMP_ROWS * TMP_COLS);
  }
  else {
     memcpy_P(arr, cross_template, TMP_ROWS * TMP_COLS);
  }
}

long int ssd(uint8_t image[IM_ROWS][IM_COLS], uint8_t temp[TMP_ROWS][TMP_COLS]) {
    //Serial.println("Start SSD");
    long int ssd_min = 0;
    for (uint8_t i = 0; i <= IM_ROWS - TMP_ROWS; i++) {
        for (uint8_t j = 0; j <= IM_ROWS - TMP_COLS; j++) {
            long int ssd = 0;
            for (uint8_t ii = 0; ii < TMP_ROWS; ii++) {
                for (uint8_t jj = 0; jj < TMP_COLS; jj++) {
                    long int diff = temp[ii][jj] - image[i+ii][j+jj];
                    ssd = ssd + (diff * diff);
                }
            }
            //Replaces SSD if new min
            if ((i == 0 && j ==0) || ssd < ssd_min) {
                //Print info for debugging
                /*Serial.println("New Min");
                Serial.print("SSD( ");
                Serial.print(i);
                Serial.print(", ");
                Serial.print(j);
                Serial.print(") = ");
                Serial.println(ssd);*/
                ssd_min = ssd;
            }
        }
    }
    //Serial.println("Finish SSD");
    //Returns lowest ssd from any overlap
    return ssd_min;
}
