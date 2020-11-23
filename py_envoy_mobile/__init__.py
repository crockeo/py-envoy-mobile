# import time
# from typing import Any
# from py_envoy_mobile import wrapper  # type: ignore
# class EnvoyConfig:
#     def __init__(self):
#         self.template = wrapper.config_template()
#         self.parameters = {
#             "virtual_clusters": "[]",
#             "dns_refresh_rate_seconds": 3,
#             "dns_failure_refresh_rate_seconds_base": 2,
#             "dns_failure_refresh_rate_seconds_max": 10,
#             "connect_timeout_seconds": 30,
#             "stats_domain": "0.0.0.0",
#             "stats_flush_interval_seconds": 60,
#             "app_id": "unspecified",
#             "app_version": "unspecified",
#             "device_os": "computer",  # TODO: actually get OS info?
#         }
#     def set(self, param: str, value: Any) -> "EnvoyConfig":
#         if param not in self.parameters:
#             raise KeyError(f"{param} not in param list")
#         self.parameters[param] = value
#         return self
#     def build(self):
#         template = self.template
#         parameters = {
#             **self.parameters,
#             "platform_filter_chain": "",
#         }
#         for key, value in parameters.items():
#             key = "{{ key }}".replace("key", key)
#             template = template.replace(key, str(value))
#         return template
# def on_engine_running():
#     # TODO: figure out why this isn't being called, but on_exit is 🤔
#     with open("~/log.txt", "w") as f:
#         f.write("does this get run and just not piped to stdout?")
#     print("on_engine_running")
# def on_exit():
#     print("on_exit")
# def main(config: str, debug_level: str):
#     engine = wrapper.Engine()
#     engine_callbacks = wrapper.EngineCallbacks().set_on_engine_running(on_engine_running).set_on_exit(on_exit)
#     # TODO: figure out why this causes an exception from envoy not being able to lock a mutex when we don't terminate
#     engine.run(engine_callbacks, config, debug_level)
#     stream = wrapper.Stream(engine)
#     stream.start(wrapper.HttpCallbacks())
#     headers = (
#         wrapper.Headers().set_header("method", "POST").set_header("authority", "google.com").set_header("path", "/")
#     )
#     stream.send_headers(headers, True)
#     engine.terminate()
# if __name__ == "__main__":
#     main(
#         EnvoyConfig().build(),
#         "debug",
#     )
