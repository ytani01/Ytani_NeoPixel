#!/usr/bin/env python3
#
#

import time
from rpi_ws281x import PixelStrip, Color
from my_logger import get_logger


class ytNeoPixel:
    """ ytNeoPixel """
    __log = get_logger(__name__, False)

    DEF_PIN        = 10
    DEF_COUNT      = 14
    DEF_FREQ       = 800000  # 800kHz
    DEF_DMA        = 10
    DEF_BRIGHTNESS = 10
    DEF_INVERT     = False
    DEF_CH         = 0

    def __init__(self, pin=DEF_PIN, count=DEF_COUNT, debug=False):
        """ constractor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('pin=%d, count=%d', pin, count)

        self._pin = pin
        self._count = count

        self._pixel = PixelStrip(self._count, self._pin,
                                 self.DEF_FREQ, self.DEF_DMA, self.DEF_INVERT,
                                 self.DEF_BRIGHTNESS,
                                 self.DEF_CH)
        self._pixel.begin()

        self.off_all()

    def set(self, i, color=Color(255, 255, 255)):
        """ set """
        self.__log.debug('i=%d, color=0x%06X', i, color)

        self._pixel.setPixelColor(i, color)
        self._pixel.show()

    def set_all(self, color=Color(255, 255, 255)):
        """ set_all """
        self.__log.debug('color=0x%06X', color)

        for i in range(self._count):
            self.set(i, color)

    def off_all(self):
        """ all_off """
        self.__log.debug('')

        self.set_all(Color(0, 0, 0))

    def xfade(self, i, color=0x000000, n=5, interval=0.05):
        """ """
        self.__log.debug('i=%d, color=0x%06X', i, color)

        c1 = self._pixel.getPixelColorRGB(i)
        c2 = lambda: None
        setattr(c2, 'r', color >> 16 & 0xff)
        setattr(c2, 'g', color >>  8 & 0xff)
        setattr(c2, 'b', color >>  0 & 0xff)

        dr = c2.r - c1.r
        dg = c2.g - c1.g
        db = c2.b - c1.b
        
        r = c1.r
        g = c1.g
        b = c1.b
        for j in range(n):
            r += dr / n
            g += dg / n
            b += db / n

            self._pixel.setPixelColorRGB(i, int(r), int(g), int(b))
            self._pixel.show()

            time.sleep(interval)

    def xfade_all(self, color=0x000000, n=10, interval=0.1):
        """ """
        self.__log.debug('color=0x%06X', color)

        c1 = [None] * self._count
        c2 = [None] * self._count
        d  = [None] * self._count
        for i in range(self._count):
            c1[i] = self._pixel.getPixelColorRGB(i)

            c2[i] = lambda: None
            setattr(c2[i], 'r', color >> 16 & 0xff)
            setattr(c2[i], 'g', color >>  8 & 0xff)
            setattr(c2[i], 'b', color >>  0 & 0xff)

            d[i] = lambda: None
            setattr(d[i], 'r', c2[i].r - c1[i].r)
            setattr(d[i], 'g', c2[i].g - c1[i].g)
            setattr(d[i], 'b', c2[i].b - c1[i].b)

        for j in range(n):
            for i in range(self._count):
                c1[i].r += d[i].r / n
                c1[i].g += d[i].g / n
                c1[i].b += d[i].b / n

                self._pixel.setPixelColorRGB(i,
                                             int(c1[i].r),
                                             int(c1[i].g),
                                             int(c1[i].b))

            self._pixel.show()

            time.sleep(interval)

    def end(self):
        """ end """
        self.__log.debug('')

        self.xfade_all(0xFFFFFF, n=5, interval=0.02)
        self.xfade_all(0x000000, n=5, interval=0.02)


import random


class SampleApp:
    """ Sample App """
    __log = get_logger(__name__, False)

    def __init__(self,
                 pin=ytNeoPixel.DEF_PIN,
                 count=ytNeoPixel.DEF_COUNT,
                 debug=False):
        """ constractor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('pin=%d, count=%d', pin, count)

        self._pin = pin
        self._count = count
        self._obj = ytNeoPixel(pin, count, self._dbg)

    def main(self):
        """ main """
        self.__log.debug('')

        for i in range(2):
            self._obj.set_all(random.randrange(0x1000000))
            time.sleep(1)

        self._obj.set_all(0x000000)

        for i in range(1000):
            self._obj.xfade(random.randrange(self._count),
                            random.randrange(0x1000000),
                            n=5, interval=0.01)

        for i in range(5):
            self._obj.xfade_all(random.randrange(0x1000000))
            time.sleep(1)

    def end(self):
        """ end """
        self.__log.debug('')

        self._obj.end()


import click
CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])


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

