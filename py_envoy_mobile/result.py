from enum import Enum
from typing import Dict

import attr


class Status(Enum):
    Unknown = 0
    Errored = 1
    Completed = 2
    Canceled = 3


@attr.s(auto_attribs=True)
class Result:
    headers: Dict[str, str]
    body: bytes
    metadata: Dict[str, str]
    trailers: Dict[str, str]
    status: Status
