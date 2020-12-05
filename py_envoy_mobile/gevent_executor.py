import os
import threading
from typing import Callable
from typing import TypeVar

import gevent
from gevent.pool import Group
from greenlet import greenlet

from py_envoy_mobile import wrapper  # type: ignore


T = TypeVar("T")


class ThreadsafeChannel:
    """
    gevent doesn't play well with multiple real threads. There is no documented canonical way to
    move work across threads, a la asyncio.loop.call_soon_threadsafe. Also, if the main thread is
    waiting on work from another thread, it will raise a LoopExit causing the program to exit.

    This class solves both of those problems by creating a thread-safe way to move data across
    threads that binds with gevent.
    """

    def __init__(self):
        self.hub = gevent.get_hub()
        self.watcher = self.hub.loop.async_()
        self.lock = threading.Lock()
        self.values: List[T] = []

    def put(self, value: T) -> None:
        """
        put enqueues a value and notifies and greenlets waiting on a value that one is available.
        """
        with self.lock:
            self.values.append(value)
            self.watcher.send()

    def get(self) -> T:
        """
        get retrieves an enqueued value. If none exists it waits until notified that such a value is
        available.
        """
        self.lock.acquire()
        while len(self.values) == 0:
            self.lock.release()
            self.hub.wait(self.watcher)
            self.lock.acquire()

        value: T = self.values.pop(0)
        self.lock.release()
        return value


class GeventExecutor(wrapper.ExecutorBase):
    def __init__(self):
        super().__init__()
        self.group = Group()
        self.channel: ThreadsafeChannel[Callable[[], None]] = ThreadsafeChannel()
        self.spawn_work_greenlet = gevent.spawn(self._spawn_work)

    def __del__(self):
        super().__del__()
        self.spawn_work_greenlet.kill()

    def execute(self, func: Callable[[], None]):
        self.channel.put(func)

    def _spawn_work(self):
        while True:
            self.group.spawn(self.channel.get())
