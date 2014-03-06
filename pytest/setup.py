# To build: $ python setup.py build_ext --inplace

from distutils.core import setup, Extension
setup(name='shim', version='0.1',  \
      ext_modules=[Extension('shim', ['shim.cpp'])])
