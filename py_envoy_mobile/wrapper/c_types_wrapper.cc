#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <iostream>
#include <utility>
#include <vector>

#include "library/common/main_interface.h"
#include "library/common/types/c_types.h"

#include "py_envoy_data.h"
#include "py_envoy_headers.h"
#include "py_envoy_http_callbacks.h"

// Missing pieces:
//   - implementation for stream methods (outline below)
//
//   - record_...
//     - counter, gauge_set, gauge_add, gauge_sub
//
//   - register_platform_api
//
//   - run_engine
//
//   - envoy_engine_callbacks wrapper
//
//   - envoy_status_t wrapper


// PyEngineObject wraps envoy-mobile's envoy_engine_t type into a Python class that creates an
// engine when it is initialized.
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

PyObject *PyEngineObject_terminate(PyEngineObject *self) {
  terminate_engine(self->engine);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef PyEngineObject_methods[] = {
  {
    "terminate",
    (PyCFunction)PyEngineObject_terminate,
    METH_NOARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyEngineType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Engine",
  .tp_doc = "envoy-mobile engine handle",
  .tp_basicsize = sizeof(PyEngineObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyEngineObject_new,
  .tp_init = (initproc)PyEngineObject_init,
  .tp_methods = PyEngineObject_methods,
};


// PyStreamObject wraps envoy-mobile's envoy_stream_t type into a Python class. Initializing the
// PyStreamObject (with a PyEngineObject as a param) initializes a new stream.
struct PyStreamObject {
  PyObject_HEAD
  envoy_stream_t stream;
};

static PyObject *PyStreamObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyStreamObject *self;
  self = (PyStreamObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->stream = 0;
  }
  return (PyObject *)self;
}

static int PyStreamObject_init(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  static char *kwlist[] = {"engine", nullptr};
  PyEngineObject *engine;

  if (
      !PyArg_ParseTupleAndKeywords(
         args,
         kwargs,
         "O",
         kwlist,
         &engine
      )
  ) {
    return -1;
  }

  self->stream = init_stream(engine->engine);
  if (self->stream < 0) {
    return -1;
  }
  return 0;
}

static PyObject *PyStreamObject_send_headers(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  PyHeadersObject *headers;
  bool close;
  if (!PyArg_ParseTuple(args, "Op:send_headers", &headers, &close)) {
    return nullptr;
  }

  envoy_status_t status = send_headers(self->stream, headers->headers, close);
  std::cout << status << std::endl;
  // TODO: do something with status

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *PyStreamObject_send_data(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  return (PyObject *)self;
}

static PyObject *PyStreamObject_send_metadata(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  return (PyObject *)self;
}

static PyObject *PyStreamObject_send_trailers(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  return (PyObject *)self;
}

static PyObject *PyStreamObject_reset(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  return (PyObject *)self;
}

static PyObject *PyStreamObject_close(PyStreamObject *self, PyObject *args, PyObject *kwargs) {
  return (PyObject *)self;
}

static PyMethodDef PyStreamObject_methods[] = {
  {
    "send_headers",
    (PyCFunction)PyStreamObject_send_headers,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_data",
    (PyCFunction)PyStreamObject_send_data,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_metadata",
    (PyCFunction)PyStreamObject_send_metadata,
    METH_VARARGS,
    nullptr,
  },
  {
    "send_trailers",
    (PyCFunction)PyStreamObject_send_trailers,
    METH_VARARGS,
    nullptr,
  },
  {
    "reset",
    (PyCFunction)PyStreamObject_reset,
    METH_VARARGS,
    nullptr,
  },
  {
    "close",
    (PyCFunction)PyStreamObject_close,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyStreamType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Stream",
  .tp_doc = "envoy-mobile stream handle",
  .tp_basicsize = sizeof(PyStreamObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyStreamObject_new,
  .tp_init = (initproc)PyStreamObject_init,
  .tp_methods = PyStreamObject_methods,
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
  std::vector<std::pair<PyTypeObject *, const char *>> types = {
    {
      &PyEnvoyDataType,
      "Data",
    },
    {
      &PyHeadersType,
      "Headers",
    },
    {
      &PyHttpCallbacksType,
      "HttpCallbacks",
    },
    {
      &PyEngineType,
      "Engine",
    },
    {
      &PyStreamType,
      "Stream",
    },
  };

  for (auto pair : types) {
    if (PyType_Ready(pair.first) < 0) {
      return nullptr;
    }
  }

  auto mod = PyModule_Create(&c_types_wrapper_module);
  if (mod == nullptr)
    return nullptr;

  for (auto pair : types) {
    auto type = pair.first;
    auto name = pair.second;

    Py_INCREF(type);
    if (PyModule_AddObject(mod, name, (PyObject *)type) < 0) {
      Py_DECREF(type);
      Py_DECREF(&mod);
      return nullptr;
    }
  }

  return mod;
}

#ifdef __cplusplus
}
#endif
