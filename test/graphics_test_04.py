import os
import pygame
import time
import signal
import sys
from pyscope import pyscope

# Constants
FPS = 30        # Frames per second
screen = pygame.display.set_mode([640,480])
screen.fill([255,255,255])

# Main
def main():
    global FPSCLOCK, mainloop
    mainloop = True

    # Setup display
    pygame.init()
    scope = pyscope()
    FPSCLOCK = pygame.time.Clock()

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
