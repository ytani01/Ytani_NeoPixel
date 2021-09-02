# Ytani_NeoPixel
NeoPixel Library for Raspberry Pi

## 1. Install

### 1.1 Raspberry Pi OS setup

SPIでNeoPixelを利用するための設定

/boot/cmdline.txt
```
... spidev.bufsiz=32768
```

/boot/config.txt
```
[pi3]
core_freq=250

[pi4]
core_freq_min=500
```

sh
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
$ cd ~/env1
$ git clone git@github.com:ytani01/Ytani_NeoPixel.git
$ cd Ytani_NeoPixel
$ ./install.sh
```

## 2. Sample (test)

```
$ ytani-neopixel test1 14
````


## A. References

* [jgarff/rpi_ws281x(github)](https://github.com/jgarff/rpi_ws281x)
