#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/syslog.h>
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/include/python3.9/Python.h"
#include "../inc/utilits.h"
#include "../inc/firebase.h"


int sendEntry(const char* PIgate_ID, const char* Plate)
{
    PyObject *pName, *pModule, *pDict ; 
    PyObject *pFunc,*presult;

    // Set PYTHONPATH TO working directory
    setenv("PYTHONPATH","/etc/",1);

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString((char*)"firebase");

    // Load the module object
    pModule = PyImport_Import(pName);

    if(pModule == NULL)
    {
        syslog(LOG_ERR,"No firebase.py available!\n" );
        return -EXIT_FAILURE;
    }

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    if(pDict == NULL)
    {
        syslog(LOG_ERR,"pDict is pointing to NULL \n" );
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

    syslog(LOG_INFO, "Entry sent ! :D");

    return EXIT_SUCCESS;
}

int receivePlates(void)
{
    PyObject *pName, *pModule, *pDict ; 
    PyObject *pFunc,*presult;
    PyObject *presultString, *encodedString;
    int presult_length;
    static int firstTime = 1;
    char platesSize[PLATESSIZE+1];

    if(firstTime)
    {
        close(recPlatePIPE[0]); // Close reading end 
        firstTime = 0;

    }

    // Set PYTHONPATH TO working directory
    setenv("PYTHONPATH","/etc/",1);

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString((char*)"firebase");

    // Load the module object
    pModule = PyImport_Import(pName);

    if(pDict == NULL)
    {
        syslog(LOG_ERR,"pDict is pointing to NULL \n" );
        return -EXIT_FAILURE;
    }

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);
        
    if(pDict == NULL)
        return -EXIT_FAILURE;    
    
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
        { //returns NULL if an exception was raised
            //char *plates = PyBytes_AsString(encodedString); //pointer refers to the internal buffer of encodedString

            strcpy(platesSize,PyBytes_AsString(encodedString));

            syslog(LOG_INFO, "Plate: %s \n", platesSize );

            write(recPlatePIPE[1], platesSize, (PLATESSIZE+1) );

        }
        else
        {
            syslog(LOG_WARNING,"Encoded pointer is NULL\n" );
        }
    }


    return EXIT_SUCCESS;
}

int isToOpen(const char* PIgate_ID)
{
    PyObject *pName, *pModule, *pDict ; 
    PyObject *pFunc,*presult;
    int presult_length;
    int isToOpen = 0;

    // Set PYTHONPATH TO working directory
    setenv("PYTHONPATH","/etc/",1);

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString((char*)"firebase");

    // Load the module object
    pModule = PyImport_Import(pName);

    if(pModule == NULL)
    {
        syslog(LOG_ERR,"No firebase.py available!\n" );
        return -EXIT_FAILURE;
    }

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    if(pDict == NULL)
    {
        syslog(LOG_ERR,"pDict is pointing to NULL \n" );
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

    //Cleanup
    return isToOpen;
}