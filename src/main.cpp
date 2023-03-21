#include "mbed.h"
#include "utils.h"
#include "Ds18b20.h"
//Setup
Ds18b20 ds18b20(PC_0);
Ticker tickerTemp;
float temp;


void getTemp() {
    ds18b20.start();
    wait_ms(750);
    temp = ds18b20.readTemperature();
}

int main() {
    //Setup
    tickerTemp.attach(&getTemp, 1);     //Intervall sollte nicht zu klein sein, da sonst die Messung nicht fertig ist

    
    while(1) {
        // Temperatur auf der Konsole ausgeben
        printf("Temperatur: %.2f Grad Celsius\n", temp);
        
        // Eine Sekunde warten
        wait(1);
    }
}
