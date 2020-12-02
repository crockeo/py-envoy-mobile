#include "py_executor_base.h"


void PyExecutorBase::execute(std::function<void ()> func) {
  py::gil_scoped_acquire acquire;
  this->execute_impl(func);
}

void PyExecutorBase::execute_impl(std::function<void ()> func) {
  PYBIND11_OVERRIDE_PURE(void,
                         ExecutorBase,
                         execute_impl,
                         func);
}
