#include "mbed.h"
#include "utils.h"
#include "Ds18b20.h"
#include "Tds.h"

//Setup
#define AVERAGE_INTERVAL 60                        // Average over 60 seconds
#define SAMPLE_INTERVAL 15*60                      // Sample every 15 minutes
#define SensorAnount 4

Ticker oneMinuteTicker;
Ticker xMinuteTicker;
Serial bluetooth(PA_9, PA_10);
Serial pc(USBTX, USBRX);


Ds18b20 ds18b20(PC_0);                              //Temperature Sensor      
float temp;
float tempSamples[AVERAGE_INTERVAL];
int tempSampleIndex = 0;

Tds tds(PB_0);                                      //TDS Sensor
float tdsValue;
float tdsSamples[AVERAGE_INTERVAL];
int tdsSampleIndex = 0;

Ticker ticker;
char bluetoothData;
int tickerCount = 0;

AnalogIn ph(PA_1);                                  //pH Sensor              
float phValue;
float phSamples[AVERAGE_INTERVAL];
int phSampleIndex = 0; 

/* DigitalOut trig(PA_13);                             //ultrasonic Sensor    
InterruptIn echo(PA_14);                               //Breaks everything         
Timer echoTimer; */
float abstand = 0;
float levelSamples[AVERAGE_INTERVAL];
int levelSampleIndex = 0;


void bluetoothReceive() {
    // read data from bluetooth
    if(bluetooth.readable()) {
        bluetoothData = bluetooth.getc();
        printf("Received: %c\n\r", bluetoothData);
    }
}

void bluetoothSend(float data[]) {
    for (int i = 0; i < SensorAnount; i++) {
      int byte = round(data[i]);
      bluetooth.putc((char)byte);
      
      // Print sent integer
      pc.printf("Sent Integer: %i\r\n", byte);
      
    }
    
}

float average(float value[AVERAGE_INTERVAL], int index) {
    float sum = 0;
    for (int i = 0; i < index; i++) {
        sum += value[i];
    }
    return sum / index;
}

float getLevel() {
    printf("Abstand: %f cm\n\r", abstand);
    return abstand;
}

float getPH() {
    float phValue = ph.read()*14;
    printf("pH Value: %.2f\n\r", phValue);
    return phValue;
}

float getTemp() {
    ds18b20.start();
    wait_ms(750);
    temp = ds18b20.readTemperature();
    printf("temperature: %.2f Grad Celsius\n\r", temp);
    return temp;
}

float getTDS() {
    tdsValue = tds.calculateTds();
    printf("TDS Value: %f ppm\n\r", tdsValue);
    return tdsValue;
}

void readSensor() { 
    // Reset index if necessary
    if ((tempSampleIndex >= AVERAGE_INTERVAL) | (tdsSampleIndex >= AVERAGE_INTERVAL) | (phSampleIndex >= AVERAGE_INTERVAL) | (levelSampleIndex >= AVERAGE_INTERVAL)) {
        tempSampleIndex = 0;
        tdsSampleIndex = 0;
        phSampleIndex = 0;
        levelSampleIndex = 0;
        // Detach ticker
        oneMinuteTicker.detach();
        // Calculate average
        float tempAverage = average(tempSamples, AVERAGE_INTERVAL);
        float tdsAverage = average(tdsSamples, AVERAGE_INTERVAL);
        float phAverage = average(phSamples, AVERAGE_INTERVAL);
        float levelAverage = average(levelSamples, AVERAGE_INTERVAL);
        // Print average
        printf("Average temperature: %.2f Grad Celsius\n\r", tempAverage);
        printf("Average TDS Value: %f ppm\n\r", tdsAverage);
        printf("Average pH Value: %f\n\r", phAverage);
        printf("Average level: %f cm\n\r", levelAverage);
        // Send average to bluetooth
        float data[SensorAnount] = {tempAverage, tdsAverage, phAverage, levelAverage};
        // print the data to the serial port
        for (int i = 0; i < SensorAnount; i++) {
            pc.printf("Data: %f\r\n", data[i]);
        }
        bluetoothSend(data);
    } else {
        // Read sensor
        tempSamples[tempSampleIndex] = getTemp();
        tdsSamples[tdsSampleIndex] = getTDS();
        phSamples[phSampleIndex] = getPH();
        levelSamples[levelSampleIndex] = getLevel();
        // Increment index
        tempSampleIndex++;
        tdsSampleIndex++;
        phSampleIndex++;
        levelSampleIndex++;
    }
}

void triggerReading() {
    oneMinuteTicker.attach(&readSensor, 1.0f);       // Read sensor every second, to get a more accurate average
}

void tickerFunction() {
    tds.updateBuffer();
    //bluetoothReceive();
}

int main() {
    //Setup
    pc.printf("Start\n");
    //bluetooth.attach(&bluetoothReceive);  // Attach interrupt to receive data from bluetooth    //not working
    
    ticker.attach(&tickerFunction, 0.1);
    xMinuteTicker.attach(&triggerReading, 600);    // Trigger reading every 10 minutes (time should not be shorter than one minute)
    oneMinuteTicker.attach(&readSensor, 1.0f);     // start with reading if commented out the first reading will be after 10 minutes

    while(1) {
        // wait one second
        wait(1);
    }
}
