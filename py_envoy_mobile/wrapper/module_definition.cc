#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
namespace py = pybind11;

#include "library/common/main_interface.h"
#include "py_envoy_data.h"
#include "py_envoy_engine.h"
#include "py_envoy_headers.h"
#include "py_envoy_stream.h"


const std::string get_config_template() {
  return std::string(config_template);
}

const std::string get_platform_filter_template() {
  return std::string(platform_filter_template);
}

// TODO: figure out why we're failing at import time again
PYBIND11_MODULE(wrapper, m) {
  m.def("get_config_template", &get_config_template);
  m.def("get_platform_filter_template", &get_platform_filter_template);

  py::class_<Data>(m, "Data")
    .def(py::init<const std::string&>());

  py::class_<EngineCallbacks>(m, "EngineCallbacks")
    .def(py::init<std::shared_ptr<Engine>>())
    .def("set_on_engine_running", &EngineCallbacks::set_on_engine_running)
    .def("set_on_exit", &EngineCallbacks::set_on_exit);

  py::class_<Engine, std::shared_ptr<Engine>>(m, "Engine")
    .def(py::init<>())
    .def("run", &Engine::run)
    .def("terminate", &Engine::terminate)
    .def("terminate", &Engine::terminate)
    .def("record_counter", &Engine::record_counter)
    .def("gauge_set", &Engine::gauge_set)
    .def("gauge_add", &Engine::gauge_add)
    .def("gauge_sub", &Engine::gauge_sub);

  py::class_<Headers>(m, "Headers")
    .def(py::init<>())
    .def("add", &Headers::add);

  py::class_<StreamCallbacks>(m, "StreamCallbacks")
    .def(py::init<std::shared_ptr<Stream>>())
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
