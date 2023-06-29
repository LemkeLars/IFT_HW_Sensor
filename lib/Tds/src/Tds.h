#ifndef Tds_h
#define Tds_h
#include "mbed.h"


class Tds
{
    public:
        Tds(PinName pin);
        void updateBuffer();
        float calculateTds();
    private:
        AnalogIn pin;
        int getMedianNum(int* bArray, int iFilterLen);

};
#endif