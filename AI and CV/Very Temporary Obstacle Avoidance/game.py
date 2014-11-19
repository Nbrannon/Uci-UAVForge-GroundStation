import sys, pygame, os
from time import sleep
import random
from pygame.locals import *
import math
import numpy as np


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
    return ([1,1])

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
tree = tree.move(300,150)
R = tree.width/2*math.sqrt(2)
x1 = tree.centerx
y1 = tree.centery
slope1 = 1
slope2 = 1
slope3 = 1
direction=([1,1])
while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()


    px1 = quadcopter.centerx - tree.centerx
    py1 = quadcopter.centery - tree.centery
    px2 = goal[0] - tree.centerx
    py2 = goal[1] - tree.centery
    dx = px2 - px1
    dy = py2 - py1
    dr = math.sqrt(math.pow(dx,2) + math.pow(dy,2))
    D2 = px1*py2 - px2*py1
    delta = R*R*dr*dr - D2*D2


    speed = 2

    D = math.sqrt(math.pow(tree.centerx - quadcopter.centerx, 2) + math.pow(tree.centery - quadcopter.centery,2))
    L = math.sqrt(math.fabs(math.pow(D,2) - math.pow(R+15,2)))
    x0 = quadcopter.centerx
    y0 = quadcopter.centery
    h = R*L/D
    a = (math.pow(L,2) - math.pow(R+15,2) + math.pow(D,2))/(2*D)
    x2 = x0 + a * (x1 - x0) / D
    y2 = y0 + a * (y1 - y0) / D
    x33 = x2 - h * (y1 - y0) / D
    y33 = y2 + h * (x1 - x0) / D
    x34 = x2 + h * (y1 - y0) / D
    y34 = y2 - h * (x1 - x0) / D
    try:
        x31 = x33 + (x33 - quadcopter.centerx)/length([x33-quadcopter.centerx,y33-quadcopter.centery])*70
    except ZeroDivisionError:
        x31 = x33
    try:
        y31 = y33 + (y33 - quadcopter.centery)/length([x33-quadcopter.centerx,y33-quadcopter.centery])*70
    except:
        y31 = y33
    try:
        x32 = x34 + (x34 - quadcopter.centerx)/length([x34-quadcopter.centerx,y34-quadcopter.centery])*70
    except:
        x32 = x34
    try:
        y32 = y34 + (y34 - quadcopter.centery)/length([x34-quadcopter.centerx,y34-quadcopter.centery])*70
    except:
        y32 = y34
    if(goal[0] - quadcopter.centerx != 0):
        slope1 = math.fabs((float(goal[1])-quadcopter.centery)/(goal[0] - quadcopter.centerx))
    if(x31 - quadcopter.centerx != 0):
        slope2 = math.fabs((float(y31)-quadcopter.centery)/(x31 - quadcopter.centerx))
    if(x32 - quadcopter.centerx != 0):
        slope3 = math.fabs((float(y32)-quadcopter.centery)/(x32 - quadcopter.centerx))
    if (delta>0):
        if(math.sqrt(math.pow(x31-goal[0], 2) + math.pow(y31-goal[1], 2)) > math.sqrt(math.pow(x32-goal[0], 2) + math.pow(y32-goal[1], 2))):
            direction = normalize(distance(quadcopter.centerx,quadcopter.centery,x32,y32))
            quadcopter.centerx += speed*direction[0]
            quadcopter.centery += speed*direction[1]
        else:
            direction = normalize(distance(quadcopter.centerx,quadcopter.centery,x31,y31))
            quadcopter.centerx += speed*direction[0]
            quadcopter.centery += speed*direction[1]

    else:

        direction = normalize(distance(quadcopter.centerx,quadcopter.centery,goal[0],goal[1]))
        quadcopter.centerx += speed*(direction[0])
        quadcopter.centery += speed*(direction[1])

    screen.fill(black)
    screen.blit(ball, quadcopter)
    screen.blit(ball2, tree)
    pygame.draw.circle(screen, (0, 127, 255), tree.center, int(R), 1)
    pygame.draw.circle(screen, (0, 127, 255), goal, 2, 1)
    pygame.draw.line(screen, (70, 127, 40), quadcopter.center, goal, 2)
    pygame.draw.line(screen, (70, 127, 255), quadcopter.center, (x31, y31), 2)
    pygame.draw.line(screen, (70, 127, 255), quadcopter.center, (x32, y32), 2)
    pygame.display.flip()


     #greedyGoal = int(math.sqrt(math.fabs(math.pow((x0-x1), 2), math.pow((y0-y1), 2))))
     





