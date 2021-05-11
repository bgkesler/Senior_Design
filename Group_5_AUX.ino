// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define IM_ROWS 32
#define IM_COLS 24
#define TMP_ROWS 11
#define TMP_COLS 12
#define THRESHOLD_SSD 500000


const uint8_t stop_template[] PROGMEM = {252, 255, 252, 245, 94, 96, 95, 95, 95, 95, 96, 93, 248, 252, 252, 255, 255, 255, 245, 94, 95, 110, 109, 109, 109, 109, 110, 95, 95, 245, 255, 255, 252, 248, 94, 95, 110, 107, 106, 106, 106, 106, 107, 110, 96, 89, 244, 255, 243, 94, 95, 110, 107, 106, 106, 106, 106, 107, 109, 109, 102, 81, 80, 244, 94, 96, 108, 107, 106, 106, 106, 106, 106, 107, 107, 96, 90, 92, 81, 80, 96, 104, 124, 105, 111, 118, 118, 115, 103, 122, 98, 90, 104, 101, 88, 80, 95, 207, 215, 235, 158, 235, 241, 172, 187, 218, 217, 93, 243, 204, 190, 80, 95, 210, 196, 135, 103, 167, 185, 99, 244, 83, 190, 136, 228, 109, 236, 80, 95, 148, 158, 246, 104, 168, 188, 92, 236, 84, 190, 135, 238, 192, 132, 80, 95, 209, 213, 235, 104, 169, 180, 85, 170, 212, 216, 93, 233, 85, 92, 80, 96, 104, 118, 105, 107, 102, 95, 90, 87, 102, 89, 90, 97, 90, 95, 80, 93, 95, 108, 109, 96, 90, 90, 90, 90, 90, 90, 90, 90, 94, 81, 82, 247, 95, 95, 101, 90, 90, 90, 90, 90, 90, 90, 92, 94, 81, 80, 248, 252, 250, 88, 81, 94, 92, 90, 90, 90, 90, 92, 94, 81, 80, 248, 255, 252, 255, 248, 80, 81, 94, 92, 92, 92, 92, 94, 81, 80, 245, 255, 255, 252, 255, 255, 245, 80, 80, 80, 80, 80, 80, 80, 81, 248, 254, 252, 255};
const uint8_t merge_template[] PROGMEM = {253, 253, 253, 253, 253, 255, 251, 173, 173, 250, 255, 253, 253, 253, 253, 253, 253, 253, 253, 253, 255, 252, 174, 128, 125, 176, 251, 255, 253, 253, 253, 253, 253, 253, 253, 255, 252, 172, 130, 198, 191, 125, 174, 251, 255, 253, 253, 253, 253, 253, 255, 252, 172, 131, 177, 151, 200, 190, 125, 174, 251, 255, 253, 253, 253, 255, 252, 173, 130, 181, 54, 10, 136, 199, 190, 126, 175, 251, 255, 253, 255, 252, 173, 133, 196, 97, 21, 6, 46, 170, 198, 191, 130, 174, 251, 255, 251, 175, 133, 193, 194, 191, 93, 15, 172, 190, 193, 194, 191, 130, 176, 250, 171, 134, 194, 194, 193, 202, 95, 6, 163, 199, 193, 193, 194, 192, 131, 173, 165, 132, 195, 194, 193, 201, 96, 0, 57, 191, 197, 193, 194, 194, 129, 171, 249, 167, 132, 194, 194, 201, 96, 2, 0, 66, 175, 195, 192, 129, 173, 250, 255, 250, 164, 132, 194, 202, 94, 17, 99, 0, 75, 195, 129, 171, 251, 255, 253, 255, 248, 160, 132, 202, 90, 6, 181, 148, 169, 131, 171, 251, 255, 253, 253, 253, 255, 247, 160, 137, 135, 87, 185, 199, 131, 171, 251, 255, 253, 253, 253, 253, 253, 255, 247, 159, 135, 202, 194, 129, 171, 251, 255, 253, 253, 253, 253, 253, 253, 253, 255, 247, 161, 131, 130, 173, 251, 255, 253, 253, 253, 253, 253, 253, 253, 253, 253, 255, 245, 159, 170, 250, 255, 253, 253, 253, 253, 253};
const uint8_t old_cross_template[] PROGMEM = {255, 255, 255, 255, 255, 255, 232, 123, 123, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 234, 135, 183, 183, 135, 234, 255, 255, 255, 255, 255, 255, 255, 255, 255, 234, 133, 185, 201, 199, 181, 133, 234, 255, 255, 255, 255, 255, 255, 255, 234, 133, 181, 163, 180, 198, 199, 181, 133, 234, 255, 255, 255, 255, 255, 234, 133, 182, 195, 27, 99, 211, 199, 199, 181, 133, 234, 255, 255, 255, 234, 133, 181, 199, 203, 109, 19, 91, 168, 198, 199, 181, 133, 234, 255, 232, 135, 181, 199, 197, 195, 55, 0, 10, 70, 202, 196, 199, 181, 135, 232, 124, 183, 199, 197, 193, 82, 92, 37, 24, 112, 198, 196, 196, 199, 183, 124, 124, 183, 199, 196, 195, 188, 179, 13, 3, 171, 201, 197, 196, 199, 183, 124, 232, 135, 181, 198, 187, 195, 95, 24, 19, 116, 194, 186, 198, 181, 135, 232, 255, 234, 134, 172, 143, 141, 34, 85, 79, 27, 138, 142, 170, 134, 234, 255, 255, 255, 234, 134, 185, 202, 57, 158, 185, 66, 193, 186, 134, 234, 255, 255, 255, 255, 255, 234, 135, 157, 126, 132, 135, 134, 155, 136, 234, 255, 255, 255, 255, 255, 255, 255, 234, 133, 181, 198, 198, 180, 132, 234, 255, 255, 255, 255, 255, 255, 255, 255, 255, 234, 135, 183, 183, 135, 234, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 232, 123, 123, 232, 255, 255, 255, 255, 255, 255};

const uint8_t cross_template[] PROGMEM = {159, 161, 165, 169, 163, 144, 165, 144, 159, 160, 156, 153,
169, 172, 171, 153, 149, 171, 156, 127, 151, 155, 167, 166,
182, 185, 160, 156, 177, 180, 142, 98, 179, 164, 161, 172,
193, 158, 166, 182, 165, 91, 45, 121, 189, 188, 175, 163,
159, 173, 187, 190, 106, 78, 41, 66, 184, 196, 194, 182,
180, 189, 192, 195, 128, 67, 41, 135, 109, 169, 200, 195,
188, 195, 197, 201, 189, 64, 39, 177, 204, 200, 203, 202,
155, 195, 202, 202, 194, 48, 75, 87, 205, 206, 206, 203,
214, 161, 201, 206, 165, 68, 197, 69, 188, 209, 212, 185,
201, 173, 157, 206, 125, 135, 213, 121, 140, 210, 187, 160,
57, 59, 105, 158, 108, 194, 215, 182, 124, 184, 121, 43};

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
}

void loop() {
  if (full)
  {
    full = false;
    printRXBUF();
    uint8_t temp[TMP_ROWS][TMP_COLS];
    load_template(&temp[0][0], 2);
    long int ssd_result = ssd(rx_buf, temp);
    Serial.println(ssd_result);
    if (ssd_result < THRESHOLD_SSD) {
      Serial.println("Pedestrian Crossing Sign");
      result = true;
    }
    else {
      Serial.println("NOT Pedestrian Crossing Sign");
      result = false;
    }
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
     memcpy_P(arr, merge_template, TMP_ROWS * TMP_COLS);
  }
  else {
     memcpy_P(arr, cross_template, TMP_ROWS * TMP_COLS);
  }
}

long int ssd(uint8_t image[IM_ROWS][IM_COLS], uint8_t temp[TMP_ROWS][TMP_COLS]) {
    Serial.println("Start SSD");
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
                Serial.println("New Min");
                Serial.print("SSD( ");
                Serial.print(i);
                Serial.print(", ");
                Serial.print(j);
                Serial.print(") = ");
                Serial.println(ssd);
                ssd_min = ssd;
            }
        }
    }
    Serial.println("Finish SSD");
    //Returns lowest ssd from any overlap
    return ssd_min;
}
