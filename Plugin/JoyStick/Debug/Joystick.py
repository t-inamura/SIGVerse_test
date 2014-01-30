# This code is a modification from the code of example provided in pygame documenation. 
# Modification done by Raghvendra Jain. 


import pygame, sys
from pygame.locals import *

# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)

pygame.init()


# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputing the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)
        # Set the width and height of the screen [width,height]
        # size = [500, 700]
        # screen = pygame.display.set_mode(size)
        # pygame.display.set_caption("The Joysticks")
        #self.screen.fill(WHITE)
        # self.reset()



    def printS(self, screen, textString):
        textBitmap = self.font.render(textString, True, WHITE)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height
        
    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15
        
    def indent(self):
        self.x += 10
        
    def unindent(self):
        self.x -= 10

    def joystick(self):
        size = [500, 700]
        screen = pygame.display.set_mode(size)
        #self.screen.fill(WHITE)
        
        pygame.display.set_caption("The Joysticks")
        pygame.display.set_caption("The Joysticks")
        #Loop until the user clicks the close button.
        done = False
        # Used to manage how fast the screen updates
        clock = pygame.time.Clock()

        # Initialize the joysticks
        pygame.joystick.init()
    
        # # Get ready to print
        # self = TextPrint()

        # -------- Main Program Loop -----------
        while done==False:
            # EVENT PROCESSING STEP
            for event in pygame.event.get(): # User did something
                if event.type == pygame.QUIT: # If user clicked close
                    done = True
                    pygame.quit()

                    # done=True # Flag that we are done so we exit this loop
                    sys.exit()

        
                # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
                if event.type == pygame.JOYBUTTONDOWN:
                    print("Joystick button pressed.")
                if event.type == pygame.JOYBUTTONUP:
                    print("Joystick button released.")
 
            # DRAWING STEP
            # First, clear the screen to white. Don't put other drawing commands
            # above this, or they will be erased with this command.
            screen.fill(BLACK)
            self.reset()

            # Get count of joysticks
            joystick_count = pygame.joystick.get_count()

            self.printS(screen, "Number of joysticks: {}".format(joystick_count) )
            self.indent()
            
            # For each joystick:
            for i in range(joystick_count):
                joystick = pygame.joystick.Joystick(i)
                joystick.init()
            
                self.printS(screen, "Joystick {}".format(i+1) )
                self.indent()
            
                # Get the name from the OS for the controller/joystick
                name = joystick.get_name()
                self.printS(screen, "Joystick name: {}".format(name) )
                
                # Usually axis run in pairs, up/down for one, and left/right for
                # the other.
                axes = joystick.get_numaxes()
                self.printS(screen, "Number of axes: {}".format(axes) )
                self.indent()
                
                for j in range( axes ):
                    axis = joystick.get_axis( j )
                    self.printS(screen, "For Joystick {},  Axis {} value: {:>6.3f}".format( i+1, j+1, axis) )
                    axisData=[i+1, j+1, axis]
                    # yield axisData
                    yield axisData
                self.unindent()
    
                    
                buttons = joystick.get_numbuttons()
                self.printS(screen, "Number of buttons: {}".format(buttons) )
                self.indent()

                for j in range( buttons ):
                    button = joystick.get_button( j )
                    self.printS(screen, "Button {:>2} value: {}".format(j+1, button) )
                    yield [9,j+1,button]
                self.unindent()
                    
                # Hat switch. All or nothing for direction, not like joysticks.
                # Value comes back in an array.
                hats = joystick.get_numhats()
                self.printS(screen, "Number of hats: {}".format(hats) )
                self.indent()

                for k in range( hats ):
                    hat = joystick.get_hat( k )
                    self.printS(screen, "For Joystick {}, Hat {} value: {}".format(i+1, k+1, str(hat)) )
                    # yield i+1, k+1, str(hat)
                self.unindent()
                
                self.unindent()

            
                # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
                
                # Go ahead and update the screen with what we've drawn.
            pygame.display.flip()

            # Limit to 20 frames per second
            clock.tick(20)
    
    # Close the window and quit.
    # If you forget this line, the program will 'hang'
    # on exit if running from IDLE.
        pygame.quit ()

if __name__ == "__main__" :
    # theApp = App()
    # Get ready to print
    textPrint = TextPrint()
    a=textPrint.joystick()
    #for i in range(0,30):
    while True:
        print next(a)
    pygame.quit()
