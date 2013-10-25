#include <Python.h>

#include <unistd.h>

#include "spi_mover.h"

#include "shim.h"


static spi_mover * instance_p = NULL;

/* Test method for shim */
static PyObject *shim_method(PyObject *self)//, PyObject *args)
{
    /* Define variables for arguments */    
    char *s = "HI EVERYBODY";

    /* Parse arguments */
    //if (!PyArg_ParseTuple(args, "s", &s))
    //{
    //    return NULL;
    //}

    /* Do something */
    printf("%s", s);

    /* Return value */
    Py_RETURN_NONE;
}

/* Method mapping table - tells Python which methods are available */
static PyMethodDef shim_funcs[] = {
    {"shim_method", (PyCFunction)shim_method, 
     METH_NOARGS, NULL},
    {NULL}
};

/* Init function - this is called when the module is loaded in Python */
void initshimext(void)
{
    Py_InitModule3("shimext", shim_funcs, "Extension test");
}

bool shim_init(const char * path)
{
	if(instance_p != NULL)
	{
		return false;
	}

	instance_p = new spi_mover(path);
	
	return true;
}


bool shim_transfer(int num, uint8_t* out, uint8_t* in)
{
	if(instance_p == NULL)
	{
		return false;
	}

	return(instance_p->transfer(num,  out, in));
}


void shim_uninit()
{
	if(instance_p != NULL)
	{
		delete instance_p;
		instance_p = NULL;
	}
}
