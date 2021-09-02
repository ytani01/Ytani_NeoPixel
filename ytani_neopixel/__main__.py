#
# (c) 2021 Yoichi Tanibayashi
#
"""
main for ytani_neopixel package
"""
import random
import time

from . import Ytani_NeoPixel, __prog_name__, __version__
from rpi_ws281x import Color
from .my_logger import get_logger

__author__ = 'Yoichi Tanibayashi'
__date__ = '2021/09'


class Test1App:
    """ Test1App """
    __log = get_logger(__name__, False)

    def __init__(self, led_num:int,
                 brightness:int, sleep_sec:float,
                 debug=False):
        """ Constructor

        Parameters
        ----------
        led_num: int
        """
        self._dbg = debug
        self.__log = get_logger(self.__class__.__name__, self._dbg)
        self.__log.debug('led_num=%d', led_num)
        self.__log.debug('brightness=%s', brightness)
        self.__log.debug('sleep_sec=%s', sleep_sec)

        self._led_num = led_num
        self._brightness = brightness
        self._sleep_sec = sleep_sec

        self._obj = Ytani_NeoPixel(led_n=self._led_num,
                                   brightness=self._brightness,
                                   debug=self._dbg)

    def main(self):
        """ main """
        self.__log.debug('start')
        self._obj.xfade_all(0xFFFFFF, n=10, interval=.1)
        time.sleep(self._sleep_sec)
        self._obj.xfade_all(0x000000, n=10, interval=.2)
        self.__log.debug('done')

    def end(self):
        """ end: Call at the end of usage """
        self.__log.debug('doing ..')
        self._obj.end()
        self.__log.debug('done')


class Test2App:
    """ Test2App """
    __log = get_logger(__name__, False)

    def __init__(self,
                 led_n:int=Ytani_NeoPixel.DEF_LED_N,
                 led_i:int=0,
                 color_r:int=255, color_g:int=255, color_b:int=255,
                 brightness:int=Ytani_NeoPixel.DEF_BRIGHTNESS,
                 debug=False):
        """ Constructor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('led_n, led_i=%s', (led_n, led_i))
        self.__log.debug('color_rgb=%s', (color_r, color_g, color_b))
        self.__log.debug('brightness=%s', brightness)

        self._led_n = led_n
        self._led_i = led_i
        self._color = Color(color_r, color_g, color_b)
        self._brightness = brightness

        self._obj = Ytani_NeoPixel(led_n=self._led_n,
                                   brightness=self._brightness,
                                   debug=self._dbg)

    def main(self):
        """ main """
        self.__log.debug('')

        self._obj.set(self._led_i, self._color)

        time.sleep(10)

    def end(self):
        """ end """
        self.__log.debug('')
        self._obj.end()


import click
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


@cli.command(help="""test progaram""")
@click.argument('led_num', type=int, nargs=1)
@click.option('--brightness', '-b', 'brightness', type=int,
              default=Ytani_NeoPixel.DEF_BRIGHTNESS,
              help='brightness')
@click.option('--sleep_sec', '-s', 'sleep_sec', type=float,
              default=1.0,
              help='sleep sec')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def test1(led_num, brightness, sleep_sec, debug):
    """ test1  """
    log = get_logger(__name__, debug)

    app = Test1App(led_num, brightness, sleep_sec, debug=debug)
    try:
        app.main()
    finally:
        log.debug('finally')
        app.end()
        log.info('end')


@cli.command(help="""test progaram""")
@click.argument('led_n', type=int, nargs=1)
@click.argument('led_i', type=int, nargs=1)
@click.argument('color_r', type=int, nargs=1)
@click.argument('color_g', type=int, nargs=1)
@click.argument('color_b', type=int, nargs=1)
@click.option('--brightness', '-b', 'brightness', type=int,
              default=Ytani_NeoPixel.DEF_BRIGHTNESS,
              help='brightness')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def test2(led_n, led_i, color_r, color_g, color_b, brightness, debug):
    """ test1  """
    log = get_logger(__name__, debug)

    app = Test2App(led_n, led_i, color_r, color_g, color_b, brightness,
                   debug=debug)
    try:
        app.main()
    finally:
        log.debug('finally')
        app.end()
        log.info('end')


if __name__ == '__main__':
    cli(prog_name=__prog_name__)
