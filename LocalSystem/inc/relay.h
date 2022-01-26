#ifndef RELAY_H
#define RELAY_H

typedef enum  {relayON = 0 , relayOFF } relayStatus_t;

int initRelay();

void remRelay();

int openGate();

int getGateStatus();


#endif //RELAY_H