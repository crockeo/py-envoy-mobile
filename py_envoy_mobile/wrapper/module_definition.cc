#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
#include "pybind11/stl.h"
namespace py = pybind11;

#include "library/common/main_interface.h"
#include "executor_base.h"
#include "py_envoy_data.h"
#include "py_envoy_engine.h"
#include "py_envoy_headers.h"
#include "py_envoy_stream.h"

class PyExecutorBase : public ExecutorBase {
public:
  using ExecutorBase::ExecutorBase;

  void execute(std::function<void ()> func) override {
    PYBIND11_OVERRIDE_PURE(void,
                           ExecutorBase,
                           execute,
                           func);
  }
};

class PyEngine : public Engine {
public:
  using Engine::Engine;

  std::optional<EngineCallback> get_thunk(bool wait) {
    std::unique_lock<std::mutex> lock(this->thunks_mtx_);
    if (wait) {
      py::gil_scoped_release release;
      this->thunks_cv_.wait(lock, [this]{ return this->thunks_.size() > 0; });
    }

    if (this->thunks_.size() == 0) {
      return std::optional<EngineCallback>();
    }

    auto thunk = this->thunks_.front();
    this->thunks_.pop_front();
    return std::optional(thunk);
  }
};

const std::string get_config_template() {
  return std::string(config_template);
}

const std::string get_platform_filter_template() {
  return std::string(platform_filter_template);
}

PYBIND11_MODULE(wrapper, m) {
  m.def("get_config_template", &get_config_template);
  m.def("get_platform_filter_template", &get_platform_filter_template);

  py::class_<ExecutorBase, PyExecutorBase>(m, "ExecutorBase")
    .def(py::init<>())
    .def("execute", &ExecutorBase::execute);

  py::class_<Data>(m, "Data")
    .def(py::init<const std::string&>())
    .def("as_str", &Data::as_str)
    .def("as_bytes", [](const Data& d) {
      return py::bytes(d.as_str());
    });

  py::class_<EngineCallbacks>(m, "EngineCallbacks")
    .def(py::init<std::shared_ptr<Engine>, ExecutorBase&>(), py::keep_alive<1, 3>())
    .def("set_on_engine_running", &EngineCallbacks::set_on_engine_running)
    .def("set_on_exit", &EngineCallbacks::set_on_exit);

  py::class_<Engine, PyEngine, std::shared_ptr<Engine>>(m, "Engine")
    .def(py::init<>())
    .def("running", &Engine::running)
    .def("run", &Engine::run)
    .def("terminate", &Engine::terminate)
    .def("terminate", &Engine::terminate)
    .def("record_counter", &Engine::record_counter)
    .def("gauge_set", &Engine::gauge_set)
    .def("gauge_add", &Engine::gauge_add)
    .def("gauge_sub", &Engine::gauge_sub)
    .def("get_thunk", &Engine::get_thunk);

  py::class_<Headers>(m, "Headers")
    .def(py::init<>())
    .def("__getitem__", &Headers::operator[])
    .def("__iter__", [](const Headers& h) {
      return py::make_iterator(h.begin(), h.end());
    }, py::keep_alive<0, 1>())
    .def("add", &Headers::add);

  py::class_<StreamCallbacks>(m, "StreamCallbacks")
    .def(py::init<std::shared_ptr<Stream>, ExecutorBase&>(), py::keep_alive<1, 3>())
    .def("set_on_headers", &StreamCallbacks::set_on_headers)
    .def("set_on_data", &StreamCallbacks::set_on_data)
    .def("set_on_metadata", &StreamCallbacks::set_on_metadata)
    .def("set_on_trailers", &StreamCallbacks::set_on_trailers)
    .def("set_on_error", &StreamCallbacks::set_on_error)
    .def("set_on_complete", &StreamCallbacks::set_on_complete)
    .def("set_on_cancel", &StreamCallbacks::set_on_cancel);

  py::class_<Stream, std::shared_ptr<Stream>>(m, "Stream")
    .def(py::init<std::shared_ptr<Engine>>())
    .def("start", &Stream::start)
    .def("send_headers", &Stream::send_headers)
    .def("send_data", &Stream::send_data)
    .def("send_metadata", &Stream::send_metadata)
    .def("send_trailers", &Stream::send_trailers)
    .def("reset", &Stream::reset)
    .def("close", &Stream::close);
}
