# Ytani NeoPixel Library

## for Python3 on Raspberry Pi
  [rpi-python3](rpi-python3/)
  
## for C++ on Arduino, ESP32, ATTiny etc..
  [arduino-c++](arduino-etc/)

### !! 重要 !!

NeoPixelライブラリは、割り込みに影響を与える。
``show()``の直後に、``interrupts()`` または ``sei()``をすること！


## ハマったこと

* NeoPixelライブラリの実行で、割り込みが効かなくなる
* ATTiny85のメモリ(RAM)不足で、挙動がおかしくなる。
