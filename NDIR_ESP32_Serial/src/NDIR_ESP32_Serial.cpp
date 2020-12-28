/*
Description:
This is a example code for Sandbox Electronics NDIR CO2 sensor module with ESP32-DevKit .
You can get one of those products on
http://sandboxelectronics.com

Version:
V1.2

Release Date:
2019-01-10

Author:
Tiequan Shao          support@sandboxelectronics.com
Christoph Schultz      ic-schultz@gmx.de  derived from NDIR_SoftwareSerial.

Lisence:
CC BY-NC-SA 3.0

Please keep the above information when you use this code in your project.
*/

#include <HardwareSerial.h>
#include "NDIR_ESP32_Serial.h"

class HardwareSerial;
uint8_t NDIR_ESP32Serial::cmd_measure[9]                = {0xFF,0x01,0x9C,0x00,0x00,0x00,0x00,0x00,0x63};
uint8_t NDIR_ESP32Serial::cmd_calibrateZero[9]          = {0xFF,0x01,0x87,0x00,0x00,0x00,0x00,0x00,0x78};
uint8_t NDIR_ESP32Serial::cmd_enableAutoCalibration[9]  = {0xFF,0x01,0x79,0xA0,0x00,0x00,0x00,0x00,0xE6};
uint8_t NDIR_ESP32Serial::cmd_disableAutoCalibration[9] = {0xFF,0x01,0x79,0x00,0x00,0x00,0x00,0x00,0x86};

NDIR_ESP32Serial::NDIR_ESP32Serial() : serial2;
{  
}

bool NDIR_ESP32Serial::begin()
{
    serial2.begin(9600);
    if(mesure())
    {
        return true;
    }
    else
    {
      return false;  /* code */
    }
    

}

bool NDIR_ESP32Serial::measure()
{
    uint8_t i;
    uint8_t buf[9];
    uint32_t start = millis();

    serial2.flush();

    for (i=0; i<9; i++) {
        serial2.write(cmd_measure[i]);
    }

    for (i=0; i<9;) {
        if (serial2.available()) {
            buf[i++] = serial2.read();
        }

        if (millis() - start > RECEIVE_TIMEOUT) {
            return false;
        }
    }

    if (parse(buf)) {
        return true;
    }

    return false;
}

void NDIR_ESP32Serial::calibrateZero()
{
    uint8_t i;

    for (i=0; i<9; i++) {
        serial2.write(cmd_calibrateZero[i]);
    }
}


void NDIR_ESP32Serial::enableAutoCalibration()
{
    uint8_t i;

    for (i=0; i<9; i++) {
        serial2.write(cmd_enableAutoCalibration[i]);
    }
}


void NDIR_ESP32Serial::disableAutoCalibration()
{
    uint8_t i;

    for (i=0; i<9; i++) {
        serial2.write(cmd_disableAutoCalibration[i]);
    }
}


bool NDIR_ESP32Serial::parse(uint8_t *pbuf)
{
    uint8_t i;
    uint8_t checksum = 0;

    for (i=0; i<9; i++) {
        checksum += pbuf[i];
    }

    if (pbuf[0] == 0xFF && pbuf[1] == 0x9C && checksum == 0xFF) {
        ppm = (uint32_t)pbuf[2] << 24 | (uint32_t)pbuf[3] << 16 | (uint32_t)pbuf[4] << 8 | pbuf[5];
        return true;
    } else {
        return false;
    }
}
