import os
import pygame
import time
import signal
import sys
from pyscope import pyscope

# Constants
FPS = 30                # Frames per second
RESOLUTION = [540, 960] # Yes, I know the background is hardcoded. Deal.

# Common colors
WHITE = 255,255,255
GREEN = 0,255,0
BLACK = 0,0,0
BLUE  = 0,0,255
RED   = 255,0,0

# Main
def main():
    global FPSCLOCK, mainloop
    mainloop = True

    # Setup display
    screen = pygame.display.set_mode(RESOLUTION)
    pygame.init()
    scope = pyscope()
    FPSCLOCK = pygame.time.Clock()
    pygame.mouse.set_visible(False)

    # Create background
    rect_game_mode = pygame.draw.rect(screen, WHITE, (20, 20, 290, 80), 0)
    
    # Main game loop
    while mainloop:
        drawScreen()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                mainloop = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q or event.key == pygame.K_ESCAPE:
                    mainloop = False
        pygame.display.update()
        FPSCLOCK.tick(FPS)
        
    pygame.quit()

def drawScreen():
    pass
    
# Call main
main()
