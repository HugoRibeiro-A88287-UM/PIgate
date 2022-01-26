#ifndef LEDRGB_H
#define LEDRGB_H

typedef enum {allow = 0 , denied , warning, idle}status_t;

int initLedRGB();

void remLedRGB();

int ledRGBStatus(status_t status);

int getLedRGBStatus();


#endif //LEDRGB_H