#ifndef MAIN_H
#define MAIN_H

#include <WiFiUdp.h>

extern WiFiUDP ntpUDP;
extern bool clockStartingUp;

bool connect_wifi(String n_ssid, String n_pass);

#endif