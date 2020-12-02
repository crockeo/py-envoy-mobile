import asyncio
from typing import Callable
from typing import Dict

from py_envoy_mobile import wrapper  # type: ignore
from py_envoy_mobile.asyncio_result import AsyncResult
from py_envoy_mobile.result import Result
from py_envoy_mobile.result import Status


class Stream:
    def __init__(self, engine: wrapper.Engine):
        self.engine = engine
        self.stream = wrapper.Stream(engine)
        self.stream_callbacks = None

        self.headers_value: Dict[str, str] = {}
        self.headers_result: AsyncResult[Dict[str, str]] = AsyncResult()

        self.data_value = bytearray()
        self.data_result: AsyncResult[bytes] = AsyncResult()

        self.metadata_value: Dict[str, str] = {}
        self.metadata_result: AsyncResult[Dict[str, str]] = AsyncResult()

        self.trailers_value: Dict[str, str] = {}
        self.trailers_result: AsyncResult[Dict[str, str]] = AsyncResult()

        self.status_result: AsyncResult[Status] = AsyncResult()

    async def headers(self) -> Dict[str, str]:
        return await self.headers_result.get()

    async def data(self) -> bytes:
        return await self.data_result.get()

    async def metadata(self) -> Dict[str, str]:
        return await self.metadata_result.get()

    async def trailers(self) -> Dict[str, str]:
        return await self.trailers_result.get()  # type: ignore

    async def status(self) -> Status:
        return await self.status_result.get()  # type: ignore

    async def result(self) -> Result:
        return Result(
            headers=await self.headers(),
            body=await self.data(),
            metadata=await self.metadata(),
            trailers=await self.trailers(),
            status=await self.status(),
        )

    def start(self):
        def on_headers(engine: wrapper.Engine, stream: wrapper.Stream, headers: wrapper.Headers, closed: bool):
            for key, value in headers:
                self.headers_value[key] = value

        def on_data(engine: wrapper.Engine, stream: wrapper.Stream, data: wrapper.Data, closed: bool):
            self.data_value += data.as_bytes()

        def on_metadata(engine: wrapper.Engine, stream: wrapper.Stream, metadata: wrapper.Headers):
            for key, value in metadata:
                self.metadata_value[key] = value

        def on_trailers(engine: wrapper.Engine, stream: wrapper.Stream, trailers: wrapper.Headers):
            for key, value in trailers:
                self.trailers_value[key] = value

        def on_error(engine: wrapper.Engine, stream: wrapper.Stream, code: int, message: wrapper.Data, count: int):
            self._set_results(Status.Errored)

        def on_complete(engine: wrapper.Engine, stream: wrapper.Stream):
            self._set_results(Status.Completed)

        def on_cancel(engine: wrapper.Engine, stream: wrapper.Stream):
            self._set_results(Status.Canceled)

        self.stream_callbacks = (
            wrapper.StreamCallbacks(self.stream)
            .set_on_headers(on_headers)
            .set_on_data(on_data)
            .set_on_metadata(on_metadata)
            .set_on_trailers(on_trailers)
            .set_on_error(on_error)
            .set_on_complete(on_complete)
            .set_on_cancel(on_cancel)
        )
        self.stream.start(self.stream_callbacks)

    def send_headers(self, headers: Dict[str, str], closed: bool):
        envoy_headers = wrapper.Headers()
        for key, value in headers.items():
            envoy_headers.add(key, value)
        self.stream.send_headers(envoy_headers, closed)

    def send_data(self, body: bytes, closed: bool):
        envoy_body = wrapper.Data(body)
        self.stream.send_data(envoy_body, closed)

    def close(self):
        self.send_data(b"", True)

    def send_metadata(self, metadata: Dict[str, str]):
        envoy_metadata = wrapper.Headers()
        for key, value in metadata.items():
            envoy_metadata.add(key, value)
        self.stream.send_metadata(envoy_metadata)

    def send_trailers(self, trailers: Dict[str, str]):
        envoy_trailers = wrapper.Headers()
        for key, value in trailers.items():
            envoy_trailers.add(key, value)
        self.stream.send_trailers(envoy_trailers)

    def _set_results(self, status: Status):
        self.headers_result.set(self.headers_value)
        self.data_result.set(bytes(self.data_value))
        self.metadata_result.set(self.metadata_value)
        self.trailers_result.set(self.trailers_value)
        self.status_result.set(status)
