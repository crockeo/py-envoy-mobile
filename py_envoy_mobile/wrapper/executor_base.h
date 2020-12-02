#pragma once

#include <functional>


class ExecutorBase {
public:
  ExecutorBase() { }
  virtual ~ExecutorBase() { }

  // execute is a trampoline function that takes the GIL and then calls execute_impl. execute is
  // meant to run from a non-main thread that will not otherwise have the GIL.
  virtual void execute(std::function<void ()> func) = 0;

  // execute_impl is a trampoline function that will be overriden inside of subclasses of this type.
  // implementations must be a thread-safe mechanism to schedule the work contained inside of the
  // provided function asynchronously.
  virtual void execute_impl(std::function<void ()> func) = 0;
};
