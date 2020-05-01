import ctypes
import os 
haha=ctypes.CDLL("libpulse-simple.so", mode = ctypes.RTLD_GLOBAL)
test=ctypes.CDLL("./42.so", mode = ctypes.RTLD_GLOBAL)
test.origincreate(0x6e69616d, 0, 0, 0)
test.say("haha %d\n", 666666)
