#
# (c) 2021 Yoichi tanibayashi
#
import threading
import time
import random
import click

from .ytani_neopixel import NeoPixel
from .my_logger import get_logger


class RobotEyes_Circle7LEDs(threading.Thread):
    """ Robot eyes for circle serial LED (7 LEDs) x 2

        [02]  [03]         [09]  [10]
      [01] [00]  [04]    [08] [07]  [11]
        [06]  [05]         [13]  [12]
"""

    DEF_XFADE_N = 3
    DEF_XFADE_SEC = 0.05

    DEF_LED_N = 14

    NEXT_POS_LIST = [
        [0, 0, 0, 1, 2, 3, 4, 5, 6],
        [0, 0, 1, 1, 1, 2, 6],
        [0, 0, 1, 2, 2, 2, 3],
        [0, 0, 2, 3, 3, 3, 4],
        [0, 0, 3, 4, 4, 4, 5],
        [0, 0, 4, 5, 5, 5, 6],
        [0, 0, 1, 5, 6, 6, 6],
        ]

    __log = get_logger(__name__, False)

    def __init__(self, xfade_n=DEF_XFADE_N, xfade_sec=DEF_XFADE_SEC,
                 pin=NeoPixel.DEF_PIN, led_n=NeoPixel.DEF_LED_N,
                 bg_flag=False,
                 brightness=NeoPixel.DEF_BRIGHTNESS,
                 offset=0,
                 debug=False):
        """ constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('xfade: (n, sec)=%s', (xfade_n, xfade_sec))
        self.__log.debug('bg_flag=%s', bg_flag)

        self._xfade_n = xfade_n
        self._xfade_sec = xfade_sec
        self._pin = pin
        self._led_n = led_n
        self._bg_flag = bg_flag
        self._brightness = brightness
        self._offset = offset

        self._pixel = NeoPixel(self._pin, self._led_n, self._brightness)

        self._cur_pos = 0
        self._prev_pos = 0
        self._pos_n = len(self.NEXT_POS_LIST)

        self._active = False

        self._pixel.set_all(0)
        init_color = [0] * 7
        led_i = offset % 7
        init_color[led_i] = 0x111111
        self._pixel.xfade_all(init_color, n=20, interval=0.1)
        init_color[led_i] = 0xffffff
        self._pixel.xfade_all(init_color, n=20, interval=0.1)
        time.sleep(1)

        super().__init__(daemon=True)

    def end(self, clear=True):
        """ end """
        self.__log.debug('active=%s', self._active)

        if self._active:
            self._active = False
            self.join()

        if clear:
            self.__log.debug('clear')
            self._pixel.end()

        self.__log.debug('done')

    def run(self):
        """ run """
        self.__log.debug('')

        eye_color = random.randrange(0xffffff)
        eye_bg_color = 0
        self._active = True
        while self._active:
            self._prev_pos = self._cur_pos

            next_pos_list = self.NEXT_POS_LIST[self._cur_pos]
            next_pos_i = (random.randrange(len(next_pos_list)) + self._offset)

            led_i = next_pos_i % 7
            self._cur_pos = next_pos_list[next_pos_i]

            if random.random() < 0.3:
                eye_color = random.randrange(0x1000000)
                eye_bg_color = 0
                if self._bg_flag:
                    eye_bg_color = random.randrange(0x1000000)

            color = [eye_bg_color] * self._pos_n

            led_i = (self._cur_pos + self._offset) % 7
            color[led_i] = eye_color

            self._pixel.xfade_all(color,
                                  n=self._xfade_n,
                                  interval=self._xfade_sec)

            sleep_sec = random.random() * 2

            self.__log.debug('[%s] #%06X #%06X %.2f sec',
                             self._cur_pos,
                             eye_color, eye_bg_color,
                             sleep_sec)

            time.sleep(random.random())

        self.__log.info('done(active=%s)', self._active)


@click.command(help="""Robot eyes for circle serial LED (7LEDs)""")
@click.option('--xfade_n', '--xn', '-xn', 'xfade_n', type=int,
              default=RobotEyes_Circle7LEDs.DEF_XFADE_N,
              help='cross-fade count (default: %s)' % (
                  RobotEyes_Circle7LEDs.DEF_XFADE_N))
@click.option('--xfade_sec', '--xs', '-xs', 'xfade_sec', type=float,
              default=RobotEyes_Circle7LEDs.DEF_XFADE_SEC,
              help='cross-fade interval sec (default: %s)' % (
                  RobotEyes_Circle7LEDs.DEF_XFADE_SEC))
@click.option('--pin', '-p', 'pin', type=int,
              default=NeoPixel.DEF_PIN,
              help='GPIO pin (default: %s)' % (NeoPixel.DEF_PIN))
@click.option('--led_n', '-n', 'led_n', type=int,
              default=RobotEyes_Circle7LEDs.DEF_LED_N,
              help='number of LEDs (default: %s)' % (
                  RobotEyes_Circle7LEDs.DEF_LED_N))
@click.option('--bg_flag', '-bg', 'bg_flag', is_flag=True, default=False,
              help='background color flag')
@click.option('--brightness', '-bl', 'brightness', type=int,
              default=NeoPixel.DEF_BRIGHTNESS,
              help='brightness (default: %s)' % (NeoPixel.DEF_BRIGHTNESS))
@click.option('--offset', '-o', 'offset', type=int, default=0,
              help='offset of led index')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
def robot_eye(xfade_n, xfade_sec, pin, led_n, bg_flag, brightness,
              offset, debug):
    """ robot_eye """

    obj = RobotEyes_Circle7LEDs(xfade_n=xfade_n, xfade_sec=xfade_sec,
                                pin=pin, led_n=led_n, bg_flag=bg_flag,
                                brightness=brightness,
                                offset=offset,
                                debug=debug)

    try:
        obj.start()
        obj.join()

    finally:
        obj.end()
