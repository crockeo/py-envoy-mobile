import asyncio
from typing import Callable

from py_envoy_mobile import wrapper  # type: ignore


class AsyncioExecutor(wrapper.ExecutorBase):
    def __init__(self):
        super().__init__()
        self.loop = asyncio.get_running_loop()

    def execute(self, func: Callable[[], None]):
        self.loop_call_soon_threadsafe(func)
