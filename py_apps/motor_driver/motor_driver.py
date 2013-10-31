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
            fp, pathname, description = imp.find_module('_motor_driver', [dirname(__file__)])
        except ImportError:
            import _motor_driver
            return _motor_driver
        if fp is not None:
            try:
                _mod = imp.load_module('_motor_driver', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _motor_driver = swig_import_helper()
    del swig_import_helper
else:
    import _motor_driver
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


eUNKNOWN = _motor_driver.eUNKNOWN
eAT_HOME = _motor_driver.eAT_HOME
eRUNNING = _motor_driver.eRUNNING
eAT_FAR_END = _motor_driver.eAT_FAR_END
eERROR = _motor_driver.eERROR
eMAX = _motor_driver.eMAX
class hoss_system(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, hoss_system, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, hoss_system, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _motor_driver.new_hoss_system()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _motor_driver.delete_hoss_system
    __del__ = lambda self : None;
    def find_home(self): return _motor_driver.hoss_system_find_home(self)
    def is_any_at_far_end(self): return _motor_driver.hoss_system_is_any_at_far_end(self)
    def get_winner(self): return _motor_driver.hoss_system_get_winner(self)
    def get_status(self, *args): return _motor_driver.hoss_system_get_status(self, *args)
    def set_race_value(self, *args): return _motor_driver.hoss_system_set_race_value(self, *args)
    def race(self): return _motor_driver.hoss_system_race(self)
hoss_system_swigregister = _motor_driver.hoss_system_swigregister
hoss_system_swigregister(hoss_system)
cvar = _motor_driver.cvar
NUM_HOSSES = cvar.NUM_HOSSES



