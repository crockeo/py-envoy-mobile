from abc import ABC
from abc import abstractmethod

import gevent
from gevent.event import Event
from gevent.pool import Group

from py_envoy_mobile import wrapper  # type: ignore
from py_envoy_mobile.gevent_stream import Stream


class Engine:
    def __init__(self, config: str, log_level: str):
        self.on_engine_running_evt = Event()
        self.on_exit_evt = Event()
        self.group = Group()

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
        self.group.spawn(self._run_engine)

    def get_stream(self):
        self.on_engine_running_evt.wait()
        return Stream(self.engine)

    def terminate(self):
        self.engine.terminate()
        self.group.join()

    def _run_engine(self):
        self.engine.run(self.engine_callbacks, self.config, self.log_level)
        while self.engine.running():
            gevent.sleep(0.05)  # TODO: come up with a better way to not hog CPU
            thunk = self.engine.get_thunk()
            if thunk is None:
                continue
            self.group.spawn(thunk, self.engine)