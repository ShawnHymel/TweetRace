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

# Add pyscope module to path
path = os.path.join(os.path.dirname(__file__), 'py_apps/pyscope')
sys.path.append(path)

# Add twit_feed module to path
path = os.path.join(os.path.dirname(__file__), 'py_apps/twit_feed')
sys.path.append(path)

import display_driver
import motor_driver
import pyscope
import twit_feed

#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------

# Configuration files
PARAM_FILE = 'param.txt'

# Game paramters from the config file
SETTINGS_SECTION = 'game_settings'
SETTINGS_TERMS = 'terms'
SETTINGS_SPD_MULT = 'spd_mult'
SETTINGS_FPS = 'fps'
SETTINGS_HORSE_TICK = 'horse_tick'
SETTINGS_DEBUG = 'debug'
TWITTER_SECTION = 'twitter_auth'
TWITTER_ARGS = ['app_key', 'app_secret', 'oauth_token', 'oauth_token_secret']
RACE_NUMBER_SECTION = 'race_number_section'
RACE_NUMBER = 'race_number'

# Tweet settings and messages
TWEET_LINE_SPACING = -2
TWEET_START = 'And they\'re off! The lineup is'
TWEET_WINNER = 'We have a winner! It\'s'
TWEET_NO_WINNER = 'The race is over, and no one wins...'

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
g_spd_mult = 0
g_game_time = None
g_horse_tick = None
g_num_horses = 0
g_fps = None
g_twitter_auth = {}
g_mainloop = None
g_scope = None
g_led_display = None
g_tweet_list = []
g_race_number = None

#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------

# Automatically read the parameters text file and configure the game settings.
def config_params():   
    global g_fps
    global g_terms
    global g_spd_mult
    global g_game_time
    global g_horse_tick
    global g_num_horses
    global g_debug
    global g_twitter_auth
    global g_race_number

    # Open text file
    config = ConfigParser.RawConfigParser()
    config.read(PARAM_FILE)

    # Read in twitter authentication
    for s in TWITTER_ARGS:
        g_twitter_auth[s] = config.get(TWITTER_SECTION, s)

    # Read in game settings
    g_terms = config.get(SETTINGS_SECTION, SETTINGS_TERMS).split(',')
    g_num_horses = len(g_terms)
    g_spd_mult = int(config.get(SETTINGS_SECTION, SETTINGS_SPD_MULT))
    g_fps = int(config.get(SETTINGS_SECTION, SETTINGS_FPS))
    g_horse_tick = int(config.get(SETTINGS_SECTION, SETTINGS_HORSE_TICK))
    g_debug = int(config.get(SETTINGS_SECTION, SETTINGS_DEBUG))

    # Read in the race number
    g_race_number = int(config.get(RACE_NUMBER_SECTION, RACE_NUMBER))

# Update the parameter file with a new race number
def update_params():
    global g_race_number

    # Open text file
    config = ConfigParser.RawConfigParser()
    try:
        config.set(RACE_NUMBER_SECTION, RACE_NUMBER,

# Send display strings to alphanumeric LED displays
def display_terms():

    global g_terms
    global g_led_display

    # Remove '#' from all hashtags
    out_strs = [x[1:].lower() for x in g_terms]
    
    # In debug mode, only print to console
    if g_debug > 0:
        for s in out_strs:
            print 'Displaying: ', s
    if g_debug >= 0 and g_debug < 3:
        g_led_display = display_driver.display_driver()
        for str_index, s in enumerate(out_strs):   

            # Define char array
            out_chars = display_driver.new_charArray(len(s))

            # Fill char array
            print 'String ', str_index, ': ', s
            for i, c in enumerate(bytearray(s.upper())):
                display_driver.charArray_setitem(out_chars,i,c)
                print i, ' : ', display_driver.charArray_getitem(out_chars,i)

            # Send out string
            if not g_led_display.update_string(str_index, out_chars, len(s)):
                print 'Update string failed'
    

# Handle graphics on the screen
def draw_screen():

    global g_scope
    global g_tweet_list

    # Create fonts
    font_mode = pygame.font.Font(None, 96)
    font_timer = pygame.font.Font(None, 96)
    font_title_1 = pygame.font.Font(None, 68)
    font_title_2 = pygame.font.Font(None, 68)
    font_tweets = pygame.font.Font(None, 24)

    # Create background
    rect_bg = pygame.draw.rect(g_scope.screen, BLACK, \
                                                    (0, 0, 540, 960), 0)
    rect_title = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 20, 500, 100), 0)
    rect_game_mode = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 140, 210, 60), 0)
    rect_timer = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (250, 140, 270, 60), 0)
    rect_tweets = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 220, 500, 500), 0)
    rect_scores = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 740, 500, 100), 0)
    
    # Draw title
    title1 = "The Great American"
    title2 = "Tweet Race"
    text_title_1 = font_title_1.render(title1,1,BLACK)
    text_title_2 = font_title_2.render(title2,1,BLACK)
    g_scope.screen.blit(text_title_1, (40, 25))
    g_scope.screen.blit(text_title_2, (40, 70))

    # Draw game mode
    mode_str = font_mode.render('Race!',1,BLACK)
    g_scope.screen.blit(mode_str, (30, 135))

    # Draw game time
    if g_game_time <= 0:
        game_time_str = '00:00:00'
    else:
        game_time_min = g_game_time / 60000
        game_time_sec = (g_game_time - (game_time_min * 60000)) / 1000
        game_time_ms = (g_game_time - (game_time_min * 60000) - \
                                        (game_time_sec * 1000)) / 10
        if game_time_min < 10:
            game_time_str = '0' + str(game_time_min)
        else:
            game_time_str = str(game_time_min)
        if game_time_sec < 10:
            game_time_str = game_time_str + ':0' + str(game_time_sec)
        else:
            game_time_str = game_time_str + ':' + str(game_time_sec)
        if game_time_ms < 10:
            game_time_str = game_time_str + ':0' + str(game_time_ms)
        else:
            game_time_str = game_time_str + ':' + str(game_time_ms)
    text_timer = font_timer.render(game_time_str, 1 ,RED)
    g_scope.screen.blit(text_timer, (255, 135))

    # Draw tweets. Remove from the list ones that don't fit anymore.
    offset = 0
    for i, m in enumerate(g_tweet_list):
        if g_debug == 0 or g_debug == 1 or g_debug == 3:
            num_lines, text = draw_tweet(m, rect_tweets, font_tweets, offset)
            offset += (num_lines + 1)
            if text:
                g_tweet_list = g_tweet_list[:i]
                break

# Draw a tweet on a surface and wrap text.
# Return number of lines used and any text that did not fit
def draw_tweet(text, rect_tweets, font_tweets, offset):

    # Find bounds of rectangle and text
    font_height = font_tweets.size("Tg")[1]
    y = rect_tweets.top + offset * (font_height + TWEET_LINE_SPACING)
    
    # Loop through a tweet and wrap the text, if necessary
    num_lines = 0
    while text:
        i = 1

        # Determine if the row of text will be outside the rectangle
        if y + font_height > rect_tweets.bottom:
            break

        # Determine maximum width of the text
        while font_tweets.size(text[:i])[0] < rect_tweets.width and \
                                                        i < len(text):
            i += 1

        # If text was wrapped, break on previous letter     
        if i < len(text): 
           i -= 1

        # Render the line
        image = font_tweets.render(text[:i], 1, BLACK)
        g_scope.screen.blit(image, (rect_tweets.left + 1, y))
        y += font_height + TWEET_LINE_SPACING

        # Remove the text we just rendered
        text = text[i:]
        num_lines += 1

    return num_lines, text

#-----------------------------------------------------------------------------
# Main
#-----------------------------------------------------------------------------

def main():
    global g_fps
    global g_terms
    global g_spd_mult
    global g_game_time
    global g_horse_tick
    global g_num_horses
    global g_debug
    global g_twitter_auth
    global g_mainloop
    global g_scope
    global g_led_display
    global g_tweet_list
    global g_race_number

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

    # Create a new hoss system (motor driver) and reset
    if g_debug == 0:
        hp = motor_driver.hoss_system(False)
    else:
        hp = motor_driver.hoss_system(True)
    if g_debug < 3:
        hp.find_home()
    time.sleep(1)

    # Show terms on alphanumeric displays
    display_terms()

    # Create a TwitFeed object
    tf = twit_feed.TwitFeed(g_twitter_auth, g_terms)

    # Tweet that the race has started
    msg = 'Race ' + str(g_race_number) + ': ' + TWEET_START
    for m in g_terms:
        msg = msg + ' ' + m
    tf.tweet(msg)

    # Setup timer to move horses periodically
    pygame.time.set_timer(pygame.USEREVENT + 1, g_horse_tick)

    # Setup display
    g_game_time = 0;
    pygame.init()
    if g_debug == 0 or g_debug == 1 or g_debug == 3:
        g_scope = pyscope.pyscope()
    fps_clock = pygame.time.Clock()
    if g_debug == 0 or g_debug == 1 or g_debug == 3:
        pygame.mouse.set_visible(False)

    # Start streamer to search for terms
    tf.start_streamer()

    # Main game loop
    g_mainloop = True
    if g_debug > 0:
        print 'Start game'
    while g_mainloop:

        # Prepend global list with tweets
        tweets = tf.get_tweets()
        for m in reversed(tweets):
            g_tweet_list.insert(0, m)
            if g_debug > 0:
                print m, '\n'

        # Handle game events
        for event in pygame.event.get():

            # End game if quit event raises
            if event.type == pygame.QUIT:
                g_mainloop = False

            # End game if 'q' or 'esc' key pressed
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                    g_mainloop = False

            # Every timer interrupt, tally Tweets and move horses
            elif event.type == pygame.USEREVENT+1:
                score_card = tf.tally_tweets()
                for i in range(0, g_num_horses):
                    hp.set_race_value(i, score_card[i] * g_spd_mult)
                hp.race()
                if hp.is_any_at_far_end():
                    g_mainloop = False

        # Update screen
        g_game_time = pygame.time.get_ticks()
        if g_debug == 0 or g_debug == 1 or g_debug == 3:
            draw_screen()
            pygame.display.update()
        fps_clock.tick(g_fps)

    # Game over. Declare a winner.
    winner = hp.get_winner()
    if winner >= 0 and winner < g_num_horses:
        msg = 'Race ' + str(g_race_number) + ': ' + \
                            TWEET_WINNER + ' ' + winner
        tf.tweet(msg)
        if g_debug > 0:
            print 'End game'
            print 'Winner is ', winner
    else:
        msg = 'Race ' + str(g_race_number) + ': ' + TWEET_NO_WINNER
        tf.tweet(msg)
        if g_debug > 0:
            print 'End game'
            print 'No winner. All the horses suck.'

    # Increment race number in parameters file
    

    # Clean up Twitter feed and pygame
    tf.stop()
    pygame.quit()

# Run main
main()


