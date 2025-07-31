#include <Python.h>
#include "maya/MGlobal.h"


static PyObject* py_write(PyObject* self, PyObject* args) {
    const char* msg;
    if (!PyArg_ParseTuple(args, "s", &msg))
        return nullptr;

    // Forward to your output widget
    MyPluginUI::instance()->appendOutput(QString::fromUtf8(msg));

    Py_RETURN_NONE;
}

static PyObject* py_flush(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef PyOutputMethods[] = {
    {"write", py_write, METH_VARARGS, "Write method"},
    {"flush", py_flush, METH_VARARGS, "Flush method"},
    {nullptr, nullptr, 0, nullptr}
};

static PyTypeObject PyOutputType = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "MyOutput",              // tp_name
    sizeof(PyObject),        // tp_basicsize
    0,                       // tp_itemsize
    0,                       // tp_dealloc
    0,                       // tp_print (deprecated)
    0,                       // tp_getattr
    0,                       // tp_setattr
    0,                       // tp_reserved
    0,                       // tp_repr
    0,                       // tp_as_number
    0,                       // tp_as_sequence
    0,                       // tp_as_mapping
    0,                       // tp_hash  
    0,                       // tp_call
    0,                       // tp_str
    0,                       // tp_getattro
    0,                       // tp_setattro
    0,                       // tp_as_buffer
    Py_TPFLAGS_DEFAULT,      // tp_flags
    "Custom stdout catcher", // tp_doc
    0, 0, 0, 0, 0, 0,
    PyOutputMethods          // tp_methods
};
