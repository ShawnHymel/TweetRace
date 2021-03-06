import os
import pygame
import time
import signal
import sys
from pyscope import pyscope

# Constants
FPS = 10        # Frames per second

# Ctrl+C interrupt handler
def signal_handler(signal, frame):
    print 'Game interrupted with Ctrl+C'
    sys.exit(0)

# Main
def main():
    global FPSCLOCK, mainloop
    mainloop = True

    # Setup display
    #scope = pyscope()
    FPSCLOCK = pygame.time.Clock()

    # Main game loop
    while mainloop:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q:
                    mainloop = False
    
        handleEvents()
        drawScreen()
        pygame.display.update()
        FPSCLOCK.tick(FPS)


def handleEvents():
    pass

def drawScreen():
    pass

# Register ctrl+c event and run main
if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    main()

