#include <Python.h>

/******************************************************************************
 * Your methods
 *****************************************************************************/
static PyObject* test_method(PyObject* self, PyObject *args)
{
    int a;
    int b;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    /* Return value */
    return Py_BuildValue("i", a * b);
}

/******************************************************************************
 * Method mapping table
 *****************************************************************************/
static char shim_docs[] =
    "test_method( ): Any message you want to put here!!\n";

static PyMethodDef helloworld_funcs[] = {
    {"test_method", (PyCFunction)test_method, 
     METH_VARARGS, shim_docs},
    {NULL}
};

/******************************************************************************
 * Initialization function that's called when Python loads the extension
 *****************************************************************************/
PyMODINIT_FUNC initshim(void)
{
    Py_InitModule3("shim", helloworld_funcs,
                   "Extension module example!");
}
