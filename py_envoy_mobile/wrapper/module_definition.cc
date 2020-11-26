#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
namespace py = pybind11;

#include "library/common/main_interface.h"
#include "py_envoy_engine.h"


const std::string get_config_template() {
  return std::string(config_template);
}

const std::string get_platform_filter_template() {
  return std::string(platform_filter_template);
}

PYBIND11_MODULE(wrapper, m) {
  m.def("get_config_template", &get_config_template);
  m.def("get_platform_filter_template", &get_platform_filter_template);

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
}
