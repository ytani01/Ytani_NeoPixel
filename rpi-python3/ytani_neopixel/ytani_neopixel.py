#
# (c) 2021 Yoichi Tanibayasih
#
import time
from rpi_ws281x import PixelStrip, Color

from .my_logger import get_logger


class NeoPixel_Color:
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


class NeoPixel:
    """ NeoPixel """
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

        # self.clear()

    def end(self):
        """ end """
        self.__log.debug('')

        self.clear()

    def __len__(self):
        """ len """
        return self._led_n

    def get_brightness(self):
        """ get_brightness """
        self.__log.debug('')

        return self._pixel.getBrightness()

    def set_brightness(self, brightness):
        """ set_brightness """
        self.__log.debug('brightness=%s', brightness)

        self._pixel.setBrightness(brightness)
        self._pixel.show()

    def set(self, i=0, color=Color(255, 255, 255)):
        """ set """
        self.__log.debug('i=%d, color=0x%06X', i, color)

        self._pixel.setPixelColor(i, color)
        self._pixel.show()

    def set_all(self, color=Color(255, 255, 255)):
        """ set_all """

        if isinstance(color, int):
            color = [color]

        if len(color) == 0:
            color = [0]

        while len(color) < self._led_n:
            color += color

        msg = "[ "
        i = 0
        for c in color:
            self._pixel.setPixelColor(i, c)
            msg += "#%06X " % c
            i += 1
            if i >= self._led_n:
                break

        msg += "]"
        self.__log.debug('color=%s', msg)

        self._pixel.show()

    def clear(self):
        """ """
        self.__log.debug('')

        self.set_all(0x000000)

    def xfade(self, i, color=0x000000, n=5, interval=0.05):
        """ """
        self.__log.debug('i=%d, color=0x%06X', i, color)

        c1 = self._pixel.getPixelColorRGB(i)
        c2 = NeoPixel_Color(color=color, debug=self._dbg)

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
        self.__log.debug('color=%s, n=%d, interval=%s',
                         color, n, interval)

        if isinstance(color, int):
            color = [color]

        if len(color) == 0:
            color = [0]

        while len(color) < self._led_n:
            color += color

        c1 = []
        c2 = []
        d = []
        for i in range(self._led_n):
            c1.append(self._pixel.getPixelColorRGB(i))
            c2.append(NeoPixel_Color(color=color[i], debug=self._dbg))
            d.append(NeoPixel_Color((c2[i].r - c1[i].r),
                                    (c2[i].g - c1[i].g),
                                    (c2[i].b - c1[i].b)))

        for n_i in range(n):
            for i in range(self._led_n):
                c1[i].r += d[i].r / n
                c1[i].g += d[i].g / n
                c1[i].b += d[i].b / n

                self._pixel.setPixelColorRGB(i,
                                             round(c1[i].r),
                                             round(c1[i].g),
                                             round(c1[i].b))

            self._pixel.show()

            time.sleep(interval)
