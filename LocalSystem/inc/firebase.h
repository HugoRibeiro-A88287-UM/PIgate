#ifndef FIREBASE_H
#define FIREBASE_H


int sendEntry(const char* PIgate_ID, const char* Plate);

int receivePlates(void);

int isToOpen(const char* PIgate_ID);

int validPIgate(const char* PIgate_ID);

#endif //FIREBASE_H