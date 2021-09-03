# Ytani_NeoPixel
NeoPixel Library for Raspberry Pi

## 0. LED(NeoPixel)の電源について

NeoPixel(LED)の電源をRaspberry Pi本体と共有すると、電力不足でRaspberry Piが落ちる可能性があります。
LEDの電源は別にして、十分な電力供給をしてください。

## 1. Install

### 1.1 Raspberry Pi OS setup for SPI

SPIでNeoPixelを利用するための設定

- GPIOが、10に限定されるが..
- PCMオーディオ、PWM、アナログオーディオなどと共存できる

上記の理由で、SPIの利用を推奨しますが、他のGPIOピン(PWM, PCM)を使うこともできます。
詳しくは、[rpi_sw281x/README](https://github.com/jgarff/rpi_ws281x/blob/master/README.md)を参照ください。

#### 1.1.1 edit ``/boot/cmdline.txt``
```
... spidev.bufsiz=32768
```

#### 1.1.2 edit ``/boot/config.txt``
```
[pi3]
core_freq=250

[pi4]
core_freq_min=500
```

#### 1.1.3 reboot OS
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

## 2. Sample (test)

以下のヘルプ参照
```
$ ytani-neopixel -h
:

$ ytani-neopixel test1 -h
:

$ ytani-neopixel test2 -h
:
````


## A. References

* [jgarff/rpi_ws281x(github)](https://github.com/jgarff/rpi_ws281x)
