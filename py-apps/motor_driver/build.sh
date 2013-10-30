#!/bin/bash
echo Building motor driver Python wrapper
swig -c++ -python -shadow motor_driver.i
python setup.py build_ext --inplace
