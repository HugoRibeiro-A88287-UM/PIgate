//#include <Python.h>
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/include/python3.9/Python.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{
    int i = 0;
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *presult;
  
  
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

    //Cleanup
    Py_DECREF(pName);
    Py_DECREF(pModule);

   // pFunc is also a borrowed reference 
   pFunc = PyDict_GetItemString(pDict, (char*)"someFunction");

    //Cleanup
    Py_DECREF(pDict);

   if (PyCallable_Check(pFunc))
   {       
       pValue=Py_BuildValue("(z)",(char*)"something");

       PyErr_Print();
       printf("Let's give this a shot!\n");
       presult=PyObject_CallObject(pFunc,pValue);
    PyErr_Print();

   } else 
   {
       PyErr_Print();
   }
   printf("Result is %d\n",(int)PyLong_AsLong(presult));
   //Py_DECREF(pValue);

      //Cleanup
    Py_DECREF(pFunc);
    Py_DECREF(pValue);
    Py_XDECREF(presult);


   // Finish the Python Interpreter
   Py_Finalize();


    return 0;
}
