#
# (c) 2021 Yoichi Tanibayashi
#
"""
main for yt_neopixel package
"""
import click
from . import MyClass1, __prog_name__
from .my_logger import get_logger

__author__ = 'Yoichi Tanibayashi'
__date__ = '2021/09'


class Cmd1App:
    """ Cmd1App """
    def __init__(self, arg1, opt1, debug=False):
        """ Constructor

        Parameters
        ----------
        arg1: str
        opt1: str
        """
        self._dbg = debug
        self._log = get_logger(self.__class__.__name__, self._dbg)

        self._arg1 = arg1
        self._opt1 = opt1

        self._obj = MyClass1(self._arg1, debug=self._dbg)

    def main(self):
        """ main """
        self._log.debug('start')

        ret = self._obj.func1(self._opt1)
        print('ret =', ret)

        self._log.debug('done')

    def end(self):
        """ end: Call at the end of usage """
        self._log.debug('doing ..')
        self._obj.end()
        self._log.debug('done')


CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])


@click.group(invoke_without_command=True,
             context_settings=CONTEXT_SETTINGS, help="""
sample package
""")
@click.pass_context
def cli(ctx):
    """ command group """
    subcmd = ctx.invoked_subcommand

    if subcmd is None:
        print(ctx.get_help())


@cli.command(help="""
test progaram""")
@click.argument('arg1', type=str, nargs=-1)
@click.option('--opt1', '-o', 'opt1', multiple=True, help='option 1')
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
def cmd1(arg1, opt1, debug):
    """ cmd1  """
    log = get_logger(__name__, debug)

    app = Cmd1App(arg1, opt1, debug=debug)
    try:
        app.main()
    finally:
        log.debug('finally')
        app.end()
        log.info('end')


if __name__ == '__main__':
    cli(prog_name=__prog_name__)
