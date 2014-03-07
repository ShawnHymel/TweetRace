import os
import sys
import pygame
import signal
import time
import ConfigParser
from twython import TwythonStreamer

#-----------------------------------------------------------------------------
# Import custom modules
#-----------------------------------------------------------------------------

# Add pyscope module to path
path = os.path.join(os.path.dirname(__file__), 'py_apps/pyscope')
sys.path.append(path)

# Add twit_feed module to path
path = os.path.join(os.path.dirname(__file__), '../py_apps/twit_feed')
sys.path.append(path)

import pyscope
import twit_feed
#import tf_test_02

#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------

MAX_ENTRIES = 1
FPS = 5
BET_TERM = ['#testing', '#blargz'] #['@Gr8AmTweetRace']
AUTH =  {   'app_key': 'li8wn8Tb7xBifCnNIgyqUw', 
            'app_secret': 'vcwq36w4C4VXamlqWBDKM2E8etsOoangDoMhxNDU',
            'oauth_token': '1969690717-rGw3VkRQ8IyL4OcPWtv5Y2CeBdVn8ndJrjGKraI',
            'oauth_token_secret': 'KO7YIFMKWKaYTtz2zEyaSy044ixj5kIbWrDtZZL96ly0H'}

# Common colors
WHITE = 255,255,255
GREEN = 0,255,0
BLACK = 0,0,0
BLUE  = 0,0,255
RED   = 255,0,0

#-----------------------------------------------------------------------------
# Global Variables
#-----------------------------------------------------------------------------

g_terms = []
g_bet_loop = None
g_scope = None

#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------

# Handle graphics on the screen
def draw_starting_screen():

    global g_terms
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
    g_scope.screen.blit(text_title_1, (40, 25))
    g_scope.screen.blit(text_title_2, (130, 70))

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
    for i in range(0, MAX_ENTRIES):
        ent_str = ''.join([str(i + 1), ': '])
        if i < len(g_terms):
            ent_str = ''.join([ent_str, g_terms[i]])
        ent_disp = font_ent.render(ent_str,1,BLACK)
        g_scope.screen.blit(ent_disp, (40, 390 + (i * 30)))

# Test if a term is already in the term list
def is_in_terms(entry):
    
    global g_terms

    for term in g_terms:
        if ''.join(['#', entry]) == term:
            return True
    return False

#-----------------------------------------------------------------------------
# Main
#-----------------------------------------------------------------------------

def main():

    global g_bet_loop
    global g_scope
    global g_terms

    # Setup Twitter streamer
    tf = twit_feed.TwitFeed(AUTH)
    #tf = tf_test_02.TwitFeed(AUTH)

    # Tweet that we are accepting bets

    # Start streamer to search for terms
    tf.start_track_streamer(BET_TERM)

    # Setup display
    pygame.init()
    #g_scope = pyscope.pyscope()
    fps_clock = pygame.time.Clock()
    pygame.mouse.set_visible(False)

    # Main game loop
    g_bet_loop = False
    while g_bet_loop:

        # Handle game events
        for event in pygame.event.get():

            # End game if quit event raises
            if event.type == pygame.QUIT:
                g_bet_loop = False

            # End game if 'q' or 'esc' key pressed
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                    g_bet_loop = False

        # Get entries and print them
        entries = tf.get_entries()
        for entry in entries:
            print entry
            if is_in_terms(entry) == False:
                g_terms.append(''.join(['#', entry]))
                print len(g_terms)
                if len(g_terms) >= MAX_ENTRIES:
                    print 'breaking'
                    g_bet_loop = False

        # Update screen
        draw_starting_screen()
        pygame.display.update()
        fps_clock.tick(FPS)

    # Clean up Twitter feed and pygame
    print str(pygame.time.get_ticks())
    tf.stop_tracking()
    print str(pygame.time.get_ticks())
    
    pygame.quit()

    # Print terms
    print 'Search terms: ', g_terms

# Run main
main()
