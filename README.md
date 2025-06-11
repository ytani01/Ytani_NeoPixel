# Ytani NeoPixel Library

## 1. for Python3 on Raspberry Pi
  [rpi-python3/](rpi-python3/)
  

## 2. for C++ on Arduino, ESP32, ATTiny etc..
  [arduino-etc/](arduino-etc/)

### 2.0 !! 重要 !!

* NeoPixelライブラリの実行で、割り込みが効かなくなる
  ``show()``の直後に、``interrupts()`` または ``sei()``をすること！
* ATTiny85のメモリ(RAM)不足で、挙動がおかしくなる。
