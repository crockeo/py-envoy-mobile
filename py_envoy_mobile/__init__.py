from typing import Any

from py_envoy_mobile import wrapper  # type: ignore


class EnvoyConfig:
    def __init__(self):
        self.template = wrapper.get_config_template()
        self.parameters = {
            "virtual_clusters": "[]",
            "dns_refresh_rate_seconds": 3,
            "dns_failure_refresh_rate_seconds_base": 2,
            "dns_failure_refresh_rate_seconds_max": 10,
            "connect_timeout_seconds": 30,
            "stats_domain": "0.0.0.0",
            "stats_flush_interval_seconds": 60,
            "app_id": "unspecified",
            "app_version": "unspecified",
            "device_os": "computer",  # TODO: actually get OS info?
        }

    def set(self, param: str, value: Any) -> "EnvoyConfig":
        if param not in self.parameters:
            raise KeyError(f"{param} not in param list")
        self.parameters[param] = value
        return self

    def build(self):
        template = self.template
        parameters = {
            **self.parameters,
            "platform_filter_chain": "",
        }
        for key, value in parameters.items():
            key = "{{ key }}".replace("key", key)
            template = template.replace(key, str(value))
        return template


def on_engine_running():
    print("on_engine_running")
    engine.terminate()


def on_exit():
    print("on_exit")


if __name__ == "__main__":
    engine = wrapper.Engine()
    callbacks = wrapper.EngineCallbacks(engine).set_on_engine_running(on_engine_running).set_on_exit(on_exit)

    # note: this implicitly waits, maybe change that in the future to put the control into the
    # application layer :)
    engine.run(callbacks, EnvoyConfig().build(), "debug")
