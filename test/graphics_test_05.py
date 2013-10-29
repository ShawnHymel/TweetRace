import os
import pygame
import time
import signal
import sys
from pyscope import pyscope

# Constants
FPS = 30                    # Frames per second
RESOLUTION = [540, 960]     # Yes, I know the background is hardcoded. Deal.
GAME_TIME = 120000          # Game time (milliseconds)

# Common colors
WHITE = 255,255,255
GREEN = 0,255,0
BLACK = 0,0,0
BLUE  = 0,0,255
RED   = 255,0,0

# Main
def main():
    global FPSCLOCK, mainloop, screen, timer
    global font_title_1, font_title_2, font_timer
    mainloop = True

    # Setup display
    screen = pygame.display.set_mode(RESOLUTION)
    pygame.init()
    scope = pyscope()
    FPSCLOCK = pygame.time.Clock()
    pygame.mouse.set_visible(False)
    
    # Create fonts
    font_timer = pygame.font.Font(None, 96)
    font_title_1 = pygame.font.Font(None, 68)
    font_title_2 = pygame.font.Font(None, 68)
    
    # Main game loop
    while mainloop:
        drawScreen()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                mainloop = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                    mainloop = False
            elif event.type == pygame.USEREVENT+1:
                mainloop = False
        pygame.display.update()
        FPSCLOCK.tick(FPS)
        
    pygame.quit()

# Handle graphics on the screen
def drawScreen():

    # Create background
    rect_bg = pygame.draw.rect(screen, BLACK, (0, 0, 540, 960), 0)
    rect_title = pygame.draw.rect(screen, WHITE, (20, 20, 500, 100), 0)
    rect_game_mode = pygame.draw.rect(screen, WHITE, (20, 140, 300, 60), 0)
    rect_timer = pygame.draw.rect(screen, WHITE, (340, 140, 180, 60), 0)
    
    # Draw title
    title1 = "The Great American"
    title2 = "Tweet Race"
    text_title_1 = font_title_1.render(title1,1,GREEN)
    text_title_2 = font_title_2.render(title2,1,GREEN)
    screen.blit(text_title_1, (40, 25))
    screen.blit(text_title_2, (40, 70))

    # Draw remaining time
    game_time = GAME_TIME - pygame.time.get_ticks()
    if game_time <= 0:
        game_time_str = '00:00'
        event_quit = pygame.event.Event(pygame.QUIT)
        pygame.event.post(event_quit)
    else:
        game_time_min = game_time / 60000
        game_time_sec = (game_time - (game_time_min * 60000)) / 1000
        if game_time_min < 10:
            game_time_str = '0' + str(game_time_min)
        else:
            game_time_str = str(game_time_min)
        if game_time_sec < 10:
            game_time_str = game_time_str + ':0' + str(game_time_sec)
        else:
            game_time_str = game_time_str + ':' + str(game_time_sec)
    text_timer = font_timer.render(game_time_str,1,RED)
    screen.blit(text_timer, (341, 140))
    
# Call main
main()
