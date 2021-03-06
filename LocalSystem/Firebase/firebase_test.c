
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/include/python3.9/Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PyObject *pName, *pModule, *pDict; 

void initFirebase(void)
{

   PyObject *pFuncInit, *presult;

   // pFunc is also a borrowed reference 
   pFuncInit = PyDict_GetItemString(pDict, (char*)"initFirebase");

    if(PyCallable_Check(pFuncInit))
    {
        presult = PyObject_CallFunction(pFuncInit,NULL);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            printf("Access Error");
            Py_XDECREF(pDict);
            Py_XDECREF(pFuncInit);
            Py_XDECREF(presult);
            Py_Finalize();
            exit(-1);
        }
    }
    else 
    {
        PyErr_Print();
        Py_XDECREF(pDict);
        Py_XDECREF(pFuncInit);
        Py_XDECREF(presult);
        Py_Finalize();
        exit(-1);
    }

    Py_XDECREF(pFuncInit);
    Py_XDECREF(presult);

}

void sendEntry(void)
{
    PyObject *pFunc,*presult;

    pFunc = PyDict_GetItemString(pDict, (char*)"sendEntry");

    if (PyCallable_Check(pFunc))
    {       

        const char* pigate_id = "2";
        const char* plate = "12-33-xx";

        presult = PyObject_CallFunction(pFunc,"ss",pigate_id,plate);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            printf("Error in sendEntry");
            PyErr_Print();
            Py_XDECREF(pDict);
            Py_XDECREF(pFunc);
            Py_XDECREF(presult);
            Py_Finalize();
            exit(-1);
        }
   } 
   else 
   {
       PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pDict);
        Py_XDECREF(presult);
        Py_Finalize();
        exit(-1);
   }

    //Cleanup
   
    Py_XDECREF(pFunc);
    Py_XDECREF(presult);
}

void ReceivePlates(void)
{
    PyObject *pFunc,*presult;
    int presult_length;

    pFunc = PyDict_GetItemString(pDict, (char*)"getPlates");


    if (PyCallable_Check(pFunc))
    {
        presult = PyObject_CallFunction(pFunc,NULL);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            printf("Error in receivePlates");
            Py_XDECREF(pDict);
            Py_XDECREF(pFunc);
            Py_XDECREF(presult);
            Py_Finalize();
            exit(-1);
        }
    }
     else 
     {
        PyErr_Print();
        Py_XDECREF(pDict);
        Py_XDECREF(pFunc);
        Py_XDECREF(presult);
        Py_Finalize();
        exit(-1);
    }

    
    presult_length = PyObject_Length(presult);

    PyObject *presultString, *encodedString;

    for(int i = 0; i < presult_length ; i++)
    {
        presultString = PyList_GetItem(presult,i);
        encodedString = PyUnicode_AsEncodedString(presultString, "UTF-8", "strict");

        if (encodedString) 
        { //returns NULL if an exception was raised
            char *plates = PyBytes_AsString(encodedString); //pointer refers to the internal buffer of encodedString
        
            if(plates) 
            {
                printf("Receive plate: %s", plates);

                printf("\n");
            }
            else
            {
                printf("\nError Decoding trama2\n");
            }   

        }
        else
        {
            printf("\nError Decoding trama\n");
        }

    }

    //Cleanup
    
    Py_CLEAR(pFunc);
    Py_CLEAR(presult);
    Py_CLEAR(presultString);
    Py_CLEAR(encodedString);
}

void isToOpen(void)
{
    PyObject *pFunc,*presult;
    PyObject *presultString, *encodedString;
    int presult_length;

    pFunc = PyDict_GetItemString(pDict, (char*)"checkIsToOpen");


    if (PyCallable_Check(pFunc))
    {
        const char* aux = "1";

        presult = PyObject_CallFunction(pFunc,"s", aux);

        if( (int)PyLong_AsLong(presult) == EINVAL )
        {
            printf("Error in checkIsToOpen \n");
            Py_XDECREF(pDict);
            Py_XDECREF(pFunc);
            Py_XDECREF(presult);
            Py_Finalize();
            exit(-1);
        }

        printf(" \n The isToOpen Variavel is:  %d \n ", (int)PyLong_AsLong(presult));

    }
     else 
     {
        PyErr_Print();
        Py_XDECREF(pDict);
        Py_XDECREF(pFunc);
        Py_XDECREF(presult);
        Py_Finalize();
        exit(-1);
    }



    //Cleanup
    
    Py_XDECREF(pFunc);
    Py_XDECREF(presult);
    Py_XDECREF(presultString);
    Py_XDECREF(encodedString);

}

int main()
{
    while (1)
    {
    
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
        printf("No module with that name");
        return -1;
    }

    // pDict is a borrowed reference 
   pDict = PyModule_GetDict(pModule);


    //initFirebase();
    // sendEntry();
    // sleep(2);
    ReceivePlates();
    
    
    // sleep(2);
    // isToOpen();

    //Cleanup
    

    // Finish the Python Interpreter
    Py_Finalize();

    sleep(30);

    }

    Py_XDECREF(pName);
    Py_XDECREF(pModule);
    Py_XDECREF(pDict);

    return 0;
}