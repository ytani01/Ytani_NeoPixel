#
# (c) 2021 Yoichi Tanibayashi
#
__author__ = 'Yoichi Tanibayashi'
__date__ = '2021/09'


import time
import click
from cuilib import Cui
from . import __prog_name__, __version__
from . import NeoPixel
from . import robot_eye
from . import get_logger

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])


@click.group(invoke_without_command=True,
             context_settings=CONTEXT_SETTINGS, help="""
ytani-neopixel: version """ + __version__)
@click.version_option(version=__version__)
@click.pass_context
def cli(ctx):
    """ command group """
    subcmd = ctx.invoked_subcommand

    if subcmd is None:
        print(ctx.get_help())


@cli.command(help="""LED_I   index of Target LED""")
@click.argument('led_i', type=int)
@click.option('--color', '-c', 'color', type=str, default="0xffffff",
              help="color")
@click.option('--led_n', '-n', 'led_n', type=int, default=NeoPixel.DEF_LED_N,
              help='number of LEDs (default: %s)' % NeoPixel.DEF_LED_N)
@click.option('--brightness', '-b', 'brightness', type=int,
              default=NeoPixel.DEF_BRIGHTNESS,
              help='brightness (default: %s)' % NeoPixel.DEF_BRIGHTNESS)
@click.option('--sec', '-s', 'sec', type=float,
              default=3.0,
              help='seconds (default: %s)' % 3.0)
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def position(led_n, led_i, color, brightness, sec, debug):
    """ position  """

    pixel = NeoPixel(led_n=led_n, brightness=brightness,
                     debug=debug)

    try:
        pixel.set(led_i, int(color, 16))
        time.sleep(sec)

    finally:
        pixel.end()


@cli.command(help="""color pattern""")
@click.argument('color', type=str, nargs=-1)
@click.option('--led_num', '-n', 'led_num', type=int,
              default=NeoPixel.DEF_LED_N,
              help='number of LEDs (default: %s)' % NeoPixel.DEF_LED_N)
@click.option('--brightness', '-b', 'brightness', type=int,
              default=NeoPixel.DEF_BRIGHTNESS,
              help='brightness (default: %s)' % NeoPixel.DEF_BRIGHTNESS)
@click.option('--sleep_sec', '-s', 'sleep_sec', type=float,
              default=0.5,
              help='sleep sec (default: %s sec)' % 0.5)
@click.option('--xfade', '-x', 'xfade', is_flag=True, default=False,
              help='cross fade flag')
@click.option('--loop', '-l', 'loop', is_flag=True, default=False,
              help='shift and loop')
@click.option('--clear', '-c', 'clear', is_flag=True, default=False,
              help='clear flag')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def pattern(color, led_num, brightness, sleep_sec, xfade, loop, clear, debug):
    """ pattern """

    pixel = NeoPixel(led_n=led_num, brightness=brightness,
                     debug=debug)

    color = list(color)

    try:
        while True:
            c_int = [int(c, 16) for c in color]
            if xfade:
                pixel.xfade_all(c_int, n=5, interval=sleep_sec/5)
            else:
                pixel.set_all(c_int)
                time.sleep(sleep_sec)

            if loop:
                c1 = color.pop()
                color.insert(0, c1)
            else:
                break

    finally:
        if clear:
            if xfade:
                pixel.xfade_all(0)
            pixel.end()


@cli.command(help="""RGB test""")
@click.option('--pin', '-p', 'pin', type=int,
              default=NeoPixel.DEF_PIN,
              help='GPIO pin (default: %s)' % (NeoPixel.DEF_PIN))
@click.option('--led_num', '-n', 'led_num', type=int,
              default=NeoPixel.DEF_LED_N,
              help='number of LEDs (default: %s)' % NeoPixel.DEF_LED_N)
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def rgb(pin, led_num, debug):
    """ rgb_test """

    app = RGBtest(pin, led_num, debug=debug)

    try:
        app.main()

    finally:
        app.end()


class RGBtest:
    """ RGB test """
    __log = get_logger(__name__, False)

    def __init__(self, pin, led_n, debug=False):
        """ init """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('pin=%s, led_n=%s', pin, led_n)

        self._r = 0x80
        self._g = 0x80
        self._b = 0x80
        self._brightness = 16

        self._pixel = NeoPixel(pin=pin, led_n=led_n,
                               brightness=self._brightness,
                               debug=debug)

        self._cui = Cui(debug=debug)

        self._cui.add('hH?', self._cui.help, 'command help')
        self._cui.add(['KEY_ESCAPE', '\x04'], self._cui.end, 'quit')
        self._cui.add(['KEY_UP'], self.cmd_brightness_up,
                      'up brightness')
        self._cui.add(['KEY_DOWN'], self.cmd_brightness_down,
                      'down brightness')
        self._cui.add('qQr', self.cmd_r_up, 'up R')
        self._cui.add('aAR', self.cmd_r_down, 'down R')
        self._cui.add('wWg', self.cmd_g_up, 'up G')
        self._cui.add('sSG', self.cmd_g_down, 'down G')
        self._cui.add('eEb', self.cmd_b_up, 'up B')
        self._cui.add('dDB', self.cmd_b_down, 'down B')

    def main(self):
        """ main """
        self.__log.debug('')

        print('[ESC] to exit\n')

        self.update()
        self.print_status()

        self._cui.start()
        time.sleep(1)

        while self._cui.is_active():
            self.update()
            time.sleep(1)

    def end(self):
        """ end """
        self.__log.debug('')

        self._pixel.end()
        self._cui.end()
        self._cui.join()

    def update(self):
        """ get_color """
        self.__log.debug('')

        color = self.rgb2color(self._r, self._g, self._b)
        self._pixel.set_all(color)
        self._pixel.set_brightness(self._brightness)

    def rgb2color(self, r, g, b):
        """ rgb2color """
        self.__log.debug('RGB=#%02X%02X%02X', r, g, b)

        color = (r << 16) | (g << 8) | b
        self.__log.debug('color=0x%06X', color)

        return color

    def print_status(self):
        """ print_status """
        self.__log.debug('')

        print('#%02X%02X%02X, %d' % (
            self._r, self._g, self._b, self._brightness))

    def cmd_brightness_up(self, cmd):
        """ cmd_brightness_up """
        self.__log.debug('cmd=%s', cmd)

        self._brightness = min(self._brightness * 2, 255)
        self.update()
        self.print_status()

    def cmd_brightness_down(self, cmd):
        """ cmd_brightness_down """
        self.__log.debug('cmd=%s', cmd)

        self._brightness = round(max(self._brightness / 2, 1))
        self.update()
        self.print_status()

    def cmd_r_up(self, cmd):
        """ cmd_r_up """
        self.__log.debug('cmd=%s', cmd)

        d = 1
        if cmd == 'q':
            d = 0x10
        self._r = round(min(self._r + d, 0xff))
        self.update()
        self.print_status()

    def cmd_r_down(self, cmd):
        """ cmd_r_down """
        self.__log.debug('cmd=%s', cmd)

        d = 1
        if cmd == 'a':
            d = 0x10
        self._r = round(max(self._r - d, 0))
        self.update()
        self.print_status()

    def cmd_g_up(self, cmd):
        """ cmd_g_up """
        self.__log.debug('cmd=%s', cmd)

        d = 1
        if cmd == 'w':
            d = 0x10
        self._g = round(min(self._g + d, 0xff))
        self.update()
        self.print_status()

    def cmd_g_down(self, cmd):
        """ cmd_g_down """
        self.__log.debug('cmd=%s', cmd)

        d = 1
        if cmd == 's':
            d = 0x10
        self._g = round(max(self._g - d, 0))
        self.update()
        self.print_status()

    def cmd_b_up(self, cmd):
        """ cmd_b_up """
        self.__log.debug('cmd=%s', cmd)

        d = 1
        if cmd == 'e':
            d = 0x10
        self._b = round(min(self._b + d, 0xff))
        self.update()
        self.print_status()

    def cmd_b_down(self, cmd):
        """ cmd_b_down """
        self.__log.debug('cmd=%s', cmd)

        d = 1
        if cmd == 'd':
            d = 0x10
        self._b = round(max(self._b - d, 0))
        self.update()
        self.print_status()


cli.add_command(robot_eye)


if __name__ == '__main__':
    cli(prog_name=__prog_name__)
