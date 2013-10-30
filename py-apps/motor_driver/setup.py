from distutils.core import setup, Extension

motor_driver_module = Extension('_motor_driver', sources=[
                            '../../c-apps/spi-mover/spi_mover.cpp',
                            '../../c-apps/motor-driver/dspin_driver.cpp',
                            '../../c-apps/motor-driver/hoss_system.cpp', 
                            'motor_driver_wrap.cxx'])

motor_driver_module.extra_compile_args = \
                                ['-I../../c-apps/spi-mover']

setup(name='motor_driver', version='1.0', 
      author='Shawn Hymel, Byron Jacquot', 
      description="""Driver for SPI-based stepper motors""", 
      ext_modules=[motor_driver_module], py_modules=['motor_driver'])
