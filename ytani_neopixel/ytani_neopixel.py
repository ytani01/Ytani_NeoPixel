#
# (c) 2021 Yoichi Tanibayasih
#
import time
from rpi_ws281x import PixelStrip, Color

from .my_logger import get_logger


class Ytani_NeoPixel_Color:
    """  """
    __log = get_logger(__name__, False)

    def __init__(self, r: int = 0, g: int = 0, b: int = 0,
                 color: int = None,
                 debug=False):
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)

        if color is None:
            self.r = r
            self.g = g
            self.b = b
        else:
            self.r = color >> 16 & 0xff
            self.g = color >> 8 & 0xff
            self.b = color & 0xff


class Ytani_NeoPixel:
    """ Ytani_NeoPixel """
    __log = get_logger(__name__, False)

    DEF_PIN = 10  # SPI
    DEF_LED_N = 14
    DEF_BRIGHTNESS = 50
    DEF_FREQ = 800000  # 800kHz
    DEF_DMA = 10
    DEF_INVERT = False
    DEF_CH = 0

    def __init__(self, pin=DEF_PIN, led_n=DEF_LED_N,
                 brightness=DEF_BRIGHTNESS,
                 freq=DEF_FREQ, dma=DEF_DMA,
                 invert=DEF_INVERT, ch=DEF_CH,
                 debug=False):
        """ constractor """
        self._dbg = debug
        __class__.__log = get_logger(__class__.__name__, self._dbg)
        self.__log.debug('pin=%d, led_n=%d, brightness=%d',
                         pin, led_n, brightness)
        self.__log.debug('freq=%d, dma=%d, invert=%s, ch=%d',
                         freq, dma, invert, ch)

        self._pin = pin
        self._led_n = led_n
        self._brightness = brightness
        self._freq = freq
        self._dma = dma
        self._invert = invert
        self._ch = ch

        self._pixel = PixelStrip(self._led_n, self._pin,
                                 self._freq, self._dma, self._invert,
                                 self._brightness, self._ch)
        self._pixel.begin()

        self.off_all()

    def end(self):
        """ end """
        self.__log.debug('')

        self.off_all()

    def get_rgb(self, color):
        """  """
        self.__log.debug('color=0x%06X', color)

        r = color >> 16 & 0xff
        g = color >> 8 & 0xff
        b = color & 0xff

        return (r, g, b)

    def set(self, i, color=Color(255, 255, 255)):
        """ set """
        self.__log.debug('i=%d, color=0x%06X', i, color)

        self._pixel.setPixelColor(i, color)
        self._pixel.show()

    def set_all(self, color=Color(255, 255, 255)):
        """ set_all """
        self.__log.debug('color=0x%06X', color)

        for i in range(self._led_n):
            self._pixel.setPixelColor(i, color)

        self._pixel.show()

    def off_all(self):
        """ """
        self.__log.debug('')

        self.set_all(Color(0, 0, 0))

    def xfade(self, i, color=0x000000, n=5, interval=0.05):
        """ """
        self.__log.debug('i=%d, color=0x%06X', i, color)

        c1 = self._pixel.getPixelColorRGB(i)

        """
        c2 = lambda: None
        setattr(c2, 'r', color >> 16 & 0xff)
        setattr(c2, 'g', color >> 8 & 0xff)
        setattr(c2, 'b', color & 0xff)
"""
        c2 = Ytani_NeoPixel_Color(color=color, debug=self._dbg)

        dr = c2.r - c1.r
        dg = c2.g - c1.g
        db = c2.b - c1.b

        for n_i in range(n):
            c1.r += dr / n
            c1.g += dg / n
            c1.b += db / n

            self._pixel.setPixelColorRGB(i, int(c1.r), int(c1.g), int(c1.b))
            self._pixel.show()

            time.sleep(interval)

    def xfade_all(self, color=0x000000, n=10, interval=0.1):
        """ """
        self.__log.debug('color=0x%06X, n=%d, interval=%s',
                         color, n, interval)

        c1 = []
        c2 = []
        d = []
        for i in range(self._led_n):
            c1.append(self._pixel.getPixelColorRGB(i))

            c2.append(Ytani_NeoPixel_Color(color=color, debug=self._dbg))

            d.append(Ytani_NeoPixel_Color(c2[i].r - c1[i].r,
                                          c2[i].g - c1[i].g,
                                          c2[i].b - c1[i].b ))

        for n_i in range(n):
            for i in range(self._led_n):
                c1[i].r += d[i].r / n
                c1[i].g += d[i].g / n
                c1[i].b += d[i].b / n

                (r, g, b) = (round(c1[i].r), round(c1[i].g), round(c1[i].b))
                self.__log.debug('%d: c1[%d]=(%s, %s, %s)', n_i, i, r, g, b)
                self._pixel.setPixelColorRGB(i, r, g, b)

            self._pixel.show()

            time.sleep(interval)
