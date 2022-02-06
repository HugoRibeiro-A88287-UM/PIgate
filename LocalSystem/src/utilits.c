/**
 * @file utilits.c
 * @author PIgate
 * @brief Utilitis Module Implementation
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <errno.h>
#include <sys/syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../inc/utilits.h"

int entriesDBPIPE[2];
int recPlatePIPE[2];

void setupThread(int priority, pthread_attr_t *pthread_attr, struct sched_param *pthread_param)
{
	int minPrio, maxPrio;

	minPrio = sched_get_priority_min (SCHED_RR);
	maxPrio = sched_get_priority_max (SCHED_RR);

    if(priority < minPrio || priority > maxPrio)
    {
        syslog(LOG_ERR, "Thread priorities not valid.\n");
        exit(1);
    }

    pthread_param->sched_priority = priority;

    pthread_attr_setschedparam(&pthread_attr, &pthread_param);
}

void checkFail(int status)
{
	if(status)
	{     
        syslog(LOG_ERR, "pthread_create() got error %d\n",status);
        exit(1);    		
  	}
}

void removeHiffen(char* buffer, int len)
{

    char auxBuffer[len];
    char * token = strtok(buffer, "-");
    
    //Clear Buffer
    strcpy(auxBuffer,"");

    // loop through the string to extract all other tokens
   while( token != NULL ) 
   {
        strcat(auxBuffer,token);
        token = strtok(NULL, "-");
   }

    //Hiffens removed
    strcpy(buffer,auxBuffer);

}

void insertPlateHiffen(char* buffer, int len)
{
    const int plateCharacterNumber = 6;
    char auxBuffer[len];
    char str[2] = "\0";

    //Clear Buffer
    strcpy(auxBuffer,"");

    //Plate format with Hiffen

    for(int i = 0 ; i < plateCharacterNumber ; i ++ )
    {
        str[0] = buffer[i];
        strcat(auxBuffer,str);
        if((i+1) % 2 == 0 && i < (plateCharacterNumber-1) )
            strcat(auxBuffer,"-");

    }

    //Copy the new buffer
    strcpy(buffer,auxBuffer);
}
