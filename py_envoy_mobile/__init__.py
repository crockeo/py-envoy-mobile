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


def on_headers(engine: wrapper.Engine, stream: wrapper.Stream, data: wrapper.Data, closed: bool):
    print("on headers")


def on_data(engine: wrapper.Engine, stream: wrapper.Stream, headers: wrapper.Headers, closed: bool):
    print("on data")


def on_metadata(engine: wrapper.Engine, stream: wrapper.Stream, metadata: wrapper.Headers):
    print("on metadata")


def on_trailers(engine: wrapper.Engine, stream: wrapper.Stream, trailers: wrapper.Headers):
    print("on trailers")


# TODO: enable this once we have errors coming up
# def on_error():
#     pass


def on_complete(engine: wrapper.Engine, stream: wrapper.Stream):
    print("on complete")


def on_cancel(engine: wrapper.Engine, stream: wrapper.Stream):
    print("on cancel")


def on_engine_running(engine: wrapper.Engine):
    print("on_engine_running")

    stream = wrapper.Stream(engine)
    stream_callbacks = (
        wrapper.StreamCallbacks(stream)
        .set_on_headers(on_headers)
        .set_on_data(on_data)
        .set_on_metadata(on_metadata)
        .set_on_trailers(on_trailers)
        .set_on_complete(on_complete)
        .set_on_cancel(on_cancel)
    )
    stream.start(stream_callbacks)

    # TODO: actually do something interesting here

    engine.terminate()


# TODO: this is never actually called, because we call terminate() from within on_engine_running and
# kill the Engine event loop
def on_exit(engine: wrapper.Engine):
    print("on_exit")


if __name__ == "__main__":
    engine = wrapper.Engine()
    callbacks = wrapper.EngineCallbacks(engine).set_on_engine_running(on_engine_running).set_on_exit(on_exit)

    # note: this implicitly waits, maybe change that in the future to put the control into the
    # application layer :)
    engine.run(callbacks, EnvoyConfig().build(), "info")
