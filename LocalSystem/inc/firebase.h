#ifndef FIREBASE_H
#define FIREBASE_H

int initFirebase(void);

void remFirebase(void);

int sendEntry(const char* PIgate_ID, const char* Plate);

int receivePlates(void);

int isToOpen(void);

#endif //FIREBASE_H