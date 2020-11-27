#include "py_envoy_engine.h"

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "library/common/main_interface.h"


static void py_dispatch_on_engine_running(void *context) {
  EngineCallbacks *callbacks = static_cast<EngineCallbacks *>(context);
  callbacks->engine->put_thunk([=](Engine& engine) {
    callbacks->on_engine_running(engine);
  });
}

static void py_dispatch_on_exit(void *context) {
  EngineCallbacks *callbacks = static_cast<EngineCallbacks *>(context);
  callbacks->engine->put_thunk([=](Engine& engine) {
    callbacks->on_exit(engine);
  });
}


EngineCallbacks::EngineCallbacks(std::shared_ptr<Engine> engine) {
  this->callbacks = {
    .on_engine_running = &py_dispatch_on_engine_running,
    .on_exit = &py_dispatch_on_exit,
    .context = this,
  };
  this->engine = engine;
}

EngineCallbacks& EngineCallbacks::set_on_engine_running(EngineCallback on_engine_running) {
  this->on_engine_running = on_engine_running;
  return *this;
}

EngineCallbacks& EngineCallbacks::set_on_exit(EngineCallback on_exit) {
  this->on_exit = on_exit;
  return *this;
}


Engine::Engine() {
  this->engine_ = init_engine();
  if (this->engine_ == 0) {
    throw std::runtime_error("failed to init engine");
  }
  this->terminated_ = false;
}

Engine::~Engine() {
  if (!this->terminated_) {
    this->terminate();
  }
}

void Engine::run(const EngineCallbacks& callbacks, const std::string& config, const std::string& log_level) {
  envoy_status_t status = run_engine(this->engine_, callbacks.callbacks, config.c_str(), log_level.c_str());
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to run engine");
  }

  // busy wait for thunks
  while (!this->terminated_) {
    std::optional<EngineCallback> thunk;
    {
      std::unique_lock<std::mutex> lock(this->thunks_mtx_);
      this->thunks_cv_.wait(lock, [this]{ return this->thunks_.size() > 0; });

      thunk = this->thunks_.back();
      this->thunks_.pop_back();
    }

    if (thunk.has_value()) {
      thunk.value()(*this);
    }
  }
}

void Engine::terminate() {
  terminate_engine(this->engine_);
  this->terminated_ = true;
}

void Engine::record_counter(const std::string& name, uint64_t count) {
  envoy_status_t status = ::record_counter(this->engine_, name.c_str(), count);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to record counter");
  }
}

void Engine::gauge_set(const std::string& name, uint64_t value) {
  envoy_status_t status = record_gauge_set(this->engine_, name.c_str(), value);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to set gauge");
  }
}

void Engine::gauge_add(const std::string& name, uint64_t amount) {
  envoy_status_t status = record_gauge_add(this->engine_, name.c_str(), amount);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to add to gauge");
  }
}

void Engine::gauge_sub(const std::string& name, uint64_t amount) {
  envoy_status_t status = record_gauge_sub(this->engine_, name.c_str(), amount);
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to subtract from gauge");
  }
}

void Engine::put_thunk(const EngineCallback&& thunk) {
  std::unique_lock<std::mutex> lock(this->thunks_mtx_);
  this->thunks_.push_back(std::move(thunk));
  this->thunks_cv_.notify_one();
}

envoy_engine_t Engine::handle() {
  return this->engine_;
}
