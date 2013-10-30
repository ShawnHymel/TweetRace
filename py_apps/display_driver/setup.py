from distutils.core import setup, Extension

display_driver_module = Extension('_display_driver', sources=[
                            '../../c-apps/spi-mover/spi_mover.cpp',
                            '../../c-apps/display-driver/string_cb.cpp',
                            '../../c-apps/display-driver/display_driver.cpp', 
                            'display_driver_wrap.cxx'])

display_driver_module.extra_compile_args = \
                                ['-I../../c-apps/spi-mover']

setup(name='display_driver', version='1.0', 
      author='Shawn Hymel, Byron Jacquot', 
      description="""Driver for SPI-based LED segment displays""", 
      ext_modules=[display_driver_module], py_modules=['display_driver'])
