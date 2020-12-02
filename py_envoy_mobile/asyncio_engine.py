import asyncio
from typing import Callable
from typing import Generic
from typing import TypeVar

from py_envoy_mobile import wrapper  # type: ignore
from py_envoy_mobile.asyncio_stream import Stream


class Engine:
    def __init__(self, config: str, log_level: str):
        self.on_engine_running_evt = asyncio.Event()
        self.on_exit_evt = asyncio.Event()
        self.tasks = []

        def _on_engine_running(engine: wrapper.Engine):
            self.on_engine_running_evt.set()

        def _on_exit(engine: wrapper.Engine):
            self.on_exit_evt.set()

        self.engine = wrapper.Engine()
        self.engine_callbacks = (
            wrapper.EngineCallbacks(self.engine).set_on_engine_running(_on_engine_running).set_on_exit(_on_exit)
        )
        self.config = config
        self.log_level = log_level
        self.tasks.append(asyncio.create_task(self._run_engine()))

    async def get_stream(self) -> Stream:
        await self.on_engine_running_evt.wait()
        return Stream(self.engine)

    async def terminate(self):
        self.engine.terminate()
        await asyncio.gather(self.tasks)
        await self.on_exit_evt.wait()

    async def _run_engine(self):
        # TODO: make this....not halt forever while blocking SIGINT :)
        self.engine.run(self.engine_callbacks, self.config, self.log_level)
        while self.engine.running():
            thunk = self.engine.get_thunk()
            self.tasks.append(asyncio.create_task(self._as_coroutine(thunk)))

    def _as_coroutine(self, func: Callable[[wrapper.Engine], None]):
        async def _as_coroutine_impl():
            func(self.engine)

        return _as_coroutine_impl()
