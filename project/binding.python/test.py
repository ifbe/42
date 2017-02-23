import ctypes
import os 
haha=ctypes.CDLL("libpulse-simple.so", mode = ctypes.RTLD_GLOBAL)
test=ctypes.CDLL("42.so", mode = ctypes.RTLD_GLOBAL)
test.birth()
test.death()
