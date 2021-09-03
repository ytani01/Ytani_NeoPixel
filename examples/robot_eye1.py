#!/usr/bin/env python3

# *  cirle serial LED (7 LEDs) x 2
#
#     [02]  [03]         [09]  [10]
#   [01] [00]  [04]    [08] [07]  [11]
#     [06]  [05]         [13]  [12]

from ytani_neopixel import NeoPixel
from my_logger import get_logger
import threading
import queue
import time


class RobotEye(threading.Thread):
    """ Rboto Eye """
    DEF_PIN = 10    # SPI
    DEF_LED_N = 14
    DEF_BRIGHTNESS = 15

    CMD_NULL = 'NULL'
    CMD_END = 'END'
    CMD_SEARCHING = 'SEARCHING'
    CMD_CENTER = 'CENTER'
    CMD_BIG = 'BIG'
    CMD_LEFT = 'LEFT'
    CMD_RIGHT = 'RIGHT'

    __log = get_logger(__name__, False)

    def __init__(self, pin=DEF_PIN, led_n=DEF_LED_N,
                 brightness=DEF_BRIGHTNESS,
                 debug=False):
        """ constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)

        self._pin = pin
        self._led_n = led_n
        self._brightness = brightness

        self._pixel = NeoPixel(self._pin, self._led_n, self._brightness,
                               debug=self._dbg)

        self._cmdq = queue.Queue()

        self._mode_center = RobotEye_Mode_Center(
            self._pixel, debug=self._dbg)
        self._mode_big = RobotEye_Mode_Big(
            self._pixel, debug=self._dbg)
        self._mode_searching = RobotEye_Mode_Searching(
            self._pixel, loop_delay=0.3, xfade=True, debug=self._dbg)

        self._cur_mode = self._mode_center

        self._active = False

        super().__init__(daemon=True)

    def end(self, clear=True):
        """ end """
        self.__log.debug('active=%s', self._active)

        if self._active:
            self.send(self.CMD_END)
            self.join()

        if clear:
            self._pixel.end()

        self.__log.debug('done')

    def send(self, cmd):
        """ send """
        self.__log.debug('cmd=%s', cmd)

        self._cmdq.put(cmd)

    def exec_cmd(self, cmd):
        """ exec """
        self.__log.debug('cmd=%s', cmd)

        cmdline = cmd.split()
        self.__log.debug('cmdline=%s', cmdline)

        if len(cmdline) == 0:
            cmdline.append(self.CMD_NULL)
            return True

        if cmdline[0] == self.CMD_END:
            return False

        if cmdline[0] == self.CMD_CENTER:
            self._cur_mode = self._mode_center

        if cmdline[0] == self.CMD_BIG:
            self._cur_mode = self._mode_big

        if cmdline[0] == self.CMD_SEARCHING:
            self._cur_mode = self._mode_searching

        self._pattern_i = 0

        self.__log.warning('ignore: cmdline=%s', cmdline)
        return True

    def run(self):
        """ run """
        self.__log.debug('')

        self._active = True
        while self._active:
            try:
                cmd = self._cmdq.get(block=False)
                self.__log.debug('cmd=%s', cmd)

            except queue.Empty:
                cmd = None
                self._cur_mode.loop()

            if cmd is not None:
                self._active = self.exec_cmd(cmd)

        self.__log.info('done(active=%s)', self._active)


class RobotEye_Mode_base:
    """ mode base """

    PATTERN = [[0]]

    DEF_LOOP_DELAY = 0.5  # sec (should be overriden in sub class)

    __log = get_logger(__name__, False)

    def __init__(self, pixel,
                 loop_delay=DEF_LOOP_DELAY, xfade=False,
                 debug=False):
        """ constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('loop_delay=%s, xfade=%s', loop_delay, xfade)

        self._pixel = pixel
        self._loop_delay = loop_delay
        self._xfade = xfade

        if self._loop_delay is None:
            self._loop_delay = self.DEF_LOOP_DELAY

        self._pattern_n = len(self.PATTERN)
        self.__log.debug('pattern_n=%s', self._pattern_n)

        self._pattern_i = 0
        self._count = 0

    def loop(self):
        """ loop_base

        !!! shoud be overiden in sub class !!!

        """
        self.__log.debug('pattern:%s', self.PATTERN[self._pattern_i])
        if self._xfade:
            self._pixel.xfade_all(self.PATTERN[self._pattern_i], n=5, interval=0.05)
        else:
            self._pixel.set_all(self.PATTERN[self._pattern_i])

        self._count += 1
        self._pattern_i = self._count % self._pattern_n

        self.__log.debug('count=%d, pattern_i=%d',
                         self._count, self._pattern_i)

        time.sleep(self._loop_delay)


class RobotEye_Mode_Center(RobotEye_Mode_base):
    """ Mode: Center """

    PATTERN = [[0x0000FF, 0, 0, 0, 0, 0, 0]]


class RobotEye_Mode_Big(RobotEye_Mode_base):
    """ Mode: Center """

    PATTERN = [[0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00,
                0x00FF00, 0x00FF00, 0x00FF00]]


class RobotEye_Mode_Searching(RobotEye_Mode_base):
    """ Mode: Searching """

    PATTERN = [
        [0xFF0000, 0, 0, 0, 0, 0, 0],
        [0, 0xFF0000, 0, 0, 0, 0, 0],
        [0xFF0000, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0xFF0000, 0, 0]
        ]


import cuilib


class SampleApp:
    """ Sample """

    __log = get_logger(__name__, False)

    def __init__(self, debug=False):
        """ constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)

        self._obj = RobotEye(debug=self._dbg)
        self._cui = cuilib.Cui(debug=self._dbg)

        self._cui.add('hH?', self.cmd_help, 'command help')
        self._cui.add(['q', 'Q', 'KEY_ESCAPE', '\x04'], self.cmd_quit, 'quit')
        self._cui.add('cC', self.cmd_center, 'center')
        self._cui.add('bB', self.cmd_big, 'big')
        self._cui.add('sS', self.cmd_searching, 'searching')

    def main(self):
        """ main """
        self.__log.debug('')

        self._obj.start()
        self._cui.start()
        self._cui.join()

    def end(self):
        """ end """
        self.__log.debug('')
        self._obj.end()
        self.__log.debug('done')

    def cmd_help(self, key_sym):
        self._cui.help(True)

    def cmd_quit(self, key_sym):
        self._cui.end()

    def cmd_center(self, key_sym):
        self._obj.send(RobotEye.CMD_CENTER)

    def cmd_searching(self, key_sym):
        self._obj.send(RobotEye.CMD_SEARCHING)

    def cmd_big(self, key_sym):
        self._obj.send(RobotEye.CMD_BIG)


import click
CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])


@click.command(context_settings=CONTEXT_SETTINGS, help='''
RobotEye sample''')
@click.option('--debug', '-d', 'debug', is_flag=True,
              default=False,
              help="debug flag")
def main(debug):
    """ main """
    __log = get_logger(__name__, debug)

    app = SampleApp(debug=debug)
    try:
        app.main()
    finally:
        __log.debug('finally')
        app.end()


if __name__ == '__main__':
    main()
