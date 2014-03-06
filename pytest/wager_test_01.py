import os
import sys
import pygame
import signal
import time
import ConfigParser

#-----------------------------------------------------------------------------
# Import custom modules
#-----------------------------------------------------------------------------

# Add twit_feed module to path
path = os.path.join(os.path.dirname(__file__), '../py_apps/twit_feed')
sys.path.append(path)

import twit_feed


