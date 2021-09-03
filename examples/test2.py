#!/usr/bin/env python3

from ytani_neopixel import NeoPixel
import random
import time

p = NeoPixel(led_n=14, brightness=15)

def test():
    while True:
        c = [0] * (random.randrange(len(p)-1) + 1)
        for i in range(len(c)):
            c[i] = random.randrange(0x1000000)
        print(c)
        p.xfade_all(c)

        time.sleep(1)


if __name__ == '__main__':
    try:
        print('[Ctrl]-[C] to exit\n')

        test()

    finally:
        print('intrrupted')
        p.end()
