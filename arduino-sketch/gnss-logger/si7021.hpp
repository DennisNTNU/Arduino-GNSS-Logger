#ifndef SI7021_H
#define SI7021_H


#define SI7021_ADDRESS 0x40

#define CMD_MEASURE_HUMIDITY 0xF5
#define CMD_READ_TEMP_CMD 0xE0


int readDataSi7021(float* buf);

#endif
