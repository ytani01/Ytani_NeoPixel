#!/usr/bin/env python3
#
# simle example 1

from ytani_neopixel import NeoPixel
import time


p = NeoPixel(led_n=14, brightness=15)

try:
    print('[Ctrl]-[C] to exit\n')

    while True:
        c = [0xff0000, 0xffff00, 0x0000ff]
        p.xfade_all(c)
        time.sleep(1)

        c = [0x0000ff, 0xff0000, 0xffff00]
        p.xfade_all(c)
        time.sleep(1)

        c = [0xffff00, 0x0000ff, 0xff0000]
        p.xfade_all(c)
        time.sleep(1)

finally:
    print('intrrupted')
    p.end()
