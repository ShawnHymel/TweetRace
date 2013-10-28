#!/bin/bash
echo Building display driver Python wrapper
swig -c++ -python -shadow display_driver.i
python setup.py build_ext --inplace
