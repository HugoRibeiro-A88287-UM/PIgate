/**
 * @file utilits.h
 * @author PIgate
 * @brief Usefull functions for all the modules
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef UTILITS_H
#define UTILITS_H

#include <pthread.h>

/**
 * @brief Global Defenitions
 * 
 */
#define MAXPLATESLEN 512
#define MAXPLATELENDIGITS 4
#define PLATESSIZE 8
#define PIGATELEN 8
#define FIFOLEN 4
#define READWRITE_PERMISSION 0666
#define SHM_PIGATEID_NAME "shm_PIgateID"

/**
 * @brief PIPE array's defenition
 * 
 */
extern int entriesDBPIPE[2];
extern int recPlatePIPE[2];


/**
 * @brief Setup a given thread, so it can be successfully created
 * 
 * @param priority Thread Priority
 * @param pthread_attr Thread Attribute
 * @param pthread_param Thread Paramenter
 */
void setupThread(int priority, pthread_attr_t *pthread_attr, struct sched_param *pthread_param);


/**
 * @brief Verifies if a thread was successfully created
 * 
 * @param status Pthread_create return
 */
void checkFail(int status);


/**
 * @brief For a given buffer, this funtions removes all the present hiffens (-)
 *  Before: AA-00-00  
 *  After: AA0000
 * 
 * @param buffer 
 * @param len buffer's length
 * 
 */
void removeHiffen(char* buffer, int len);


/**
 * @brief Following the Portuguese plates, this functions puts an hiffen (-) between
 * a par of numbers or letters
 * 
 * Before: AA0000
 * After: AA-00-00
 * 
 * @param buffer Must be big enough
 * @param len buffer's length
 * 
 */
void insertPlateHiffen(char* buffer, int len);



#endif //UTILITS_H