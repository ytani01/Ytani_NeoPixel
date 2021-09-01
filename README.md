# ytNeoPixelPi
NeoPixel Library for Raspberry Pi and Python

## Install

### Raspberry Pi OS setup

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

### make Python Virtualenv(venv)

```
$ cd ~
$ python3 -m venv env1
$ cd ~/env1
$ . ./bin/activate
(env1) $
```

### download source from git

```
$ cd ~/env1
$ git clone git@github.com:ytani01/ytNeoPixelPi.git
```


## Sample (test)


## References

* [jgarff/rpi_ws281x(github)](https://github.com/jgarff/rpi_ws281x)
