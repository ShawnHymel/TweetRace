#!/bin/bash
echo Building all Python modules
cd py_apps/display_driver
sh build.sh
cd ../../py_apps/motor_driver
sh build.sh
cd ../..
