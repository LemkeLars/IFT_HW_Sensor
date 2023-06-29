#include "mbed.h"
#include "utils.h"
#include "Ds18b20.h"
#include "Tds.h"

//Setup
#define AVERAGE_INTERVAL 60                        // Average over 60 seconds
#define SAMPLE_INTERVAL 15*60                      // Sample every 15 minutes

Ticker oneMinuteTicker;
Ticker xMinuteTicker;


Ds18b20 ds18b20(PC_0);
Ticker tickerTemp;
float temp;
float tempSamples[AVERAGE_INTERVAL];
int tempSampleIndex = 0;

Tds tds(PB_0);                                      //TDS Sensor
Ticker tickerTDS;
float tdsValue;
float tdsSamples[AVERAGE_INTERVAL];
int tdsSampleIndex = 0;


void updateBuffer() {
    tds.updateBuffer();
}


float average(float value[AVERAGE_INTERVAL], int index) {
    float sum = 0;
    for (int i = 0; i < index; i++) {
        sum += value[i];
    }
    return sum / index;
}


float getTemp() {
    ds18b20.start();
    wait_ms(750);
    temp = ds18b20.readTemperature();
    printf("temperature: %.2f Grad Celsius\n", temp);
    return temp;
}


float getTDS() {
    tdsValue = tds.calculateTds();
    printf("TDS Value: %f ppm\n", tdsValue);
    return tdsValue;
}


void readSensor() { 
    // Reset index if necessary
    if ((tempSampleIndex >= AVERAGE_INTERVAL) | (tdsSampleIndex >= AVERAGE_INTERVAL)) {
        tempSampleIndex = 0;
        tdsSampleIndex = 0;
        oneMinuteTicker.detach();
        // Calculate average
        float tempAverage = average(tempSamples, AVERAGE_INTERVAL);
        float tdsAverage = average(tdsSamples, AVERAGE_INTERVAL);
        // Print average
        printf("Average temperature: %.2f Grad Celsius\n", tempAverage);
        printf("Average TDS Value: %f ppm\n", tdsAverage);
    } else {
        // Read sensor
        tempSamples[tempSampleIndex] = getTemp();
        tdsSamples[tdsSampleIndex] = getTDS();
        // Increment index
        tempSampleIndex++;
        tdsSampleIndex++;
    }
}


void triggerReading() {
    oneMinuteTicker.attach(&readSensor, 1.0f);       // Read sensor every second, to get a more accurate average
}


int main() {
    //Setup
    tickerTDS.attach(&updateBuffer, 0.1);
    xMinuteTicker.attach(&triggerReading, 600);    // Trigger reading every 10 minutes (time should not be shorter than one minute)
    oneMinuteTicker.attach(&readSensor, 1.0f);

    while(1) {
        wait(1);                                  // wait one second
        
    }
}
