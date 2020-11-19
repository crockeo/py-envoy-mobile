#include "py_envoy_engine.h"


#include "library/common/main_interface.h"
#include "library/common/types/c_types.h"


PyObject *PyEngineObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyEngineObject *self;
  self = (PyEngineObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->engine = 0;
  }
  return (PyObject *)self;
}

int PyEngineObject_init(PyEngineObject *self, PyObject *args, PyObject *kwargs) {
  envoy_engine_t engine = init_engine();
  if (engine == 0) {
    return -1;
  }

  self->engine = engine;
  return 0;
}

PyObject *PyEngineObject_terminate(PyEngineObject *self) {
  terminate_engine(self->engine);
  Py_INCREF(Py_None);
  return Py_None;
}
