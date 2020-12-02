#pragma once

#include <functional>


class ExecutorBase {
public:
  ExecutorBase() { }
  virtual ~ExecutorBase() { }

  // execute is a trampoline function that is meant to schedule work to be done inside of Python.
  virtual void execute(std::function<void ()> func) = 0;
};
