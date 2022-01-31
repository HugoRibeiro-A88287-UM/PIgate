/**
 * @file firebase.c
 * @author PIgate
 * @brief Firebase Module Implementation
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/syslog.h>
#include <stdbool.h>
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/include/python3.9/Python.h"

#include "../inc/utilits.h"
#include "../inc/firebase.h"

/**
 * @brief Get the pDict object
 * 
 * @return NULL if an error occurs ; pDict Pointer
 */
static PyObject* get_pDict(void)
{
    PyObject *pName, *pModule, *pDict ; 

    // Set PYTHONPATH TO working directory
    setenv("PYTHONPATH","/etc/PIgate/",1);

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString((char*)"firebase");

    // Load the module object
    pModule = PyImport_Import(pName);

    if(pModule == NULL)
    {
        syslog(LOG_ERR,"No firebase.py available!\n" );
        return NULL;
    }

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    return pDict;

}


int sendEntry(const char* PIgate_ID, const char* Plate)
{
    PyObject *pFunc,*presult, *pDict;

    pDict = get_pDict();
        
    if(pDict == NULL)
    {
        syslog(LOG_ERR," get_pDict not succeed \n" );
        return -EXIT_FAILURE;
    }  

    
    pFunc = PyDict_GetItemString(pDict, (char*)"sendEntry");

    if (PyCallable_Check(pFunc))
    {       
        syslog(LOG_INFO,"Received info: %s \n %s \n", PIgate_ID, Plate);

        presult = PyObject_CallFunction(pFunc,"ss",PIgate_ID,Plate);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"Entry not sent \n" );
            return -EXIT_FAILURE;
        }
   } 
   else 
   {
        syslog(LOG_ERR,"Python sendEntry function is broken!\n" );
        return -EXIT_FAILURE;
   }


    return EXIT_SUCCESS;
}

int receivePlates(void)
{
    PyObject *pFunc,*presult, *pDict;
    PyObject *presultString, *encodedString;
    int presult_length;
    static bool firstTime = true;
    char platesSize[PLATESSIZE+1];

    if(firstTime)
    {
        close(recPlatePIPE[0]); // Close reading end 
        firstTime = false;

    }
    
    pDict = get_pDict();
        
    if(pDict == NULL)
    {
        syslog(LOG_ERR," get_pDict not succeed \n" );
        return -EXIT_FAILURE;
    }   
    
    pFunc = PyDict_GetItemString(pDict, (char*)"getPlates");

    if (PyCallable_Check(pFunc))
    {

        presult = PyObject_CallFunction(pFunc,NULL);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"Plates not Received\n" );
            return -EXIT_FAILURE;
        }
    }
    else 
    {
        syslog(LOG_ERR,"Python getPlates function is broken!\n" );
        return -EXIT_FAILURE;
    }


    presult_length = PyObject_Length(presult);

    //Insert length in PIPE
    sprintf(platesSize, "%d", presult_length);   
    write(recPlatePIPE[1],platesSize,4);

    for(int i = 0; i < presult_length ; i++)
    {
        presultString = PyList_GetItem(presult,i);
        encodedString = PyUnicode_AsEncodedString(presultString, "UTF-8", "strict");

        if (encodedString) 
        {//returns NULL if an exception was raised

            strcpy(platesSize,PyBytes_AsString(encodedString)); //pointer refers to the internal buffer of encodedString

            syslog(LOG_INFO, "Plate: %s \n", platesSize );

            write(recPlatePIPE[1], platesSize, (PLATESSIZE+1) );

        }
        else
        {
            syslog(LOG_WARNING,"Encoded pointer is NULL\n" );
            return -EXIT_FAILURE;
        }
    }

    
    return EXIT_SUCCESS;
}

int isToOpen(const char* PIgate_ID)
{
    PyObject *pFunc,*presult, *pDict;
    int presult_length;
    int isToOpen = 0;

    
    // pDict is a borrowed reference 
    pDict = get_pDict();

    if(pDict == NULL)
    {
        syslog(LOG_ERR," get_pDict not succeed \n" );
        return -EXIT_FAILURE;
    }

    pFunc = PyDict_GetItemString(pDict, (char*)"checkIsToOpen");


    if (PyCallable_Check(pFunc))
    {
        
        presult = PyObject_CallFunction(pFunc,"s", PIgate_ID);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"Error in checkIsToOpen \n" );
            return -EXIT_FAILURE;
        }

        isToOpen = (int)PyLong_AsLong(presult);
        
    }
    else 
    {
        syslog(LOG_ERR,"Python checkIsToOpen function is broken!\n" );
        return -EXIT_FAILURE;
    }

    return isToOpen;
}

int validPIgate(const char* PIgate_ID)
{
    PyObject *pFunc,*presult, *pDict;
    PyObject *presultString, *encodedString;
    int presult_length;
    char PIgateReceived[PIGATELEN];

    pDict = get_pDict();
        
    if(pDict == NULL)
    {
        syslog(LOG_ERR," get_pDict not succeed \n" );
        return -EXIT_FAILURE;
    }   
    
    pFunc = PyDict_GetItemString(pDict, (char*)"getPIgates");

    if (PyCallable_Check(pFunc))
    {

        presult = PyObject_CallFunction(pFunc,NULL);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"PIgates not Received\n" );
            return -EXIT_FAILURE;
        }
    }
    else 
    {
        syslog(LOG_ERR,"Python getPIgates function is broken!\n" );
        return -EXIT_FAILURE;
    }


    presult_length = PyObject_Length(presult);

    for(int i = 0; i < presult_length ; i++)
    {
        presultString = PyList_GetItem(presult,i);
        encodedString = PyUnicode_AsEncodedString(presultString, "UTF-8", "strict");

        if (encodedString) 
        { //returns NULL if an exception was raised

            strcpy(PIgateReceived,PyBytes_AsString(encodedString)); //pointer refers to the internal buffer of encodedString

            if(strcmp(PIgateReceived,PIgate_ID) == 0)
            {
                syslog(LOG_INFO,"Valid PIgate_ID:  %s \n", PIgate_ID );
                return EXIT_SUCCESS;
            }

        }
        else
        {
            syslog(LOG_WARNING,"Encoded pointer is NULL\n" );
        }
    }


    return -EXIT_FAILURE;
}