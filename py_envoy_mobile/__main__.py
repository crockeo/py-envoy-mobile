import asyncio
import faulthandler
import sys
import traceback
from typing import Any

from py_envoy_mobile import envoy_client
from py_envoy_mobile import wrapper  # type: ignore
from py_envoy_mobile.asyncio_engine import Engine as AsyncioEngine
from py_envoy_mobile.gevent_engine import Engine as GeventEngine


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


def gevent_main():
    engine = GeventEngine(EnvoyConfig().build(), "info")
    stream = engine.get_stream()
    stream.start()
    stream.send_headers(
        {
            ":authority": "www.google.com",
            ":method": "GET",
            ":path": "/",
            ":scheme": "https",
        },
        False,
    )
    stream.close()
    print(stream.result())
    engine.terminate()  # this is synchronous
    engine.join()


async def asyncio_main():
    engine = AsyncioEngine(EnvoyConfig().build(), "info")
    stream = await engine.get_stream()
    stream.start()
    stream.send_headers(
        {
            ":authority": "www.google.com",
            ":method": "GET",
            ":path": "/",
            ":scheme": "https",
        },
        False,
    )
    stream.close()
    result = await stream.result()
    print(result)
    await engine.terminate()


if __name__ == "__main__":
    faulthandler.enable()

    if len(sys.argv) < 2:
        gevent_main()
    elif sys.argv[1] == "gevent":
        gevent_main()
    elif sys.argv[1] == "asyncio":
        asyncio.run(asyncio_main())
    else:
        raise ValueError(f"unknown mode {sys.argv[1]}")
