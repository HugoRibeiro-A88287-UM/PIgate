#ifndef UTILITS_H
#define UTILITS_H

#include <pthread.h>

void setupThread(int priority, pthread_attr_t *pthread_attr, struct sched_param *pthread_param);

void checkFail(int status);

#endif //UTILITS_H