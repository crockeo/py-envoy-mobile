#define PY_SSIZE_T_CLEAN
#include "Python.h"

static PyObject *c_types_wrapper_system(PyObject *self, PyObject *args) {
  const char *command;
  int sts;

  if (!PyArg_ParseTuple(args, "s", &command)) {
    return nullptr;
  }

  sts = system(command);
  return PyLong_FromLong(sts);
}

static PyMethodDef CTypesWrapperMethods[] = {
  {
    "system",
    c_types_wrapper_system,
    METH_VARARGS,
    "Execute a shell command",
  },
  {nullptr, nullptr, 0, nullptr},
};

static struct PyModuleDef c_types_wrapper_module = {
  PyModuleDef_HEAD_INIT,
  "c_types_wrapper",
  nullptr,
  -1,
  CTypesWrapperMethods,
};

PyObject *PyInit_c_types_wrapper(void) {
  return PyModule_Create(&c_types_wrapper_module);
}
