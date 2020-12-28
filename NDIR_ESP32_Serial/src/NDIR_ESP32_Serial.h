#ifndef NDIR_ESP32_SERIAL_H
#define NDIR_ESP32_SERIAL_H

#include <ESP32/HardwareSerial.h>

class NDIR_ESP32Serial
{
    public:
        NDIR_ESP32Serial();
        uint32_t ppm;

        bool  begin();
        bool  measure();
        void     calibrateZero();
        void     enableAutoCalibration();
        void     disableAutoCalibration();
    private:
    HardwareSerial serial2;
       bool  parse(uint8_t *pbuf);

        static uint8_t cmd_measure[9];
        static uint8_t cmd_calibrateZero[9];
        static uint8_t cmd_enableAutoCalibration[9];
        static uint8_t cmd_disableAutoCalibration[9];
};
#endif

