#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <iostream>
#include <utility>
#include <vector>

#include "library/common/main_interface.h"
#include "library/common/types/c_types.h"

#include "py_envoy_data.h"
#include "py_envoy_engine.h"
#include "py_envoy_engine_callbacks.h"
#include "py_envoy_headers.h"
#include "py_envoy_http_callbacks.h"
#include "py_envoy_stream.h"

// Missing pieces:
//   - implementation for stream methods (outline below)
//
//   - record_...
//     - counter, gauge_set, gauge_add, gauge_sub
//
//   - register_platform_api


static PyObject *c_types_wrapper_config_template() {
  PyObject *py_string_config_template;
  py_string_config_template = Py_BuildValue("s", config_template);
  if (py_string_config_template == nullptr) {
    PyErr_SetString(PyExc_RuntimeError, "failed to build config template");
    return nullptr;
  }

  Py_INCREF(py_string_config_template);
  return py_string_config_template;
}

static PyMethodDef c_types_wrapper_methods[] = {
  {
    "config_template",
    (PyCFunction)c_types_wrapper_config_template,
    METH_NOARGS,
    nullptr,
  },
  {nullptr},
};


static struct PyModuleDef c_types_wrapper_module = {
  PyModuleDef_HEAD_INIT,
  "c_types_wrapper",
  nullptr,
  -1,
  c_types_wrapper_methods,
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
    {
      &PyEngineCallbacksType,
      "EngineCallbacks",
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
