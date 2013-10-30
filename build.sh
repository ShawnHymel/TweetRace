#!/bin/bash
echo Building all Python modules
cd py-apps/display_driver
sh build.sh
cd ../../py-apps/motor_driver
sh build.sh
cd ../..
