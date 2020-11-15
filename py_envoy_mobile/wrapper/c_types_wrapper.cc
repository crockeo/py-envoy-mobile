#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <iostream>
#include <utility>
#include <vector>

#include "library/common/main_interface.h"
#include "library/common/types/c_types.h"

// Missing pieces:
//   - implementation for stream methods (outline below)
//
//   - envoy_header wrapper
//   - envoy_headers wrapper
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

struct PyEnvoyDataObject {
  PyObject_HEAD
  envoy_data data;
};

static PyObject *PyEnvoyData_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);
static PyObject *PyEnvoyData_init(PyEnvoyDataObject *self, PyObject *args, PyObject *kwargs);
static void PyEnvoyData_dealloc(PyEnvoyDataObject *self);

static PyTypeObject PyEnvoyDataType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Data",
  .tp_doc = "",
  .tp_basicsize = sizeof(PyEnvoyDataObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyEnvoyData_new,
  .tp_init = (initproc)PyEnvoyData_init,
  .tp_dealloc = (destructor)PyEnvoyData_dealloc,
};

static PyObject *PyEnvoyData_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
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

static PyObject *PyEnvoyData_init(PyEnvoyDataObject *self, PyObject *args, PyObject *kwargs) {
  Py_buffer buffer;
  if (!PyArg_ParseTuple(args, "s*", &buffer)) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  // we copy the data contained inside the buffer so we can tie the lifecycle of the envoy_data
  // contents to the lifecycle of the PyEnvoyDataObject.
  void *bufCpy = (unsigned char *)safe_malloc(buffer.len);
  memcpy(bufCpy, buffer.buf, buffer.len);

  self->data.length = buffer.len;
  self->data.bytes = (unsigned char *)bufCpy;
  self->data.release = envoy_noop_release;
  self->data.context = nullptr;

  Py_INCREF(self);
  return (PyObject *)self;
}

static void PyEnvoyData_dealloc(PyEnvoyDataObject *self) {
  if (self->data.bytes != nullptr) {
    // We assume that, if the bytes are populated, then the rest of the envoy_data object is
    // well-formed.
    delete self->data.bytes;
    self->data.release(self->data.context);
  }
  Py_TYPE(self)->tp_free((PyObject *)self);
}

// TODO: populate headers defn
struct PyHeadersObject {
  PyObject_HEAD
  envoy_headers headers;
};

static PyObject *PyHeadersObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyHeadersObject *self;
  self = (PyHeadersObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->headers.length = 0;
    self->headers.headers = nullptr;
  }
  return (PyObject *)self;
}

static PyObject *PyHeadersObject_set_header(PyHeadersObject *self, PyObject *args) {
  // PyEnvoyDataObject *header;
  // PyEnvoyDataObject *value;

  // if (PyArg_ParseTuple(args, "OO", &header, &value) < 0) {
  //   Py_INCREF(self);
  //   return (PyObject *)self;
  // }

  // envoy_header header = {
  //   header->data,
  //   value->data,
  // };

  // TODO: actually set the header

  Py_INCREF(self);
  return (PyObject *)self;
}

static PyMethodDef PyHeadersObject_methods[] = {
  {
    "set_header",
    (PyCFunction)PyHeadersObject_set_header,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyHeadersType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.Headers",
  .tp_doc = "",
  .tp_basicsize = sizeof(PyHeadersObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyHeadersObject_new,
  .tp_methods = PyHeadersObject_methods,
};


// TODO: populate the set_...
struct PyHttpCallbacksObject {
  PyObject_HEAD
  envoy_http_callbacks http_callbacks;
};

static PyObject *PyHttpCallbacksObject_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
  PyHttpCallbacksObject *self;
  self = (PyHttpCallbacksObject *)type->tp_alloc(type, 0);
  if (self != nullptr) {
    self->http_callbacks.on_headers = nullptr;
    self->http_callbacks.on_data = nullptr;
    self->http_callbacks.on_metadata = nullptr;
    self->http_callbacks.on_trailers = nullptr;
    self->http_callbacks.on_error = nullptr;
    self->http_callbacks.on_complete = nullptr;
    self->http_callbacks.on_cancel = nullptr;
  }
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_headers(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_data(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_metadata(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_trailers(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_error(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_complete(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyObject *PyHttpCallbacksObject_set_on_cancel(PyHttpCallbacksObject *self, PyObject *args) {
  Py_INCREF(self);
  return (PyObject *)self;
}

static PyMethodDef PyHttpCallbacksObject_methods[] = {
  {
    "set_on_headers",
    (PyCFunction)PyHttpCallbacksObject_set_on_headers,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_data",
    (PyCFunction)PyHttpCallbacksObject_set_on_data,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_metadata",
    (PyCFunction)PyHttpCallbacksObject_set_on_metadata,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_trailers",
    (PyCFunction)PyHttpCallbacksObject_set_on_trailers,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_error",
    (PyCFunction)PyHttpCallbacksObject_set_on_error,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_complete",
    (PyCFunction)PyHttpCallbacksObject_set_on_complete,
    METH_VARARGS,
    nullptr,
  },
  {
    "set_on_cancel",
    (PyCFunction)PyHttpCallbacksObject_set_on_cancel,
    METH_VARARGS,
    nullptr,
  },
  {nullptr},
};

static PyTypeObject PyHttpCallbacksType = {
  PyVarObject_HEAD_INIT(nullptr, 0)

  .tp_name = "c_types_wrapper.HttpCallbacks",
  .tp_doc = "",
  .tp_basicsize = sizeof(PyHttpCallbacksObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyHttpCallbacksObject_new,
  .tp_methods = PyHttpCallbacksObject_methods,
};


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
  return (PyObject *)self;
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
