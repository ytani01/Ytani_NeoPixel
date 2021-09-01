# ytNeoPixelPi
NeoPixel Library for Raspberry Pi and Python

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

### 1.3 download source from git

```
$ cd ~/env1
$ git clone git@github.com:ytani01/ytNeoPixelPi.git
```

### 1.4 install test program

```
$ cd ~/env1/ytNeoPixel
$ ./install.sh
```

## 2. Sample (test)



## A. References

* [jgarff/rpi_ws281x(github)](https://github.com/jgarff/rpi_ws281x)
