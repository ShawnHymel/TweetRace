import os
import sys
import pygame
import signal
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

# Configuration files
PARAM_FILE = 'param.txt'

# Other game paramters
RESOLUTION = [540, 960]
SETTINGS_SECTION = 'game_settings'
SETTINGS_TERMS = 'terms'
SETTINGS_TIME = 'game_time'
SETTINGS_FPS = 'fps'
SETTINGS_DEBUG = 'debug'
TWITTER_SECTION = 'twitter_auth'
TWITTER_ARGS = ['app_key', 'app_secret', 'oauth_token', 'oauth_token_secret']

# Common colors
WHITE = 255,255,255
GREEN = 0,255,0
BLACK = 0,0,0
BLUE  = 0,0,255
RED   = 255,0,0

#-----------------------------------------------------------------------------
# Global Variables
#-----------------------------------------------------------------------------

g_debug = None
g_terms = None
g_game_time = None
g_fps = None
g_twitter_auth = {}
g_mainloop = None
g_screen = None

#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------

# Automatically read the parameters text file and configure the game settings.
def config_params():   
    global g_fps
    global g_terms
    global g_game_time
    global g_debug
    global g_twitter_auth

    # Open text file
    config = ConfigParser.RawConfigParser()
    config.read(PARAM_FILE)

    # Read in twitter authentication
    for s in TWITTER_ARGS:
        g_twitter_auth[s] = config.get(TWITTER_SECTION, s)

    # Read in game settings
    g_terms = config.get(SETTINGS_SECTION, SETTINGS_TERMS).split(',')
    g_game_time = int(config.get(SETTINGS_SECTION, SETTINGS_TIME))
    g_game_time = g_game_time * 1000
    g_fps = int(config.get(SETTINGS_SECTION, SETTINGS_FPS))
    g_debug = int(config.get(SETTINGS_SECTION, SETTINGS_DEBUG))

# Handle graphics on the screen
def drawScreen():

    global g_screen

    # Create fonts
    font_timer = pygame.font.Font(None, 96)
    font_title_1 = pygame.font.Font(None, 68)
    font_title_2 = pygame.font.Font(None, 68)

    # Create background
    rect_bg = pygame.draw.rect(g_screen, BLACK, (0, 0, 540, 960), 0)
    rect_title = pygame.draw.rect(g_screen, WHITE, (20, 20, 500, 100), 0)
    rect_game_mode = pygame.draw.rect(g_screen, WHITE, (20, 140, 300, 60), 0)
    rect_timer = pygame.draw.rect(g_screen, WHITE, (340, 140, 180, 60), 0)
    
    # Draw title
    title1 = "The Great American"
    title2 = "Tweet Race"
    text_title_1 = font_title_1.render(title1,1,GREEN)
    text_title_2 = font_title_2.render(title2,1,GREEN)
    g_screen.blit(text_title_1, (40, 25))
    g_screen.blit(text_title_2, (40, 70))

    # Draw remaining time
    time_remaining = g_game_time - pygame.time.get_ticks()
    if time_remaining <= 0:
        game_time_str = '00:00'
        event_quit = pygame.event.Event(pygame.QUIT)
        pygame.event.post(event_quit)
    else:
        game_time_min = time_remaining / 60000
        game_time_sec = (time_remaining - (game_time_min * 60000)) / 1000
        if game_time_min < 10:
            game_time_str = '0' + str(game_time_min)
        else:
            game_time_str = str(game_time_min)
        if game_time_sec < 10:
            game_time_str = game_time_str + ':0' + str(game_time_sec)
        else:
            game_time_str = game_time_str + ':' + str(game_time_sec)
    text_timer = font_timer.render(game_time_str,1,RED)
    g_screen.blit(text_timer, (341, 140))

#-----------------------------------------------------------------------------
# Main
#-----------------------------------------------------------------------------

def main():
    global g_fps
    global g_terms
    global g_game_time
    global g_debug
    global g_twitter_auth
    global g_mainloop
    global g_screen

    # Read in parameters file and configure game
    config_params()
    if g_debug > 0:
        print ''
        print '============================='
        print 'The Great American Tweet Race'
        print '============================='
        print 'In debug mode: ', g_debug
        print ''
        print 'Search terms: ', g_terms

    # Create a TwitFeed object
    tf = twit_feed.TwitFeed(g_twitter_auth, g_terms)

    # Setup display
    g_screen = pygame.display.set_mode(RESOLUTION)
    pygame.init()
    fps_clock = pygame.time.Clock()
    #pygame.mouse.set_visible(False)

    # Main game loop
    g_mainloop = True
    if g_debug > 0:
        print 'Start game'
    while g_mainloop:
        drawScreen()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                g_mainloop = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                    g_mainloop = False
            elif event.type == pygame.USEREVENT+1:
                g_mainloop = False
        pygame.display.update()
        fps_clock.tick(g_fps)

    # Game over. Declare a winner
    tweet_list = tf.get_tweets()
    if g_debug > 0:
        for m in tweet_list:
            print m, '\n'
        print 'End game'
        print tf.tally_tweets()
    tf.stop()
    pygame.quit()

# Run main
main()
