#include <math.h>
#include <stdio.h>

#include "measurement_data.h"

double lum_decode(uint8_t code) {
    double delta = log(65535.0 + 1.0) / 254.0;
    return exp(code * delta) - 1.0;
}

int measurement_parse(unsigned char *data, measurement_data_t *measurement)
{
    /* Data format */
    switch (data[7]) {
        case 0x05:
            // https://docs.ruuvi.com/communication/bluetooth-advertisements/data-format-5-rawv2

            measurement->version = 0x05;
            sprintf(measurement->bda, "%02X:%02X:%02X:%02X:%02X:%02X",
                (uint8_t)data[25],
                (uint8_t)data[26],
                (uint8_t)data[27],
                (uint8_t)data[28],
                (uint8_t)data[29],
                (uint8_t)data[30]);

            measurement->temperature = (((uint8_t)data[8] << 8) |
                (uint8_t)data[9]) * 0.005;

            if(measurement->temperature > 163.836)
                measurement->temperature -= 327.68;

            measurement->humidity = (((uint8_t)data[10] << 8) |
                (uint8_t)data[11]) * 0.0025;
            measurement->pressure = (((uint8_t)data[12] << 8) |
                (uint8_t)data[13]) + 50000;
            measurement->acceleration_x = (((uint8_t)data[14] << 8) |
                (uint8_t)data[15]) / 1000.0;
            measurement->acceleration_y = (((uint8_t)data[16] << 8) |
                (uint8_t)data[17]) / 1000.0;
            measurement->acceleration_z = (((uint8_t)data[18] << 8) |
                (uint8_t)data[19]) / 1000.0;

            if(measurement->acceleration_x > 32.767)
                measurement->acceleration_x -= 65.536;
            if(measurement->acceleration_y > 32.767)
                measurement->acceleration_y -= 65.536;
            if(measurement->acceleration_z > 32.767)
                measurement->acceleration_z -= 65.536;

            measurement->battery = ((((uint8_t)data[20] << 3) |
                ((uint8_t)data[21] >> 5))  + 1600) / 1000.0;
            measurement->txpower = (((uint8_t)data[21] & 0x1f) * 2) - 40;
            measurement->moves = (uint8_t)data[22];
            measurement->sequence = (((uint8_t)data[23] << 8) |
                (uint8_t)data[24]);
        break;

        case 0x06:
            // https://docs.ruuvi.com/communication/bluetooth-advertisements/data-format-6

            measurement->version = 0x06;

            sprintf(measurement->bda, "%02X:%02X:%02X",
                (uint8_t)data[24],
                (uint8_t)data[25],
                (uint8_t)data[26]);

            measurement->temperature = (((uint8_t)data[8] << 8) |
                (uint8_t)data[9]) * 0.005;

            if(measurement->temperature > 163.836)
                measurement->temperature -= 327.68;

            measurement->humidity = (((uint8_t)data[10] << 8) |
                (uint8_t)data[11]) * 0.0025;
            measurement->pressure = ((((uint8_t)data[12] << 8) |
                (uint8_t)data[13]) + 50000);
            measurement->pm25 = (((uint8_t)data[14] << 8) |
                (uint8_t)data[15]);
            measurement->co2 = (((uint8_t)data[16] << 8) |
                (uint8_t)data[17]);
            measurement->voc = ((uint8_t)data[18] << 1) + (data[23] & 0b01000000);
            measurement->nox = ((uint8_t)data[19] << 1) + (data[23] & 0b10000000);
            measurement->luminosity = lum_decode(data[20]);

            measurement->sequence = (uint8_t)data[22];

            measurement->flag_calibration = (data[23] & 0b00000001);

        break;
    }

    if (measurement->version == 0x00){
        return -1;
    }

    return 0;
}
