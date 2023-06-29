#include "mbed.h"
#include "Tds.h"

Tds tds(PB_0);

Ticker t;

void updateBuffer() {
    tds.updateBuffer();
}

int main() {
    t.attach(&updateBuffer, 0.1);

    while(1) {
        printf("TDS: %f\n", tds.calculateTds());
        wait(1);
    }

}