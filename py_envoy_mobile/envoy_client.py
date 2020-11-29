from enum import Enum
from typing import Dict
from typing import Optional

import attr
import gevent
from gevent.event import AsyncResult

from py_envoy_mobile import wrapper  # type: ignore


class EnvoyClientStatus(Enum):
    Unknown = 0
    Errored = 1
    Completed = 2
    Canceled = 3


@attr.s(auto_attribs=True)
class EnvoyClientResult:
    headers: Dict[str, str]
    body: bytes
    metadata: Dict[str, str]
    trailers: Dict[str, str]
    status: EnvoyClientStatus


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
    ) -> EnvoyClientResult:
        headers = headers or {}
        headers = {
            **headers,
            ":authority": host,
            ":method": method,
            ":path": path,
            ":scheme": scheme,
        }

        stream = wrapper.Stream(self.engine)

        result = EnvoyClientResult(
            headers={},
            body=b"",
            metadata={},
            trailers={},
            status=EnvoyClientStatus.Unknown,
        )
        result_body = bytearray()
        result_status = AsyncResult()

        def on_headers(engine: wrapper.Engine, stream: wrapper.Stream, headers: wrapper.Headers, closed: bool):
            for key, value in headers:
                result.headers[key] = value

        def on_data(engine: wrapper.Engine, stream: wrapper.Stream, data: wrapper.Data, closed: bool):
            result_body.extend(data.as_bytes())

        def on_metadata(engine: wrapper.Engine, stream: wrapper.Stream, metadata: wrapper.Headers):
            for key, value in metadata:
                result.metadata[key] = value

        def on_trailers(engine: wrapper.Engine, stream: wrapper.Stream, trailers: wrapper.Headers):
            for key, value in trailers:
                result.trailers[key] = value

        def on_error(engine: wrapper.Engine, stream: wrapper.Stream, code: int, message: wrapper.Data, count: int):
            result_status.set(EnvoyClientStatus.Errored)

        def on_complete(engine: wrapper.Engine, stream: wrapper.Stream):
            result_status.set(EnvoyClientStatus.Completed)

        def on_cancel(engine: wrapper.Engine, stream: wrapper.Stream):
            result_status.set(EnvoyClientStatus.Canceled)

        stream_callbacks = (
            wrapper.StreamCallbacks(stream)
            .set_on_headers(on_headers)
            .set_on_data(on_data)
            .set_on_metadata(on_metadata)
            .set_on_trailers(on_trailers)
            .set_on_error(on_error)
            .set_on_complete(on_complete)
            .set_on_cancel(on_cancel)
        )

        stream.start(stream_callbacks)

        envoy_headers = wrapper.Headers()
        for key, value in headers.items():
            envoy_headers.add(key, value)
        stream.send_headers(envoy_headers, False)

        stream.send_data(wrapper.Data(body), True)

        result.status = result_status.get()
        result.body = bytes(result_body)

        return result
