# Ytani_NeoPixel
NeoPixel(WS281x) Library for Python 3 on Raspberry Pi

NeoPixel = WS2812B, SK6812


## 特徴

- 繰り返しパターンをリスト構造で簡単に指定できる。
- クロス・フェードできる


## 0. LED(NeoPixel)の電源について

NeoPixel(LED)の電源をRaspberry Pi本体と共有すると、電力不足でRaspberry Piが落ちる可能性があります。
LEDの電源は別にして、十分な電力供給をしてください。


## 1. Install

### 1.1 Raspberry Pi OS setup for SPI

SPIでNeoPixelを利用するための設定

- GPIOが、10に限定されるが..
- PCMオーディオ、PWM、アナログオーディオなどと共存できる
- 個人的な経験上、一番安定している(?)

上記の理由で、SPIの利用を推奨しますが、他のGPIOピン(PWM, PCM)を使うこともできます。
詳しくは、[rpi_sw281x/README](https://github.com/jgarff/rpi_ws281x/blob/master/README.md)をご参照ください。


#### 1.1.1 add user to group ``spi``

一般ユーザー(ここでは ``pi``)が、
spiデバイス(/dev/spi*)にアクセスできるようにする。

```
$ sudo gpasswd -a pi spi
```


#### 1.1.2 edit ``/boot/cmdline.txt``

行末に`` spidev.bufsiz=32768``を追加する。


#### 1.1.3 edit ``/boot/config.txt``

GPUコア周波数調整

```
[pi3]
core_freq=250

[pi4]
core_freq_min=500
```


#### 1.1.4 reboot OS
```
$ sudo reboot
```


### 1.2 make Python Virtualenv(venv)

```
$ cd ~
$ python3 -m venv env1
$ cd ~/env1
$ . ./bin/activate
(env1) $
```


### 1.3 download source and install

```
(env1)$ cd ~/env1  # venv以外の他のディレクトリでも可
(env1)$ git clone git@github.com:ytani01/Ytani_NeoPixel.git
(env1)$ cd Ytani_NeoPixel
(env1)$ ./install.sh
```

## 2. Sample (test) Programs

以下のヘルプ参照
```
$ ytani-neopixel -h
:
```

## 3. Library Usage

### 3.1 Simple example

``` python
#!/usr/bin/env python3

from ytani_neopixel import NeoPixel

pixel = NeoPixel(pin=10, led_n=16)

pixel.xfade_all([0xffffff, 0xff0000, 0x00ff00, 0x0000ff])
pixel.xfade_all(0)
pixel.end()
```

## A. References

* [jgarff/rpi_ws281x(github)](https://github.com/jgarff/rpi_ws281x)
