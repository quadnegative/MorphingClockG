#ifndef PARAMS_H
#define PARAMS_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern JsonDocument DefaultConfig();

extern int8_t R1_PIN;
extern int8_t G1_PIN;
extern int8_t B1_PIN;
extern int8_t R2_PIN;
extern int8_t G2_PIN;
extern int8_t B2_PIN;

extern int8_t A_PIN;
extern int8_t B_PIN;
extern int8_t C_PIN;
extern int8_t D_PIN;
extern int8_t E_PIN;
extern int8_t LAT_PIN;
extern int8_t OE_PIN;
extern int8_t CLK_PIN;

#endif