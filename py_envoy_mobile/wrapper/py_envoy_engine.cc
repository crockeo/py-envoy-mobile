#include "py_envoy_engine.h"

#include "library/common/main_interface.h"


// these functions are defined statically (in both senses!) because:
//
//   - (in the sense of why they're not just std::function directly) envoy-mobile requires a
//     function pointer, which lambdas can't provide
//
//   - (in the sense of the static keyword) because they're only used here
static void py_dispatch_on_engine_running(void *context) {
  EngineCallbacks *callbacks = static_cast<EngineCallbacks *>(context);
  callbacks->on_engine_running();
}

static void py_dispatch_on_exit(void *context) {
  EngineCallbacks *callbacks = static_cast<EngineCallbacks *>(context);
  callbacks->on_exit();
}


EngineCallbacks::EngineCallbacks() {
  this->callbacks = {
    .on_engine_running = &py_dispatch_on_engine_running,
    .on_exit = &py_dispatch_on_exit,
    .context = this,
  };
}

EngineCallbacks& EngineCallbacks::set_on_engine_running(std::function<void ()> on_engine_running) {
  this->on_engine_running = on_engine_running;
  return *this;
}

EngineCallbacks& EngineCallbacks::set_on_exit(std::function<void ()> on_exit) {
  this->on_exit = on_exit;
  return *this;
}


Engine::Engine() {
  this->engine_ = init_engine();
  if (this->engine_ == 0) {
    // TODO(chillen): convert this to a python exception
  }
}

void Engine::run(const EngineCallbacks& callbacks, const std::string& config, const std::string& log_level) {
  envoy_status_t status = run_engine(this->engine_, callbacks.callbacks, config.c_str(), log_level.c_str());
  if (status == ENVOY_FAILURE) {
    // TODO(chillen): convert this to a python exception
  }
}

void Engine::terminate() {
  terminate_engine(self->engine_);
}

void Engine::record_counter(const std::string& name, uint64_t count) {
  envoy_status_t status = record_counter_inc(this->engine_, name.c_str(), count);
  if (status == ENVOY_FAILURE) {
    // TODO(chillen): convert this to a python exception
  }
}

void Engine::gauge_set(const std::string& name, uint64_t value) {
  envoy_status_t status = record_gauge_set(this->engine_, name.c_str(), value);
  if (status == ENVOY_FAILURE) {
    // TODO(chillen): convert this to a python exception
  }
}

void Engine::gauge_add(const std::string& name, uint64_t amount) {
  envoy_status_t status = record_gauge_add(this->engine_, name.c_str(), amount);
  if (status == ENVOY_FAILURE) {
    // TODO(chillen): convert this to a python exception
  }
}

void Engine::gauge_sub(const std::string& name, uint64_t amount) {
  envoy_status_t status = record_gauge_sub(this->engine_, name.c_str(), amount);
  if (status == ENVOY_FAILURE) {
    // TODO(chillen): convert this to a python exception
  }
}
