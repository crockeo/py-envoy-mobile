import asyncio
from typing import Callable
from typing import Generic
from typing import TypeVar

from py_envoy_mobile import wrapper  # type: ignore
from py_envoy_mobile.asyncio_executor import AsyncioExecutor
from py_envoy_mobile.asyncio_stream import Stream


class Engine:
    def __init__(self, config: str, log_level: str):
        self.on_engine_running_evt = asyncio.Event()
        self.on_exit_evt = asyncio.Event()

        def _on_engine_running(engine: wrapper.Engine):
            self.on_engine_running_evt.set()

        def _on_exit(engine: wrapper.Engine):
            self.on_exit_evt.set()

        self.engine = wrapper.Engine()
        self.engine_callbacks = (
            wrapper.EngineCallbacks(self.engine, AsyncioExecutor())
            .set_on_engine_running(_on_engine_running)
            .set_on_exit(_on_exit)
        )
        self.config = config
        self.log_level = log_level
        self.engine.run(self.engine_callbacks, self.config, self.log_level)

    async def get_stream(self) -> Stream:
        await self.on_engine_running_evt.wait()
        return Stream(self.engine)

    async def terminate(self):
        self.engine.terminate()
        await self.on_exit_evt.wait()
