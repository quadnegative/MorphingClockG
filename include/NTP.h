#ifndef NTP_H
#define NTP_H

#include <NTPClient.h>
extern NTPClient timeClient;

bool validateNTPServer(String NTPServer);
int setupTimeOffset(bool verbose);
void setupNTP(bool verbose);

#endif