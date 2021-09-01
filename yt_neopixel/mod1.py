#
# (c) 2021 Yoichi Tanibayasih
#
from .my_logger import get_logger


class MyClass1:
    """ """
    def __init__(self, param1, debug=False):
        self._dbg = debug
        self._log = get_logger(self.__class__.__name__, self._dbg)
        self._log.debug('param1=%s', param1)

        self._param1 = param1

    def func1(self, param1):
        self._log.debug('param1=%s', param1)

        return (self._param1, param1)

    def end(self):
        self._log.debug('')
