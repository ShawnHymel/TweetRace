# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.40
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.
# This file is compatible with both classic and new-style classes.

from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_spi_mover', [dirname(__file__)])
        except ImportError:
            import _spi_mover
            return _spi_mover
        if fp is not None:
            try:
                _mod = imp.load_module('_spi_mover', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _spi_mover = swig_import_helper()
    del swig_import_helper
else:
    import _spi_mover
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0



def new_uint8_tp():
  return _spi_mover.new_uint8_tp()
new_uint8_tp = _spi_mover.new_uint8_tp

def copy_uint8_tp(*args):
  return _spi_mover.copy_uint8_tp(*args)
copy_uint8_tp = _spi_mover.copy_uint8_tp

def delete_uint8_tp(*args):
  return _spi_mover.delete_uint8_tp(*args)
delete_uint8_tp = _spi_mover.delete_uint8_tp

def uint8_tp_assign(*args):
  return _spi_mover.uint8_tp_assign(*args)
uint8_tp_assign = _spi_mover.uint8_tp_assign

def uint8_tp_value(*args):
  return _spi_mover.uint8_tp_value(*args)
uint8_tp_value = _spi_mover.uint8_tp_value

def new_uint8_tArray(*args):
  return _spi_mover.new_uint8_tArray(*args)
new_uint8_tArray = _spi_mover.new_uint8_tArray

def delete_uint8_tArray(*args):
  return _spi_mover.delete_uint8_tArray(*args)
delete_uint8_tArray = _spi_mover.delete_uint8_tArray

def uint8_tArray_getitem(*args):
  return _spi_mover.uint8_tArray_getitem(*args)
uint8_tArray_getitem = _spi_mover.uint8_tArray_getitem

def uint8_tArray_setitem(*args):
  return _spi_mover.uint8_tArray_setitem(*args)
uint8_tArray_setitem = _spi_mover.uint8_tArray_setitem
class spi_mover(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, spi_mover, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, spi_mover, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _spi_mover.new_spi_mover(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _spi_mover.delete_spi_mover
    __del__ = lambda self : None;
    def transfer(self, *args): return _spi_mover.spi_mover_transfer(self, *args)
spi_mover_swigregister = _spi_mover.spi_mover_swigregister
spi_mover_swigregister(spi_mover)



