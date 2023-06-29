# TDS Sensor Library for MBED

The TDS Sensor Library for MBED is designed to facilitate the reading and processing of data from a Total Dissolved Solids (TDS) sensor. TDS sensors measure the conductivity of a solution to estimate the total concentration of dissolved solids, making them useful in a variety of applications, including water quality testing.

This library implements the functions needed to read data from the sensor, process the readings to reduce noise, and calculate the TDS value in parts per million (ppm).

## Key Features

1. **Data Reading:** Regularly reads analog data from a TDS sensor connected to an MBED device.

2. **Data Buffering:** Stores the most recent sensor readings in a buffer for processing.

3. **Noise Reduction:** Uses a median filter to reduce noise in the sensor readings.

4. **TDS Calculation:** Calculates the TDS value in ppm using a provided formula that accounts for temperature compensation.

## Usage

To use this library, instantiate the TDS sensor object with the appropriate MBED analog input pin, and then call the provided functions within your main loop to read the sensor data and calculate the TDS value.

Please refer to the included example code for more details on how to use this library.

