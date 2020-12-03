import os
from typing import Callable

import gevent
from gevent.pool import Group
from greenlet import greenlet

from py_envoy_mobile import wrapper  # type: ignore


class GeventExecutor(wrapper.ExecutorBase):
    """
    There are a couple of weird things about this class that aren't particularly well documented:

      - To construct a greenlet from a non-main thread and attach it to a hub on the main thread,
        one needs to directly use a greenlet.greenlet. See:
        https://github.com/gevent/gevent/issues/931#issuecomment-274117169
        This still isn't thread-safe, but it ought not fail in practice because of the GIL.

      - To prevent spurious LoopExits from gevent because of the event loop emptying on the main
        thread, we provide a greenlet _lazy_cycle to loop once every time a callbck is called. This
        is super hacky, and I hope I learn how to do it right before this sees the light of day.
    """

    def __init__(self):
        super().__init__()
        self.hub = gevent.get_hub()
        self.watcher = self.hub.loop.async_()
        self.lazy_cycle = gevent.spawn(self._lazy_cycle)

    def __del__(self):
        self.watcher.close()  # need to close, or it leaks resources
        gevent.kill(self._start_on_main)  # need to kill, or we leak greenlets
        super().__del__(self)

    def execute(self, func: Callable[[], None]):
        g = greenlet(func, self.hub)
        self.hub.loop.run_callback(g.switch)
        self.watcher.send()

    def _lazy_cycle(self):
        self.watcher.start(self._lazy_cycle)
