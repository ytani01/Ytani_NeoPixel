#
# (c) 2021 Yoichi Tanibayashi
#
"""
main for ytani_neopixel package
"""
import random
import time

from . import Ytani_NeoPixel, __prog_name__, __version__
from .my_logger import get_logger

__author__ = 'Yoichi Tanibayashi'
__date__ = '2021/09'


class Test1App:
    """ Test1App """
    __log = get_logger(__name__, False)

    def __init__(self, led_num, debug=False):
        """ Constructor

        Parameters
        ----------
        arg1: str
        """
        self._dbg = debug
        self.__log = get_logger(self.__class__.__name__, self._dbg)
        self.__log.debug('led_num=%d', led_num)

        self._led_num = led_num

        self._obj = Ytani_NeoPixel(led_n=self._led_num, debug=self._dbg)

    def main(self):
        """ main """
        self.__log.debug('start')


        self.__log.debug('done')

    def end(self):
        """ end: Call at the end of usage """
        self.__log.debug('doing ..')
        self._obj.xfade_all(0xFFFFFF, n=10, interval=1)
        self._obj.xfade_all(0x000000, n=10, interval=1)
        self._obj.end()
        self.__log.debug('done')


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
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def test1(led_num, debug):
    """ test1  """
    log = get_logger(__name__, debug)

    app = Test1App(led_num, debug=debug)
    try:
        app.main()
    finally:
        log.debug('finally')
        app.end()
        log.info('end')


if __name__ == '__main__':
    cli(prog_name=__prog_name__)
