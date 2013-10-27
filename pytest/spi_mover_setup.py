from distutils.core import setup, Extension

spi_mover_module = Extension('_spi_mover', sources=['spi_mover.cpp', 
                                                    'spi_mover_wrap.cxx'])

setup(name='spi_mover', version='0.1', 
      author='SparkFun Peeps', 
      description="""Example spi_mover module.""", 
      ext_modules=[spi_mover_module], py_modules=['spi_mover'])
