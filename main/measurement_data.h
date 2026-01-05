#ifndef RUUVI_GW_MEASUREMENT_DATA_H
#define RUUVI_GW_MEASUREMENT_DATA_H

typedef struct measurement_data_t {
  short version;
  char name[25];
  char bda[18];

  // common values in data format 5 and 6
  float temperature;
  float humidity;
  uint32_t pressure;
  uint16_t sequence;

  // values specific to data format 5
  float acceleration_x;
  float acceleration_y;
  float acceleration_z;
  float battery;
  uint8_t txpower;
  uint8_t moves;

  // values specific to data format 6
  uint16_t pm25;
  uint16_t co2;
  uint16_t voc;
  uint16_t nox;
  float luminosity;

  short flag_calibration;
} measurement_data_t;

int measurement_parse(unsigned char *data, measurement_data_t *measurement);

#endif
