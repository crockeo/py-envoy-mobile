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
  Py_buffer buffer;
  if (!PyArg_ParseTuple(args, "s*", &buffer)) {
    return -1;
  }

  // we copy the data contained inside the buffer so we can tie the lifecycle of the envoy_data
  // contents to the lifecycle of the PyEnvoyDataObject.
  void *bufCpy = (unsigned char *)safe_malloc(buffer.len);
  memcpy(bufCpy, buffer.buf, buffer.len);

  self->data.length = buffer.len;
  self->data.bytes = (unsigned char *)bufCpy;
  self->data.release = envoy_noop_release;
  self->data.context = nullptr;

  return 0;
}

void PyEnvoyData_dealloc(PyEnvoyDataObject *self) {
  if (self->data.bytes != nullptr) {
    // We assume that, if the bytes are populated, then the rest of the envoy_data object is
    // well-formed.
    delete self->data.bytes;
    self->data.release(self->data.context);
  }
  Py_TYPE(self)->tp_free((PyObject *)self);
}
