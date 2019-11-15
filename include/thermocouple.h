#pragma once

typedef struct {
    float temp_internal;
    float temp_external;
} thermocouple_meas_t;

thermocouple_meas_t thermocouple_get();
