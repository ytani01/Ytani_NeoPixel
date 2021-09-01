#!/usr/bin/env python3
#
#
from my_logger import get_logger


import click
CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])


class SampleApp:
    """ Sample App """
    __log = get_logger(__name__, False)

    def __init__(self, debug=False):
        """ constractor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)

    def main(self):
        """ main """
        self.__log.debug('')

    def end(self):
        """ end """
        self.__log.debug('')


@click.command(context_settings=CONTEXT_SETTINGS, help="""
""")
@click.option('--debug', '-d', 'debug', is_flag=True, default=False,
              help='debug flag')
def main(debug):
    """ Sample main """
    __log = get_logger(__name__, debug)

    app = SampleApp(debug=debug)
    try:
        app.main()
    finally:
        __log.debug('finally')
        app.end()


if __name__ == '__main__':
    main()

