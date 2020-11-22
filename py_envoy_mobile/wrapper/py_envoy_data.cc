#include "py_envoy_data.h"


PyObject *PyEnvoyData_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyEnvoyDataObject *self;
  self = (PyEnvoyDataObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->data.length = 0;
    self->data.bytes = nullptr;
    self->data.release = nullptr;
    self->data.context = nullptr;
  }
  return (PyObject *)self;
}

int PyEnvoyData_init(PyEnvoyDataObject *self, PyObject *args, PyObject *kwargs) {
  unsigned const char *data;
  Py_ssize_t data_len;
  if (!PyArg_ParseTuple(args, "s#", &data, &data_len)) {
    return -1;
  }

  self->data = copy_envoy_data(data_len, data);

  return 0;
}

void PyEnvoyData_dealloc(PyEnvoyDataObject *self) {
  if (self->data.bytes != nullptr) {
    self->data.release(self->data.context);
  }
  Py_TYPE(self)->tp_free((PyObject *)self);
}
