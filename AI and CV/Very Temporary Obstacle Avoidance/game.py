import sys, pygame, os
from time import sleep
import random
from pygame.locals import *
import math



def distance(x1,y1,x2,y2):
    distx = x2 - x1
    disty = y2 - y1
    return (distx, disty)

def length(vector): # get length (used for normalize)
        return math.sqrt((vector[0]**2 + vector[1]**2)) 

def normalize(vector): # turn a total value into a constant ammount (hard to describe)
                         # (0,50) becomes(0.0,1.0)
                         # (50,50) becomes (0.707, 0.707)
    l = length(vector)
    if l != 0:
        return (vector[0] / l, vector[1] / l)
    return None

def distance_check(self, dist):
    dist_x = dist[0] ** 2 # gets absolute value of the x distance
    dist_y = dist[1] ** 2 # gets absolute value of the y distance
    t_dist = dist_x + dist_y # gets total absolute value distance
    speed = self.speed ** 2 # gets aboslute value of the speed

    if t_dist < (speed): # read function description above
        return True


pygame.init()
screen = pygame.display.set_mode((1200, 800))
size = width, height = 1200, 800

black = 250, 250, 250
goal = (800, 400)

ball = pygame.image.load("ball.bmp")
quadcopter = ball.get_rect()
ball2 = pygame.image.load("tree.png")
tree = ball2.get_rect()
tree = tree.move(400,120)
R = int(tree.width/2*math.sqrt(2))
x1 = tree.centerx
y1 = tree.centery
while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    D2 = quadcopter.centerx*goal[1] - goal[0]*quadcopter.centery
    dp = math.sqrt(math.pow(quadcopter.centerx - goal[0],2) + math.pow(quadcopter.centery - goal[1],2))
    delta = math.pow(R,2)*math.pow(dp,2) - math.pow(D2,2)


    speed = 2    

    
    D = int(math.sqrt(math.pow(tree.centerx - quadcopter.centerx, 2) + math.pow(tree.centery - quadcopter.centery,2)))
    L = int(math.sqrt(math.fabs(math.pow(D,2) - math.pow(R,2))))
    x0 = quadcopter.centerx
    y0 = quadcopter.centery
    h = R*L/D
    a = (math.pow(L,2) - math.pow(R,2) + math.pow(D,2))/(2*D)
    x2 = x0 + a * (x1 - x0) / D
    y2 = y0 + a * (y1 - y0) / D
    x31 = x2 - h * (y1 - y0) / D
    y31 = y2 + h * (x1 - x0) / D
    x32 = x2 + h * (y1 - y0) / D
    y32 = y2 - h * (x1 - x0) / D
    
    print(quadcopter.centery)
    if (delta<0):
        direction = normalize(distance(quadcopter.centerx,quadcopter.centery,goal[0],goal[1]))
    else:
        if(math.sqrt(math.pow(x31-goal[0], 2) + math.pow(y31-goal[1], 2)) > math.sqrt(math.pow(x32-goal[0], 2) + math.pow(y32-goal[1], 2))):
            direction = normalize(distance(quadcopter.centerx,quadcopter.centery,x32,y32))
            if (math.sqrt(math.pow(quadcopter.centerx - x32,2) + math.pow(quadcopter.centery - y32, 2)) < speed):
                speed = 1
        else:
            direction = normalize(distance(quadcopter.centerx,quadcopter.centery,x31,y31))
            if (math.sqrt(math.pow(quadcopter.centerx - x31,2) + math.pow(quadcopter.centery - y31, 2)) < speed):
                speed = 1


    screen.fill(black)
    screen.blit(ball, quadcopter)
    screen.blit(ball2, tree)
    pygame.draw.circle(screen, (0, 127, 255), tree.center, R, 1)
    pygame.draw.circle(screen, (0, 127, 255), goal, 2, 1)
    pygame.draw.line(screen, (70, 127, 40), quadcopter.center, goal, 2)
    pygame.draw.line(screen, (70, 127, 255), quadcopter.center, (x31, y31), 2)
    pygame.draw.line(screen, (70, 127, 255), quadcopter.center, (x32, y32), 2)
    pygame.display.flip()


     #greedyGoal = int(math.sqrt(math.fabs(math.pow((x0-x1), 2), math.pow((y0-y1), 2))))
     

    quadcopter.centerx += speed*direction[0]
    quadcopter.centery += speed*direction[1]
    




    sleep(0.0001)

