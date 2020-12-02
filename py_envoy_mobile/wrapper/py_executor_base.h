#pragma once

#include "executor_base.h"
#include "pybind11/pybind11.h"


namespace py = pybind11;

class PyExecutorBase : public ExecutorBase {
public:
  using ExecutorBase::ExecutorBase;

  void execute(std::function<void ()> func) override;
  void execute_impl(std::function<void ()> func) override;
};
