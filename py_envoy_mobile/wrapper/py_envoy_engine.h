#pragma once

#include <functional>
#include <list>
#include <optional>
#include <string>
#include <thread>

#include "executor_base.h"
#include "library/common/types/c_types.h"


class Engine;
using EngineCallback = std::function<void (Engine&)>;


// EngineCallbacks wraps the envoy_engine_callbacks type, which includes two callbacks:
//
//   * on_engine_running -- this runs when the Envoy engine is initialized and ready to handle
//     traffic
//
//   * on_exit -- this runs when the engine exits
struct EngineCallbacks {
  EngineCallbacks(std::shared_ptr<Engine> engine, ExecutorBase& executor);

  EngineCallbacks& set_on_engine_running(EngineCallback on_engine_running);
  EngineCallbacks& set_on_exit(EngineCallback on_exit);

  EngineCallback on_engine_running;
  EngineCallback on_exit;

  envoy_engine_callbacks callbacks;
  std::shared_ptr<Engine> engine;
  ExecutorBase& executor;
};

// Engine wraps the envoy_engine_t type, which is the central handle to performing networking in
// envoy-mobile.
//
// There are several functions in envoy-mobile that receive an envoy_engine_t as the first argument.
// These functions are all included here, except for when they're used to create another data type.
class Engine {
public:
  Engine();
  virtual ~Engine();

  Engine(const Engine&&) = delete;
  Engine& operator=(const Engine&&) = delete;

  bool running() const;

  void run(const EngineCallbacks& callbacks, const std::string& config, const std::string& log_level);
  void terminate();

  void record_counter(const std::string& name, uint64_t count);
  void gauge_set(const std::string& name, uint64_t value);
  void gauge_add(const std::string& name, uint64_t amount);
  void gauge_sub(const std::string& name, uint64_t amount);

  void put_thunk(const EngineCallback&& thunk);
  virtual std::optional<EngineCallback> get_thunk(bool wait) = 0; // trampoline function defined in module_definition.c

  envoy_engine_t handle();

protected:
  std::mutex thunks_mtx_;
  std::condition_variable thunks_cv_;
  std::list<std::function<void (Engine&)>> thunks_;

private:
  envoy_engine_t engine_;
  bool terminated_;
};
