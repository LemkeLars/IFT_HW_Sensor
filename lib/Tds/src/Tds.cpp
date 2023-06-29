#include "mbed.h"
#include "Tds.h"

#define SCOUNT 30
#define VREF 5.0

Tds::Tds(PinName pinname) : pin(pinname) {
}


int analogBuffer[SCOUNT] = {0};
int analogBufferTemp[SCOUNT] = {0};
int analogBufferIndex = 0;
int copyIndex = 0;
float averageVoltage = 0;
float TdsValue = 0;
float temperature = 25;

// Function to update the analog buffer with readings
void Tds::updateBuffer() {
    analogBuffer[analogBufferIndex] = pin.read_u16(); 
    analogBufferIndex++;
    if(analogBufferIndex == SCOUNT)
        analogBufferIndex = 0;
}

// Function to calculate median
int Tds::getMedianNum(int* bArray, int iFilterLen) {
    int bTab[iFilterLen];
    memcpy(bTab, bArray, iFilterLen*sizeof(int));
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++) {
        for (i = 0; i < iFilterLen - j - 1; i++) {
            if (bTab[i] > bTab[i + 1]) {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
}

// Function to calculate Tds
float Tds::calculateTds() {
    memcpy(analogBufferTemp, analogBuffer, SCOUNT*sizeof(int));
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (VREF/ 65536.0); // convert to voltage

    float compensationCoefficient=1.0+0.02*(temperature-25.0);
    float compensationVolatge=averageVoltage/compensationCoefficient;

    TdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;

    return TdsValue;
}

