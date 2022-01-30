#ifndef UTILITS_H
#define UTILITS_H

#include <pthread.h>

#define MAXPLATESLEN 512
#define PLATESSIZE 8
#define PIGATELEN 8
#define PIGATE_ID "1"

int entriesDBPIPE[2];
int recPlatePIPE[2];


void setupThread(int priority, pthread_attr_t *pthread_attr, struct sched_param *pthread_param);

void checkFail(int status);

void removeHiffen(char* buffer, int len);

void insertHiffen(char* buffer, int len);

void definePIgate_ID(const char* PIgate_ID);

#endif //UTILITS_H