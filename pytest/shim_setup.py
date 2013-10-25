from distutils.core import setup, Extension
setup(name='shim_test', version='1.0',  \
      ext_modules=[Extension('shimext', ['shim.cpp'])])
