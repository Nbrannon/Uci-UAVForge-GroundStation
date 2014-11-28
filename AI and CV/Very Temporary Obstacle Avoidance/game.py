import sys, pygame, os
from time import sleep
import random
from pygame.locals import *
import math
import numpy as np


# Distance between two points
def pointDistance(p1,p2):
    return math.sqrt(math.pow(p1[0]-p2[0],2) + math.pow(p1[1]-p2[1],2))

# Distance between coordinates of two points
def distance(x1,y1,x2,y2):
    distx = x2 - x1
    disty = y2 - y1
    return (distx, disty)

def dotProduct(v1,v2):
    return v1[0]*v2[0] + v1[1]*v2[1]

# Length of a vector (used for normalize)
def length(vector):
    return math.sqrt((vector[0]**2 + vector[1]**2))

# Normalizing a vector
def normalize(vector): # turn a total value into a constant amount (hard to describe)
                         # (0,50) becomes(0.0,1.0)
                         # (50,50) becomes (0.707, 0.707)
    l = length(vector)
    if l != 0:
        return (vector[0] / l, vector[1] / l)
    return ([1,1])



# Check if lines intersects with a circle - http://doswa.com/2009/07/13/circle-segment-intersectioncollision.html

def closest_point_on_seg(seg_a, seg_b, circ_pos):
    seg_v = distance(seg_a[0],seg_a[1],seg_b[0],seg_b[1])
    pt_v = distance(seg_a[0],seg_a[1],circ_pos[0],circ_pos[1])
    if length(seg_v) <= 0:
        raise ValueError, "Invalid segment length"
    seg_v_unit = [i/length(seg_v) for i in seg_v]
    proj = dotProduct(pt_v,seg_v_unit)
    if proj <= 0:
        return seg_a
    if proj >= length(seg_v):
        return seg_b
    proj_v = [i * proj for i in seg_v_unit]
    closest = [proj_v[0]+seg_a[0], proj_v[1] + seg_a[1]]
    return closest

def segment_circle(seg_a, seg_b, circ_pos, circ_rad):
    closest = closest_point_on_seg(seg_a, seg_b, circ_pos)
    dist_v = [circ_pos[0] - closest[0],circ_pos[1] - closest[1]]
    if length(dist_v) <= circ_rad:
        return True
    else:
        return False

# Calculate tangent lines between the quadcopter and the a specific obstacle
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

# Update point
def updatePoint(x,y):
    try:
        x += (x - quadcopterc[0])/length([x-quadcopterc[0],y-quadcopterc[1]])
    except ZeroDivisionError:
        pass
    try:
        y += (y - quadcopterc[1])/length([x-quadcopterc[0],y-quadcopterc[1]])
    except ZeroDivisionError:
        pass
    return (x,y)




pygame.init()
screen = pygame.display.set_mode((1200, 800))
size = width, height = 1200, 800

black = 250, 250, 250
goal = (1000, 700)
ball = pygame.image.load("ball.bmp")
quadcopter = ball.get_rect()
quadcopterc = [quadcopter.centerx, quadcopter.centery]
pic = pygame.image.load("tree3.png")
tree = pic.get_rect()
tree = tree.move(-tree.width/2,-tree.height/2)
R = tree.width/2*math.sqrt(2) + 30

points = [(100,200),(300,200),(500,200),(300,700),(800,350),(700,450),(600,550),(900,250)]
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
speed = 2
while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    delta2 = 0
    min_distance = sys.maxint
    min_obstacle = obstacles_bucket[0][0]
    for bucket in obstacles_bucket:
        for obstacle in bucket:
            delta = segment_circle(quadcopterc,goal,obstacle.center,R)
            if(delta):
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
            u1 = tangent1[0] - quadcopterc[0]
            u2 = tangent1[1] - quadcopterc[1]
            v1 = tangent2[0] - quadcopterc[0]
            v2 = tangent2[1] - quadcopterc[1]
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
   # if(math.sqrt(math.pow(quadcopter.centerx - goal[0], 2) + math.pow(quadcopter.centery - goal[1], 2)) <2):
    #    os.system("pause")






