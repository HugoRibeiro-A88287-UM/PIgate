#include <errno.h>
#include <sys/syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../inc/utilits.h"

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
