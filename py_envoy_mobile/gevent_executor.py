from typing import Callable

import gevent
from gevent.pool import Group

from py_envoy_mobile import wrapper  # type: ignore


class GeventExecutor(wrapper.ExecutorBase):
    def __init__(self):
        super().__init__()
        self.hub = gevent.get_hub()
        self.evil_trickery = gevent.spawn(self._evil_trickery)

    def execute(self, func: Callable[[], None]):
        self.hub.loop.run_callback(func)

    def _evil_trickery(self):
        # TODO: figure out a better way to inform gevent that the events it's waiting for will be
        # set, and it just needs to sit tight.
        while True:
            gevent.sleep(0.01)
