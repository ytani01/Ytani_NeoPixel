#
# (c) 2021 Yoichi Tanibayashi
#
__author__ = 'Yoichi Tanibayashi'
__date__ = '2021/09'


import time
import click
from . import NeoPixel, __prog_name__, __version__
from . import RobotEye_Cirle7LEDs

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


@cli.command(help="""test progaram

    LED_I   index of Target LED

    COLOR   hex color code""")
@click.argument('led_i', type=int)
@click.argument('color', type=str)
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
def test1(led_n, led_i, color, brightness, sec, debug):
    """ test1  """

    pixel = NeoPixel(led_n=led_n, brightness=brightness,
                     debug=debug)

    try:
        pixel.set(led_i, int(color, 16))
        time.sleep(sec)

    finally:
        pixel.end()


@cli.command(help="""test progaram""")
@click.option('--led_num', '-n', 'led_num', type=int,
              default=NeoPixel.DEF_LED_N,
              help='number of LEDs (default: %s)' % NeoPixel.DEF_LED_N)
@click.option('--brightness', '-b', 'brightness', type=int,
              default=NeoPixel.DEF_BRIGHTNESS,
              help='brightness (default: %s)' % NeoPixel.DEF_BRIGHTNESS)
@click.option('--sleep_sec', '-s', 'sleep_sec', type=float,
              default=1.0,
              help='sleep sec (default: %s sec)' % 1.0)
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def test2(led_num, brightness, sleep_sec, debug):
    """ test2  """

    pixel = NeoPixel(led_n=led_num, brightness=brightness,
                     debug=debug)

    try:
        pixel.xfade_all(0xFFFFFF, n=10, interval=.1)
        time.sleep(sleep_sec)
        pixel.xfade_all(0x000000, n=10, interval=.1)

    finally:
        pixel.end()


@cli.command(help="""test progaram""")
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
@click.option('--clear', '-c', 'clear', is_flag=True, default=False,
              help='clear flag')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def test3(color, led_num, brightness, sleep_sec, xfade, clear, debug):
    """ test3 """

    pixel = NeoPixel(led_n=led_num, brightness=brightness,
                     debug=debug)

    try:
        c_int = [int(c, 16) for c in color]
        if xfade:
            pixel.xfade_all(c_int)
        else:
            pixel.set_all(c_int)
        time.sleep(sleep_sec)

    finally:
        if clear:
            if xfade:
                pixel.xfade_all(0)
            pixel.end()


@cli.command(help="""Robot eyes for cirle serial LED (7LEDs)""")
@click.option('--brightness', '-b', 'brightness', type=int,
              default=RobotEye_Cirle7LEDs.DEF_BRIGHTNESS,
              help='brightness (default: %s)' % (
                  RobotEye_Cirle7LEDs.DEF_BRIGHTNESS))
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
@click.version_option(version=__version__)
def robot_eye1(brightness, debug):
    """ robot_eye1 """

    robot_eye = RobotEye_Cirle7LEDs(brightness=brightness, debug=debug)

    try:
        robot_eye.start()
        robot_eye.join()

    finally:
        robot_eye.end()


if __name__ == '__main__':
    cli(prog_name=__prog_name__)
