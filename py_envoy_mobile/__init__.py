import faulthandler
import sys
from typing import Any

import gevent.event

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
            "device_os": sys.platform,
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


def on_engine_running(engine: wrapper.Engine):
    print("on_engine_running")

    stream = wrapper.Stream(engine)
    done = gevent.event.Event()

    def on_headers(engine: wrapper.Engine, stream: wrapper.Stream, headers: wrapper.Headers, closed: bool):
        print("on headers", closed)

    def on_data(engine: wrapper.Engine, stream: wrapper.Stream, data: wrapper.Data, closed: bool):
        print("on data", closed)
        print(data.as_str())

    def on_metadata(engine: wrapper.Engine, stream: wrapper.Stream, metadata: wrapper.Headers):
        print("on metadata")

    def on_trailers(engine: wrapper.Engine, stream: wrapper.Stream, trailers: wrapper.Headers):
        print("on trailers")

    def on_error(engine: wrapper.Engine, code: int):
        print("on error")
        done.set()

    def on_complete(engine: wrapper.Engine, stream: wrapper.Stream):
        print("on complete")
        done.set()

    def on_cancel(engine: wrapper.Engine, stream: wrapper.Stream):
        print("on cancel")
        done.set()

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
    stream.send_headers(
        wrapper.Headers()
        .add(":authority", "www.google.com")
        .add(":method", "GET")
        .add(":path", "/")
        .add(":scheme", "https"),
        True,
    )

    done.wait()
    engine.terminate()


# TODO: this is never actually called, because we call terminate() from within on_engine_running and
# kill the Engine event loop
def on_exit(engine: wrapper.Engine):
    print("on_exit")


if __name__ == "__main__":
    faulthandler.enable()

    engine = wrapper.Engine()
    callbacks = wrapper.EngineCallbacks(engine).set_on_engine_running(on_engine_running).set_on_exit(on_exit)

    # note: this implicitly waits, maybe change that in the future to put the control into the
    # application layer :)
    engine.run(callbacks, EnvoyConfig().build(), "info")
    try:
        # TODO: come up with a better way to stop this thread than Ctrl+C
        while engine.running():
            gevent.sleep(0.05)  # TODO: come up with a better way to not hog CPU
            thunk = engine.get_thunk()
            if thunk is None:
                continue
            gevent.spawn(thunk, engine)
    except KeyboardInterrupt:
        engine.terminate()
