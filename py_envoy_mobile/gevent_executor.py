from typing import Callable

import gevent
from gevent.pool import Group

from py_envoy_mobile import wrapper  # type: ignore


class GeventExecutor(wrapper.ExecutorBase):
    def __init__(self):
        super().__init__()
        self.group = Group()

    def execute(self, func: Callable[[], None]):
        # TODO: does this spawn on the thread it's calling from, or the thread of the hub to which
        # the group belongs?
        self.group.spawn(func)
