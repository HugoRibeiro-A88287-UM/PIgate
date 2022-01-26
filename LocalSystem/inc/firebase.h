#ifndef FIREBASE_H
#define FIREBASE_H

#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/include/python3.9/Python.h"

int initFirebase(void);

void remFirebase(void);

int sendEntry(const char* PIgate_ID, const char* Plate);

int receivePlates(void);

int isToOpen(void);

#endif //FIREBASE_H