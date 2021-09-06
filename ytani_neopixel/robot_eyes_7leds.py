#
# (c) 2021 Yoichi tanibayashi
#
import threading
import time
import random

from .ytani_neopixel import NeoPixel
from .my_logger import get_logger


class RobotEyes_Cirle7LEDs(threading.Thread):
    """ Robot eyes for cirle serial LED (7 LEDs) x 2

        [02]  [03]         [09]  [10]
      [01] [00]  [04]    [08] [07]  [11]
        [06]  [05]         [13]  [12]
"""

    NEXT_POS_LIST = [
        [0, 0, 0, 1, 2, 3, 4, 5, 6],
        [0, 1, 1, 1, 2, 6],
        [0, 1, 2, 2, 2, 3],
        [0, 2, 3, 3, 3, 4],
        [0, 3, 4, 4, 4, 5],
        [0, 4, 5, 5, 5, 6],
        [0, 1, 5, 6, 6, 6],
        ]

    __log = get_logger(__name__, False)

    def __init__(self, pin=NeoPixel.DEF_PIN, led_n=NeoPixel.DEF_LED_N,
                 brightness=NeoPixel.DEF_BRIGHTNESS,
                 debug=False):
        """ constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)

        self._pin = pin
        self._led_n = led_n
        self._brightness = brightness

        self._pixel = NeoPixel(self._pin, self._led_n, self._brightness)

        self._cur_pos = 0
        self._prev_pos = 0
        self._pos_n = len(self.NEXT_POS_LIST)

        self._active = False

        super().__init__(daemon=True)

    def end(self, clear=True):
        """ end """
        self.__log.debug('active=%s', self._active)

        if self._active:
            self._active = False
            self.join()

        if clear:
            self._pixel.end()

        self.__log.debug('done')

    def run(self):
        """ run """
        self.__log.debug('')

        self._active = True
        while self._active:
            self._prev_pos = self._cur_pos

            next_pos_list = self.NEXT_POS_LIST[self._cur_pos]
            next_pos_i = random.randrange(len(next_pos_list))

            self._cur_pos = next_pos_list[next_pos_i]

            eye_color = random.randrange(0x1000000)

            self.__log.debug('cur_pos=%s, eye_color=#%06X',
                             self._cur_pos, eye_color)

            color = [0] * self._pos_n
            color[self._cur_pos] = eye_color

            self._pixel.xfade_all(color, n=2, interval=0.1)

            sleep_sec = random.random()
            self.__log.debug('sleep_sec=%s', sleep_sec)
            time.sleep(random.random())

        self.__log.info('done(active=%s)', self._active)
