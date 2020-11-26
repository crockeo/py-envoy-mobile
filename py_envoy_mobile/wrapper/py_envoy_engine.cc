#include "py_envoy_engine.h"

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "library/common/main_interface.h"


// these functions are defined statically (in both senses!) because:
//
//   - (in the sense of why they're not just std::function directly) envoy-mobile requires a
//     function pointer, which lambdas can't provide
//
//   - (in the sense of the static keyword) because they're only used here
static void py_dispatch_on_engine_running(void *context) {
  EngineCallbacks *callbacks = static_cast<EngineCallbacks *>(context);
  callbacks->engine->put_thunk([=](Engine& engine) {
    // TODO: call with engine
    callbacks->on_engine_running();
  });
}

static void py_dispatch_on_exit(void *context) {
  EngineCallbacks *callbacks = static_cast<EngineCallbacks *>(context);
  callbacks->engine->put_thunk([=](Engine& engine) {
    // TODO: call with engine
    callbacks->on_exit();
  });
}


//
// EngineCallbacsk
//
EngineCallbacks::EngineCallbacks(std::shared_ptr<Engine> engine) {
  this->callbacks = {
    .on_engine_running = &py_dispatch_on_engine_running,
    .on_exit = &py_dispatch_on_exit,
    .context = this,
  };
  this->engine = engine;
}

EngineCallbacks& EngineCallbacks::set_on_engine_running(std::function<void ()> on_engine_running) {
  this->on_engine_running = on_engine_running;
  return *this;
}

EngineCallbacks& EngineCallbacks::set_on_exit(std::function<void ()> on_exit) {
  this->on_exit = on_exit;
  return *this;
}


//
// Engine
//
Engine::Engine() {
  this->engine_ = init_engine();
  if (this->engine_ == 0) {
    throw std::runtime_error("failed to init engine");
  }
  this->terminated_ = false;
}

void Engine::run(const EngineCallbacks& callbacks, const std::string& config, const std::string& log_level) {
  envoy_status_t status = run_engine(this->engine_, callbacks.callbacks, config.c_str(), log_level.c_str());
  if (status == ENVOY_FAILURE) {
    throw std::runtime_error("failed to run engine");
  }

  // busy wait for thunks
  while (true) {
    std::optional<std::function<void (Engine&)>> thunk;
    {
      std::unique_lock<std::mutex> lock(this->thunks_mtx_);
      if (this->thunks_.size() > 0) {
        thunk = this->thunks_.back();
        this->thunks_.pop_back();
      }
    }

    if (thunk.has_value()) {
      thunk.value()(*this);
    }

    if (this->terminated_) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

void Engine::put_thunk(const std::function<void (Engine&)>&& thunk) {
  std::unique_lock<std::mutex> lock(this->thunks_mtx_);
  this->thunks_.push_back(std::move(thunk));
}
