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
RACE_NUMBER_FILE = 'race_number.txt'

# Game parameters from the config file
SETTINGS_SECTION = 'game_settings'
SETTINGS_HANDLE = 'handle'
SETTINGS_TERMS = 'terms'
SETTINGS_MAX_TERMS = 'max_terms'
SETTINGS_SPD_MULT = 'spd_mult'
SETTINGS_FPS = 'fps'
SETTINGS_HORSE_TICK = 'horse_tick'
SETTINGS_DEBUG = 'debug'
SETTINGS_RESET_TIME = 'reset_time'
TWITTER_SECTION = 'twitter_auth'
TWITTER_ARGS = ['app_key', 'app_secret', 'oauth_token', 'oauth_token_secret']

# Parameters from the race number file
RACE_NUMBER_SECTION = 'race_number_section'
RACE_NUMBER = 'race_number'

# Tweet settings and messages
TWEET_LINE_SPACING = -2
TWEET_BETS_1 = 'Taking all bets! Send a tweet to '
TWEET_BETS_2 = ' with a #term to enter!'
TWEET_START = 'And they\'re off!'
TWEET_WINNER = 'We have a winner! It\'s'
TWEET_NO_WINNER = 'The race is over, and no one wins...'
TWEET_QUIT = 'Sorry folks, it looks like the race is shutting down for now.'

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
g_handle = None
g_terms = []
g_max_terms = None
g_spd_mult = 0
g_game_time = None
g_horse_tick = None
g_num_horses = 0
g_fps = None
g_twitter_auth = {}
g_scope = None
g_led_display = None
g_tweet_list = []
g_race_number = None
g_reset_time = None

#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------

# Automatically read the parameters text file and configure the game settings.
def config_params():   
    global g_fps
    global g_handle
    global g_terms
    global g_max_terms
    global g_spd_mult
    global g_game_time
    global g_horse_tick
    global g_num_horses
    global g_debug
    global g_twitter_auth
    global g_reset_time

    # Open text file
    config = ConfigParser.RawConfigParser()
    config.read(PARAM_FILE)

    # Read in twitter authentication
    for s in TWITTER_ARGS:
        g_twitter_auth[s] = config.get(TWITTER_SECTION, s)

    # Read in game settings
    g_handle = config.get(SETTINGS_SECTION, SETTINGS_HANDLE)
    g_max_terms = int(config.get(SETTINGS_SECTION, SETTINGS_MAX_TERMS))
    g_spd_mult = int(config.get(SETTINGS_SECTION, SETTINGS_SPD_MULT))
    g_fps = int(config.get(SETTINGS_SECTION, SETTINGS_FPS))
    g_horse_tick = int(config.get(SETTINGS_SECTION, SETTINGS_HORSE_TICK))
    g_debug = int(config.get(SETTINGS_SECTION, SETTINGS_DEBUG))
    g_reset_time = int(config.get(SETTINGS_SECTION, SETTINGS_RESET_TIME))

# Read race number from file, increment it, write it back
def update_race_number():

    global g_race_number

    # Read race number
    config = ConfigParser.RawConfigParser()
    config.read(RACE_NUMBER_FILE)
    g_race_number = int(config.get(RACE_NUMBER_SECTION, RACE_NUMBER))

    # Increment race number in param file for next time
    config.set(RACE_NUMBER_SECTION, RACE_NUMBER, str(g_race_number + 1))
    param_file = open(RACE_NUMBER_FILE, "w")
    config.write(param_file)
    param_file.close()
    del config

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
def draw_starting_screen():

    global g_terms
    global g_max_terms
    global g_scope

    # Create fonts
    font_mode = pygame.font.Font(None, 68)
    font_title_1 = pygame.font.Font(None, 68)
    font_title_2 = pygame.font.Font(None, 68)
    font_instr_1 = pygame.font.Font(None, 36)
    font_instr_2 = pygame.font.Font(None, 36)
    font_ent_title = pygame.font.Font(None, 36)
    font_ent = pygame.font.Font(None, 36)

    # Create background
    rect_bg = pygame.draw.rect(g_scope.screen, BLACK, \
                                                    (0, 0, 540, 960), 0)
    rect_title = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 20, 500, 100), 0)
    rect_game_mode = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 140, 500, 60), 0)
    rect_instructions = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 220, 500, 100), 0)
    rect_tweets = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 340, 500, 300), 0)
    
    # Draw title
    title1 = "The Great American"
    title2 = "Tweet Race"
    text_title_1 = font_title_1.render(title1,1,BLACK)
    text_title_2 = font_title_2.render(title2,1,BLACK)
    title_1_pos = text_title_1.get_rect()
    title_1_pos.centerx = rect_bg.centerx
    title_1_pos.top = 25
    title_2_pos = text_title_2.get_rect()
    title_2_pos.centerx = rect_bg.centerx
    title_2_pos.top = 70
    g_scope.screen.blit(text_title_1, title_1_pos)
    g_scope.screen.blit(text_title_2, title_2_pos)

    # Draw game mode
    mode_str = font_mode.render('Starting Gate',1,BLACK)
    g_scope.screen.blit(mode_str, (115, 140))

    # Draw instructions
    instr_str_1 = 'Send a tweet to @Gr8AmTweetRace'
    instr_str_2 = 'with a #term to enter!'
    instr_1 = font_instr_1.render(instr_str_1,1,BLACK)
    instr_2 = font_instr_2.render(instr_str_2,1,BLACK)
    g_scope.screen.blit(instr_1, (40, 240))
    g_scope.screen.blit(instr_2, (40, 270))

    # Draw entrants
    ent_title = font_ent_title.render('Contestants',1,BLACK)
    g_scope.screen.blit(ent_title, (40, 360))
    ent_y = 390
    for i in range(0, g_max_terms):
        ent_str = ''.join([str(i + 1), ': '])
        if i < len(g_terms):
            ent_str = ''.join([ent_str, g_terms[i]])
        ent_disp = font_ent.render(ent_str,1,BLACK)
        g_scope.screen.blit(ent_disp, (40, 390 + (i * 30)))

# Handle graphics on the screen
def draw_finish_screen(winner=None):

    global g_terms
    global g_scope

    # Create fonts
    font_mode = pygame.font.Font(None, 68)
    font_title_1 = pygame.font.Font(None, 68)
    font_title_2 = pygame.font.Font(None, 68)
    font_winner_1 = pygame.font.Font(None, 36)
    font_winner_2 = pygame.font.Font(None, 48)
    font_next = pygame.font.Font(None, 36)
    font_timer = pygame.font.Font(None, 96)

    # Create background
    rect_bg = pygame.draw.rect(g_scope.screen, BLACK, \
                                                    (0, 0, 540, 960), 0)
    rect_title = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 20, 500, 100), 0)
    rect_game_mode = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 140, 500, 60), 0)
    rect_winner = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 220, 500, 200), 0)
    rect_next = pygame.draw.rect(g_scope.screen, WHITE, \
                                                    (20, 440, 500, 140), 0)
    
    # Draw title
    title1 = "The Great American"
    title2 = "Tweet Race"
    text_title_1 = font_title_1.render(title1,1,BLACK)
    text_title_2 = font_title_2.render(title2,1,BLACK)
    title_1_pos = text_title_1.get_rect()
    title_1_pos.centerx = rect_bg.centerx
    title_1_pos.top = 25
    title_2_pos = text_title_2.get_rect()
    title_2_pos.centerx = rect_bg.centerx
    title_2_pos.top = 70
    g_scope.screen.blit(text_title_1, title_1_pos)
    g_scope.screen.blit(text_title_2, title_2_pos)

    # Draw game mode
    mode_str = font_mode.render('Finish!',1,BLACK)
    mode_pos = mode_str.get_rect()
    mode_pos.centerx = rect_bg.centerx
    mode_pos.top = 140
    g_scope.screen.blit(mode_str, mode_pos)

    # Draw winner
    if winner == None:
        winner_str_1 = 'No one wins.'
        winner_1 = font_winner_1.render(winner_str_1,1,BLACK)
        winner_1_pos = winner_1.get_rect()
        winner_1_pos.centerx = rect_bg.centerx
        winner_1_pos.top = 240
        g_scope.screen.blit(winner_1, winner_1_pos)
    else:
        winner_str_1 = 'And the winner is:'
        winner_1 = font_winner_1.render(winner_str_1,1,BLACK)
        winner_2 = font_winner_2.render(winner,1,BLACK)
        winner_2_pos = winner_2.get_rect()
        winner_2_pos.centerx = rect_bg.centerx
        winner_2_pos.top = 290
        g_scope.screen.blit(winner_1, (40, 240))
        g_scope.screen.blit(winner_2, winner_2_pos)

    # Display timer for next game
    next_str = font_next.render('Next game in:',1,BLACK)
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
    timer_pos = text_timer.get_rect()
    timer_pos.centerx = rect_bg.centerx
    timer_pos.top = 490
    g_scope.screen.blit(next_str, (40, 460))
    g_scope.screen.blit(text_timer, timer_pos)

# Test if a term is already in the term list
def is_in_terms(entry):
    
    global g_terms

    for term in g_terms:
        if ''.join(['#', entry]) == term:
            return True
    return False

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
    title_1_pos = text_title_1.get_rect()
    title_1_pos.centerx = rect_bg.centerx
    title_1_pos.top = 25
    title_2_pos = text_title_2.get_rect()
    title_2_pos.centerx = rect_bg.centerx
    title_2_pos.top = 70
    g_scope.screen.blit(text_title_1, title_1_pos)
    g_scope.screen.blit(text_title_2, title_2_pos)

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
    global g_handle
    global g_race_number
    global g_max_terms
    global g_spd_mult
    global g_game_time
    global g_horse_tick
    global g_num_horses
    global g_debug
    global g_twitter_auth
    global g_scope
    global g_led_display
    global g_tweet_list
    global g_reset_time

    # Read in parameters file and configure game
    config_params()
    if g_debug > 0:
        print ''
        print '============================='
        print 'The Great American Tweet Race'
        print '============================='
        print 'In debug mode: ', g_debug

    # Create a new hoss system (motor driver)
    if g_debug == 0 or g_debug == 1 or g_debug == 5:
        hp = motor_driver.hoss_system(False)
    else:
        hp = motor_driver.hoss_system(True)
    time.sleep(1)

    # Create a TwitFeed object
    tf = twit_feed.TwitFeed(g_twitter_auth)

    # Setup display
    g_game_time = 0;
    pygame.init()
    if g_debug == 0 or g_debug == 3:
        g_scope = pyscope.pyscope()
    fps_clock = pygame.time.Clock()
    if g_debug == 0 or g_debug == 3:
        pygame.mouse.set_visible(False)

    # Setup timer to move horses periodically
    pygame.time.set_timer(pygame.USEREVENT + 1, g_horse_tick)

    #---------------
    # Main Game Loop
    break_loop = False
    while True:

        #--------------
        # Betting Stage

        # Clear out previous terms
        g_terms = []
        g_tweet_list = []
        tweets = []
        tf.flush_tweets()
        
        # Move horses back to starting gate
        if g_debug < 3:
            hp.find_home()

        # Update race number
        update_race_number()
        
        # Start streamer to search for betting terms
        if g_debug > 0:
            print ''
            print '-------------'
            print 'Betting Stage'
            print '-------------'
            print 'Tweet entries to: ', g_handle
            print 'Max entries: ', g_max_terms

        # Tweet that we are taking bets
        msg = 'Race ' + str(g_race_number) + ': ' + TWEET_BETS_1 + \
                                                    g_handle + TWEET_BETS_2
        tf.tweet(msg)

        # Start listening for incoming tweets/bets
        tf.start_bet_streamer(g_handle)

        # Start fps clock
        fps_clock = pygame.time.Clock()

        # Main loop for betting stage
        bet_loop = True
        while bet_loop:

            # Handle game events
            for event in pygame.event.get():

                # End game if quit event raises
                if event.type == pygame.QUIT:
                    break_loop = True
                    bet_loop = False

                # End game if 'q' or 'esc' key pressed
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                        break_loop = True
                        bet_loop = False

            # Get entries and print them
            entries = tf.get_entries()
            for entry in entries:
                print entry
                if is_in_terms(entry) == False:
                    g_terms.append(''.join(['#', entry]))
                    print len(g_terms)
                    if len(g_terms) >= g_max_terms:
                        bet_loop = False

            # Update screen
            if g_debug == 0 or g_debug == 3:
                draw_starting_screen()
                pygame.display.update()
            fps_clock.tick(g_fps)

        # Clean up Twitter feed
        tf.stop_betting(2)

        # Check if we need to end the game
        if break_loop:
            break

        #-----------
        # Race Stage
        
        # Show terms on alphanumeric displays
        display_terms()

        # Set number of horses
        g_num_horses = len(g_terms)

        # Tweet that the race has started
        if g_debug > 0:
            print ''
            print '-----'
            print 'Race!'
            print '-----'
            print 'And they\'re off! Race number', str(g_race_number), \
                                                                    'has begun!'
        msg = 'Race ' + str(g_race_number) + ': ' + TWEET_START
        for m in g_terms:
            msg = msg + ' ' + m
        tf.tweet(msg)

        # Start streamer to search for terms
        tf.start_track_streamer(g_terms)

        # Main game loop
        race_loop = True
        if g_debug > 0:
            print 'Start game'
            print 'Contestants: ', g_terms
            print ''
        game_time_zero = pygame.time.get_ticks()
        while race_loop:

            # Prepend global list with tweets
            tweets = tf.get_tweets()
            for m in tweets:
                g_tweet_list.insert(0, m)
                if g_debug > 0:
                    print m, '\n'

            # Handle game events
            for event in pygame.event.get():

                # End game if quit event raises
                if event.type == pygame.QUIT:
                    break_loop = True
                    race_loop = False

                # End game if 'q' or 'esc' key pressed
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                        break_loop = True
                        race_loop = False

                # Every timer interrupt, tally Tweets and move horses
                elif event.type == pygame.USEREVENT+1:
                    score_card = tf.tally_tweets()
                    for i in range(0, g_num_horses):
                        hp.set_race_value(i, score_card[i] * g_spd_mult)
                    hp.race()
                    if hp.is_any_at_far_end():
                        race_loop = False

            # Update screen
            g_game_time = pygame.time.get_ticks() - game_time_zero
            if g_debug == 0 or g_debug == 3:
                draw_screen()
                pygame.display.update()
            fps_clock.tick(g_fps)

        # Stop tracking thread
        tf.stop_tracking(2)

        # Check if we need to end the game
        if break_loop:
            break

        #-------------
        # Finish Stage

        # Game over. Declare a winner.
        winner_num = hp.get_winner()
        winner = None
        if winner_num >= 0 and winner_num < g_num_horses:
            winner = g_terms[winner_num]
            msg = 'Race ' + str(g_race_number) + ': ' + \
                                TWEET_WINNER + ' ' + winner
            tf.tweet(msg)
            if g_debug > 0:
                print '-------'
                print 'Finish!'
                print '-------'
                print 'The winner is ', winner
        else:
            msg = 'Race ' + str(g_race_number) + ': ' + TWEET_NO_WINNER
            tf.tweet(msg)
            if g_debug > 0:
                print ''
                print '-------'
                print 'Finish!'
                print '-------'
                print 'No winner. All the horses suck.'

        # Display winner and wait for next game
        finish_loop = True
        game_time_zero = pygame.time.get_ticks() + (g_reset_time * 1000)
        while finish_loop:

            # Handle game events
            for event in pygame.event.get():

                # End game if quit event raises
                if event.type == pygame.QUIT:
                    break_loop = True
                    finish_loop = False

                # End game if 'q' or 'esc' key pressed
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                        break_loop = True
                        finish_loop = False

            # Update countdown timer
            g_game_time = game_time_zero - pygame.time.get_ticks()
            if g_game_time <= 0:
                finish_loop = False
                break

            # Update screen
            if g_debug == 0 or g_debug == 3:
                draw_finish_screen(winner)
                pygame.display.update()
            fps_clock.tick(g_fps)

        # Check if we need to end the game
        if break_loop:
            break

    # Tweet that we are shutting down
    msg = 'Race ' + str(g_race_number) + ': ' + TWEET_QUIT
    tf.tweet(msg)

    # Clean up Twitter feed and pygame
    time.sleep(1) # Needed to let threads close to avoid Exception being thrown
    pygame.quit()
    del tf

# Run main
main()
