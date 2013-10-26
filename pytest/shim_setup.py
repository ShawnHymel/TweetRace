from distutils.core import setup, Extension

shim_module = Extension('_shim', sources=['shim_wrap.cxx', 'shim.cpp'])

setup(name='shim', version='0.1', 
      author='SparkFun Peeps', 
      description="""Example Shim module.""", 
      ext_modules=[shim_module], py_modules=['shim'])
