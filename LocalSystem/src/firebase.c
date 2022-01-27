#include "../inc/firebase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/syslog.h>
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/include/python3.9/Python.h"

PyObject *pDict = NULL; 

int initFirebase(void)
{
    PyObject *pName, *pModule;

    // Set PYTHONPATH TO working directory
    setenv("PYTHONPATH",".",1);
    
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

    //Cleanup
    Py_XDECREF(pName);
    Py_XDECREF(pModule);

    return EXIT_SUCCESS;
}

void remFirebase(void)
{

    //Cleanup
    Py_XDECREF(pDict);

    // Finish the Python Interpreter
    Py_Finalize();

    return EXIT_SUCCESS;
}

int sendEntry(const char* PIgate_ID, const char* Plate)
{
    PyObject *pFunc,*presult;

    if(pDict == NULL)
        return -EXIT_FAILURE;

    pFunc = PyDict_GetItemString(pDict, (char*)"sendEntry");

    if (PyCallable_Check(pFunc))
    {       

        presult = PyObject_CallFunction(pFunc,"ss",PIgate_ID,Plate);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"Entry not sent \n" );
            PyErr_Print();
            Py_XDECREF(pDict);
            Py_XDECREF(pFunc);
            Py_XDECREF(presult);
            Py_Finalize();
            return -EXIT_FAILURE;
        }
   } 
   else 
   {
        syslog(LOG_ERR,"Python sendEntry function is broken!\n" );
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pDict);
        Py_XDECREF(presult);
        Py_Finalize();
        return -EXIT_FAILURE;
   }

    //Cleanup
   
    Py_XDECREF(pFunc);
    Py_XDECREF(presult);

    return EXIT_SUCCESS;
}

int receivePlates(void)
{
    PyObject *pFunc,*presult;
    int presult_length;

    if(pDict == NULL)
        return -EXIT_FAILURE;

    pFunc = PyDict_GetItemString(pDict, (char*)"getPlates");


    if (PyCallable_Check(pFunc))
    {
        presult = PyObject_CallFunction(pFunc,NULL);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"Plates not Received\n" );
            Py_XDECREF(pDict);
            Py_XDECREF(pFunc);
            Py_XDECREF(presult);
            Py_Finalize();
            return -EXIT_FAILURE;
        }
    }
    else 
    {
        syslog(LOG_ERR,"Python getPlates function is broken!\n" );
        PyErr_Print();
        Py_XDECREF(pDict);
        Py_XDECREF(pFunc);
        Py_XDECREF(presult);
        Py_Finalize();
        return -EXIT_FAILURE;
    }

    
    presult_length = PyObject_Length(presult);

    PyObject *presultString, *encodedString;

    for(int i = 0; i < presult_length ; i++)
    {
        presultString = PyList_GetItem(presult,i);
        encodedString = PyUnicode_AsEncodedString(presultString, "UTF-8", "strict");

        //Insert length in PIPE

        if (encodedString) 
        { //returns NULL if an exception was raised
            char *plates = PyBytes_AsString(encodedString); //pointer refers to the internal buffer of encodedString
        
            if(plates) 
            {
                //Insert IN PIPE
            }
            else
            {
                syslog(LOG_WARNING,"Plates pointer is NULL\n" );
            }   
        }
        else
        {
            syslog(LOG_WARNING,"Encoded pointer is NULL\n" );
        }
    }

    //Cleanup
    Py_XDECREF(pFunc);
    Py_XDECREF(presult);
    Py_XDECREF(presultString);
    Py_XDECREF(encodedString);

    return EXIT_SUCCESS;
}

int isToOpen(void)
{
    PyObject *pFunc,*presult;
    PyObject *presultString;
    int presult_length;
    int isToOpen = 0;

    if(pDict == NULL)
        return -EXIT_FAILURE;

    pFunc = PyDict_GetItemString(pDict, (char*)"checkIsToOpen");


    if (PyCallable_Check(pFunc))
    {
        const char* aux = "1";

        presult = PyObject_CallFunction(pFunc,"s", aux);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            syslog(LOG_WARNING,"Error in checkIsToOpen \n" );
            Py_XDECREF(pDict);
            Py_XDECREF(pFunc);
            Py_XDECREF(presult);
            Py_Finalize();
            return -EXIT_FAILURE;
        }

        isToOpen = (int)PyLong_AsLong(presult);
        syslog(LOG_INFO, " \n The isToOpen Variavel is:  %d \n ", isToOpen);
        
    }
    else 
    {
        syslog(LOG_ERR,"Python checkIsToOpen function is broken!\n" );
        PyErr_Print();
        Py_XDECREF(pDict);
        Py_XDECREF(pFunc);
        Py_XDECREF(presult);
        Py_Finalize();
        return -EXIT_FAILURE;
    }

    //Cleanup
    
    Py_XDECREF(pFunc);
    Py_XDECREF(presult);
    Py_XDECREF(presultString);

    return isToOpen;
}