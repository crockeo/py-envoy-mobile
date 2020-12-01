from enum import Enum
from typing import Dict
from typing import Optional

import attr
import gevent
from gevent.event import AsyncResult

from py_envoy_mobile import wrapper  # type: ignore
from py_envoy_mobile.gevent_stream import Stream
from py_envoy_mobile.result import Result


class EnvoyClient:
    """
    EnvoyClient provides a pythonic API atop the thin wrapper layer in py-envoy-mobile so that one
    can get the perf benefits without having to interact directly with a C-like API.

    This client assumes that the async library in use is gevent, but it will be revised to be
    agnostic.
    """

    def __init__(self, engine: wrapper.Engine):
        self.engine = engine

    def request(
        self,
        method: str,
        host: str,
        path: str,
        scheme: str = "https",
        headers: Optional[Dict[str, str]] = None,
        body: bytes = b"",
    ) -> Result:
        headers = headers or {}
        headers = {
            **headers,
            ":authority": host,
            ":method": method,
            ":path": path,
            ":scheme": scheme,
        }

        stream = Stream(self.engine)

        stream.start()
        stream.send_headers(headers, False)
        stream.send_data(body, False)
        stream.close()

        return stream.result()
