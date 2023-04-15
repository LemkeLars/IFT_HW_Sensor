#include "mbed.h"
#include "utils.h"
#include "Ds18b20.h"
//Setup
Ds18b20 ds18b20(PC_0);
Ticker tickerTemp;
float temp;

AnalogIn TDS(PB_0); //TDS Sensor
Ticker tickerTDS;
float tdsValue;


void getTemp() {
    ds18b20.start();
    wait_ms(750);
    temp = ds18b20.readTemperature();
    //printf("Temperatur: %.2f Grad Celsius\n", temp);
}

void getTDS() {
    float voltage = TDS.read() * 3.3;
    tdsValue = voltage / 1024 * 1000;
    //printf("TDS Value: %f ppm\n", tdsValue);
}

int main() {
    //Setup
    tickerTemp.attach(&getTemp, 1);     //Intervall sollte nicht zu klein sein, da sonst die Messung nicht fertig ist
    tickerTDS.attach(&getTDS, 1);     //Intervall sollte nicht zu klein sein, da sonst die Messung nicht fertig ist
    
    while(1) {
        // Temperatur auf der Konsole ausgeben
        printf("Temperatur: %.2f Grad Celsius\n", temp);
        // TDS auf der Konsole ausgeben
        printf("TDS Value: %f ppm\n", tdsValue);
        // Eine Sekunde warten
        wait(1);
    }
}
