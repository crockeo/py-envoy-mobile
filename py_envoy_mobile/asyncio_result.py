from asyncio import Event
from typing import cast
from typing import Generic
from typing import TypeVar


T = TypeVar("T")


class AsyncResult(Generic[T]):
    def __init__(self):
        self.value: Optional[T] = None
        self.value_evt = Event()

    def set(self, value: T):
        self.value = value
        self.value_evt.set()

    async def get(self) -> T:
        await self.value_evt.wait()
        assert self.value != None
        return cast(T, self.value)
