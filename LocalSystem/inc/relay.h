#ifndef RELAY_H
#define RELAY_H

typedef enum  {relayON = 0 , relayOFF } relayStatus_t;

int initRelay(void);

void remRelay(void);

int openGate(void);

int getGateStatus(void);


#endif //RELAY_H