#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i, int j) {
  return i + j;
}

PYBIND11_MODULE(__init__, m) {
  m.doc() = "a thin wrapper around envoy-mobile";

  m.def("add", &add, "A function which adds two numbers");
}
