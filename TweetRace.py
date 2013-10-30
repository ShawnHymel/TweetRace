import os, sys
import time
import ConfigParser

#-----------------------------------------------------------------------------
# Import custom modules
#-----------------------------------------------------------------------------

# Add display_driver module to path
path = os.path.join(os.path.dirname(__file__), 'py_apps/display_driver')
sys.path.append(path)

# Add motor_driver module to path
path = os.path.join(os.path.dirname(__file__), 'py_apps/motor_driver')
sys.path.append(path)

# Add twit_feed module to path
path = os.path.join(os.path.dirname(__file__), 'py_apps/twit_feed')
sys.path.append(path)

import display_driver
import motor_driver
import twit_feed

#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------

PARAM_FILE = 'param.txt'

#-----------------------------------------------------------------------------
# Global Variables
#-----------------------------------------------------------------------------

g_terms = None
g_game_time = None

#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------

# Automatically read the parameters text file and configure the game settings.
def config_params():
    
    global g_terms
    global g_game_time

    # Open text file
    config = ConfigParser.RawConfigParser()
    config.read(PARAM_FILE)

    # Read in twitter authentication
    print config.get('twitter_auth', 'app_key')
    print config.get('twitter_auth', 'app_secret')
    print config.get('twitter_auth', 'oauth_token')
    print config.get('twitter_auth', 'oauth_token_secret')

    # Read in game settings
    g_terms = config.get('game_settings', 'terms')
    g_game_time = float(config.get('game_settings', 'game_time'))

config_params()
print g_terms

# Twitter authentication
twitter_auth = {    'app_key': '0SGK64SYL8Y0R4tCbcm6SA',
                    'app_secret': 'f1RFySmg0RVRVtTGnvu9NvrWm73IT7C8Z1gpcYZAwo',
                    'oauth_token': '1969690717-lDn8UEoAIp5h2zITLyPtfGjY8EGhazHTzUVQBvH',
                    'oauth_token_secret': 'gv9vGWluxgMtMY77evxainwolbFhxIiCJsGTd79N2I' }

# Create a TwitFeed object
tf = twit_feed.TwitFeed(twitter_auth, g_terms)

# Wait for game to end and display tweets/score
print 'START OF GAME!'
time.sleep(g_game_time)
tweet_list = tf.get_tweets()
for m in tweet_list:
    print m, '\n'
print 'END OF GAME!'
print tf.tally_tweets()

# Try another game
print 'START OF GAME!'
time.sleep(g_game_time)
tweet_list = tf.get_tweets()
for m in tweet_list:
    print m, '\n'
print 'END OF GAME!'
print tf.tally_tweets()
