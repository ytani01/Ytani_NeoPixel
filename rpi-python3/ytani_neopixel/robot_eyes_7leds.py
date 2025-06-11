#
# (c) 2021 Yoichi tanibayashi
#
import threading
import queue
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

    DEF_XFADE_N = 5
    DEF_XFADE_SEC = 0.05

    DEF_LED_N = 14

    DEF_COLOR = 0x00ffff
    DEF_BG_COLOR = 0x101010

    DEF_MOVE_MODE = 'random'

    DEF_SPEED = 5

    COL = [
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xff0000,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x00ff00,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0x0000ff,
        0xffff00,
        0xffff00,
        0xffff00,
        0xffff00,
        0x00ffff,
        0x00ffff,
        0x00ffff,
        0x00ffff,
        0x00ffff,
        0x00ffff,
        0x00ffff,
        0x00ffff,
        0xff00ff,
        0xff00ff,
        0xff00ff,
        0xff00ff,
        0xff3040,
        0xff3040,
        0xff3040,
        0xff3040,
        0xff3000
    ]

    NEXT_POS_LIST = [
        [0, 0, 0, 1, 2, 3, 4, 5, 6],  # 0
        [0, 0, 1, 1, 1, 2, 6],  # 1
        [0, 0, 1, 2, 2, 2, 3],  # 2
        [0, 0, 2, 3, 3, 3, 4],  # 3
        [0, 0, 3, 4, 4, 4, 5],  # 4
        [0, 0, 4, 5, 5, 5, 6],  # 5
        [0, 0, 1, 5, 6, 6, 6],  # 6
    ]

    __log = get_logger(__name__, False)

    def __init__(self, xfade_n=DEF_XFADE_N, xfade_sec=DEF_XFADE_SEC,
                 pin=NeoPixel.DEF_PIN, led_n=NeoPixel.DEF_LED_N,
                 color=DEF_COLOR, bg_color=DEF_BG_COLOR,
                 move_mode=DEF_MOVE_MODE,
                 speed=DEF_SPEED,
                 brightness=NeoPixel.DEF_BRIGHTNESS,
                 offset=0,
                 debug=False):
        """ constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('xfade: (n, sec)=%s', (xfade_n, xfade_sec))

        self._xfade_n = xfade_n
        self._xfade_sec = xfade_sec
        self._pin = pin
        self._led_n = led_n
        self._color = color
        self._bg_color = bg_color
        self._move = move_mode
        self._speed = speed
        self._brightness = brightness
        self._offset = offset

        self._pixel = NeoPixel(self._pin, self._led_n, self._brightness)
        self._cmdq = queue.Queue()

        self._cur_pos = 0
        self._pos_n = len(self.NEXT_POS_LIST)

        self._active = False

        self._pixel.set_all(0)

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

    def send(self, cmd):
        """ send """
        self.__log.debug('cmd=%s', cmd)
        self._cmdq.put(cmd)

    def run(self):
        """ run """
        self.__log.debug('')

        col_i = 0
        self._active = True
        while self._active:
            try:
                cmdline = self._cmdq.get_nowait()
            except queue.Empty:
                cmdline = None
            except Exeption as ex:
                self.__log.warning('%s:%s', type(ex).__name__, ex)

            if cmdline is not None:
                self.__log.debug('cmdline=%s', cmdline)
                cmd = cmdline.split()
                self.__log.debug('cmd=%s', cmd)
                
                if cmd[0] == 'color':
                    self._color = int(cmd[1], 16)

                if cmd[0] == 'bg_color':
                    self._bg_color = int(cmd[1], 16)

                if cmd[0] == 'move':
                    self._move = cmd[1]

            # move mode
            if self._move in ['stop', 'pause']:
                pass

            if len(self._move) == 1 and self._move in '0123456':
                self._cur_pos = int(self._move)

            if self._move in ['random', 'rand']:
                next_pos_list = self.NEXT_POS_LIST[self._cur_pos]
                next_pos_i = random.randrange(len(next_pos_list))
                self._cur_pos = next_pos_list[next_pos_i]

            if self._move in ['forward', 'fw']:
                if self._cur_pos == 0:
                    self._cur_pos = random.randrange(6) + 1
                else:
                    self._cur_pos += 1
                    if self._cur_pos > 6:
                        self._cur_pos = 1

            if self._move in ['backward', 'bw']:
                if self._cur_pos == 0:
                    self._cur_pos = random.randrange(6) + 1
                else:
                    self._cur_pos -= 1
                    if self._cur_pos < 1:
                        self._cur_pos = 6

            # color
            if self._color < 0:
                if random.random() < 0.2:
                    col_i = int(random.randrange(len(self.COL)))
                # self.__log.debug('col_i=%s', col_i)
                eye_color = self.COL[col_i]
            else:
                eye_color = self._color

            led_i = (self._cur_pos + self._offset + 7) % 7
            color = [self._bg_color] * self._pos_n
            color[led_i] = eye_color

            # show
            self._pixel.xfade_all(color,
                                  n=self._xfade_n,
                                  interval=self._xfade_sec)

            self.__log.debug('[%s] #%06X #%06X #%06X',
                             self._cur_pos, eye_color,
                             self._color, self._bg_color)

            # sleep
            if self._speed == 0:
                self._speed = self.DEF_SPEED

            interval_sec = 1 / self._speed
            time.sleep(interval_sec)

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
@click.option('--color', '-c', 'color', type=str, default='-1',
              help='color')
@click.option('--bg_color', '-bg', 'bg_color', type=str, default='101010',
              help='background color')
@click.option('--move_mode', '-m', 'move_mode', type=str, default='random',
              help='move mode')
@click.option('--speed', '-s', 'speed', type=float, default=1,
              help='speed')
@click.option('--brightness', '-bl', 'brightness', type=int,
              default=NeoPixel.DEF_BRIGHTNESS,
              help='brightness (default: %s)' % (NeoPixel.DEF_BRIGHTNESS))
@click.option('--offset', '-o', 'offset', type=int, default=0,
              help='offset of led index')
@click.option('--interactive', '-i', 'interactive', is_flag=True,
              default=False, help='interactive mode')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
def robot_eye(xfade_n, xfade_sec, pin, led_n, color, bg_color,
              move_mode, speed,
              brightness, offset, interactive, debug):
    """ robot_eye """

    obj = RobotEyes_Circle7LEDs(xfade_n=xfade_n,
                                xfade_sec=xfade_sec,
                                pin=pin, led_n=led_n,
                                color=int(color, 16),
                                bg_color=int(bg_color, 16),
                                move_mode=move_mode,
                                speed=speed,
                                brightness=brightness,
                                offset=offset,
                                debug=debug)

    try:
        obj.start()

        if interactive:
            while True:
                cmdline = input('> ')
                print('cmdline=%a' % cmdline)
                if cmdline.split()[0] in ['exit', 'quit']:
                    break

                obj.send(cmdline)

        else:
            obj.join()

    finally:
        obj.end()
