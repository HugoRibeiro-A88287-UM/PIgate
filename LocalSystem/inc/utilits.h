#ifndef UTILITS_H
#define UTILITS_H

#include <pthread.h>

#define PLATESLEN 512
#define PLATESSIZE 8

int entriesDBPIPE[2];
int recPlatePIPE[2];

void setupThread(int priority, pthread_attr_t *pthread_attr, struct sched_param *pthread_param);

void checkFail(int status);

void removeHiffen(char* buffer, int len);

#endif //UTILITS_H