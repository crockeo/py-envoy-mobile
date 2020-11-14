#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <iostream>

#include "library/common/main_interface.h"
#include "library/common/types/c_types.h"

// PyEngineObject wraps envoy-mobile's envoy_engine_t type into a Python class that creates an engine when it is
// initialized.
struct PyEngineObject {
  PyObject_HEAD
  envoy_engine_t engine;
};

static PyObject *PyEngineObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyEngineObject *self;
  self = (PyEngineObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->engine = 0;
  }
  return (PyObject *)self;
}

static int PyEngineObject_init(PyEngineObject *self, PyObject *args, PyObject *kwargs) {
  envoy_engine_t engine = init_engine();
  if (engine == 0) {
    return -1;
  }

  self->engine = engine;
  return 0;
}

static void PyEngineObject_dealloc(PyEngineObject *self) {
  if (self->engine != 0) {
    terminate_engine(self->engine);
  }
}

static PyTypeObject PyEngineType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Engine",
  .tp_doc = "envoy-mobile engine handle",
  .tp_basicsize = sizeof(PyEngineObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyEngineObject_new,
  .tp_init = (initproc)PyEngineObject_init,
  .tp_dealloc = (destructor)PyEngineObject_dealloc,
};


static struct PyModuleDef c_types_wrapper_module = {
  PyModuleDef_HEAD_INIT,
  "c_types_wrapper",
  nullptr,
  -1,
};

#ifdef __cplusplus
extern "C" {
#endif

PyMODINIT_FUNC
PyInit_c_types_wrapper(void) {
  if (PyType_Ready(&PyEngineType) < 0)
    return nullptr;

  auto mod = PyModule_Create(&c_types_wrapper_module);
  if (mod == nullptr)
    return nullptr;

  Py_INCREF(&PyEngineType);
  if (PyModule_AddObject(mod, "Engine", (PyObject *)&PyEngineType) < 0) {
    Py_DECREF(&PyEngineType);
    Py_DECREF(&mod);
    return nullptr;
  }

  return mod;
}

#ifdef __cplusplus
}
#endif
