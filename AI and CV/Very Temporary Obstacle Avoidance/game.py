import sys, pygame, os
from time import sleep
import random
from pygame.locals import *
import math
import numpy as np

def pointDistance(p1,p2):
    return math.sqrt(math.pow(p1[0]-p2[0],2) + math.pow(p1[1]-p2[1],2))

def distance(x1,y1,x2,y2):
    distx = x2 - x1
    disty = y2 - y1
    return (distx, disty)

def length(vector): # get length (used for normalize)
    return math.sqrt((vector[0]**2 + vector[1]**2))


def normalize(vector): # turn a total value into a constant amount (hard to describe)
                         # (0,50) becomes(0.0,1.0)
                         # (50,50) becomes (0.707, 0.707)
    l = length(vector)
    if l != 0:
        return (vector[0] / l, vector[1] / l)
    return ([1,1])




def calculateDelta(obstacle):
    # We have to subtract tree x and y because our circle does not have coordinates (0,0)
    px1 = quadcopterc[0] - obstacle.centerx
    py1 = quadcopterc[1] - obstacle.centery
    px2 = goal[0] - obstacle.centerx
    py2 = goal[1] - obstacle.centery
    dx = px2 - px1
    dy = py2 - py1
    dr = math.sqrt(math.pow(dx,2) + math.pow(dy,2))
    D2 = px1*py2 - px2*py1
    delta = R*R*dr*dr - D2*D2
    return delta

def calculateTangentLines(obstacle):
    x1 = obstacle.centerx
    y1 = obstacle.centery
    D = math.sqrt(math.pow(obstacle.centerx - quadcopterc[0], 2) + math.pow(obstacle.centery - quadcopterc[1],2))
    L = math.sqrt(math.fabs(math.pow(D,2) - math.pow(R,2)))
    h = R*L/D
    a = (math.pow(L,2) - math.pow(R,2) + math.pow(D,2))/(2*D)
    x2 = quadcopterc[0] + a * (x1 - quadcopterc[0]) / D
    y2 = quadcopterc[1] + a * (y1 - quadcopterc[1]) / D
    x31 = x2 - h * (y1 - quadcopterc[1]) / D
    y31 = y2 + h * (x1 - quadcopterc[0]) / D
    x32 = x2 + h * (y1 - quadcopterc[1]) / D
    y32 = y2 - h * (x1 - quadcopterc[0]) / D
    return (x31,y31,x32,y32)


def updatePoint(x,y):
    try:
        x += (x - quadcopter.centerx)/length([x-quadcopter.centerx,y-quadcopter.centery])
    except ZeroDivisionError:
        pass
    try:
        y += (y - quadcopter.centery)/length([x-quadcopter.centerx,y-quadcopter.centery])
    except ZeroDivisionError:
        pass
    return (x,y)




pygame.init()
screen = pygame.display.set_mode((1200, 800))
size = width, height = 1200, 800

black = 250, 250, 250
goal = (800, 400)
ball = pygame.image.load("ball.bmp")
quadcopter = ball.get_rect()
quadcopterc = [quadcopter.centerx, quadcopter.centery]

pic = pygame.image.load("tree2.png")
tree = pic.get_rect()
tree = tree.move(-tree.width/2,-tree.height/2)
R = tree.width/2*math.sqrt(2)

points = [(400,150),(300,200),(1000,700),(300,700),(500,400)]
obstacles = []
obstacles_bucket = []


for point in points:
    obstacle = pic.get_rect()
    obstacle = obstacle.move(-obstacle.width/2 + point[0],-obstacle.height/2 + point[1])
    obstacles.append([obstacle,1])

for bucket in obstacles:
    if (bucket[1]==0):
        continue
    temp_bucket = [bucket[0]]
    for obstacle in temp_bucket:
        try:
            i = obstacles.index([obstacle,1])
            obstacles[i][1] = 0
        except:
            pass
        for neighbour in obstacles:
            if(neighbour[1]==0):
                continue
            if(obstacle==neighbour[0]):
                continue
            if(pointDistance(obstacle.center,neighbour[0].center)>2*R):
                continue
            temp_bucket.append(neighbour[0])
            try:
                i = obstacles.index([neighbour[0],1])
                obstacles[i][1] = 0
            except:
                pass
    obstacles_bucket.append(temp_bucket)






direction=([1,1])
speed = 7
while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    delta2 = 0
    min_distance = sys.maxint
    min_obstacle = obstacles_bucket[0][0]
    for bucket in obstacles_bucket:
        for obstacle in bucket:
            # delta = calculateDelta(obstacle)
            delta = calculateDelta(obstacle)
            if(delta>0 and distance(quadcopterc[0], quadcopterc[1], obstacle.centerx, obstacle.centery) < distance(quadcopterc[0],quadcopterc[1],goal[0],goal[1])):
                delta2 = 1
                distanc = pointDistance(obstacle, quadcopterc)
                if(distanc<min_distance):
                    min_distance = distanc
                    min_obstacle = obstacle
    tree = min_obstacle
    obstcls = []
    for bucket in obstacles_bucket:
        if min_obstacle in bucket:
            obstcls = bucket
    tangents = []
    for obstacle in obstcls:
        (x31,y31,x32,y32) = calculateTangentLines(obstacle)
        tangents.append((x31,y31))
        tangents.append((x32,y32))
    angle = 100
    x31 = 0
    y31 = 0
    x32 = 0
    y32 = 0
    for tangent1 in tangents:
        for tangent2 in tangents:
            if(tangent1==tangent2):
                continue
            u1 = tangent1[0] - quadcopter.centerx
            u2 = tangent1[1] - quadcopter.centery
            v1 = tangent2[0] - quadcopter.centerx
            v2 = tangent2[1] - quadcopter.centery
            cos = (u1*v1 + u2*v2)/((math.sqrt(math.pow(u1,2) + math.pow(u2, 2)))*(math.sqrt(math.pow(v1,2) + math.pow(v2,2))))
            if (cos<angle):
                angle = cos
                x31 = tangent1[0]
                y31 = tangent1[1]
                x32 = tangent2[0]
                y32 = tangent2[1]
            else:
                continue


    # Checking if our path is clear using circle intersection
    # More info - http://mathworld.wolfram.com/Circle-LineIntersection.html

   # delta2 = calculateDelta(tree2)


    # Calculating the tangent lines


   # (x312,y312,x322,y322) = calculateTangentLines(tree2)

    if (delta2>0):
        if(length([x31-goal[0],y31-goal[1]]) > length([x32-goal[0], y32-goal[1]])):
            (x32,y32) = updatePoint(x32,y32)
            direction = normalize(distance(quadcopterc[0],quadcopterc[1],x32,y32))
            quadcopterc[0] += speed*direction[0]
            quadcopterc[1] += speed*direction[1]
        else:
            (x31,y31) = updatePoint(x31,y31)
            direction = normalize(distance(quadcopterc[0],quadcopterc[1],x31,y31))
            quadcopterc[0] += speed*direction[0]
            quadcopterc[1] += speed*direction[1]

    else:
        direction = normalize(distance(quadcopterc[0],quadcopterc[1],goal[0],goal[1]))
        quadcopterc[0] += speed*(direction[0])
        quadcopterc[1] += speed*(direction[1])

    quadcopter.center = quadcopterc
    screen.fill(black)
    screen.blit(ball, quadcopter)
    screen.blit(pic, tree)
    for obstacle in obstacles:
        screen.blit(pic,obstacle[0])
        pygame.draw.circle(screen, (0, 127, 255), obstacle[0].center, int(R), 2)

    pygame.draw.circle(screen, (0, 127, 255), goal, 2, 1)
    pygame.draw.line(screen, (70, 127, 40), quadcopter.center, goal, 2)
    if(delta2>0):
        pygame.draw.line(screen, (70, 20, 100), quadcopter.center, (x31, y31), 2)
        pygame.draw.line(screen, (70, 20, 100), quadcopter.center, (x32, y32), 2)


   # print(direction[0]*30+quadcopter.centerx, direction[1]*30+quadcopter.centery)
    pygame.display.flip()
    pygame.time.wait(22)
   # if(math.sqrt(math.pow(quadcopter.centerx - goal[0], 2) + math.pow(quadcopter.centery - goal[1], 2)) <2):
    #    os.system("pause")







