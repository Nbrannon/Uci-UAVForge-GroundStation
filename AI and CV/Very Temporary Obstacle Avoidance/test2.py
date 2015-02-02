import sys, os
import pygame
from time import sleep
import random
from pygame.locals import *
import math
#import numpy as np

# Calculate distane between two points
def pointDistance(p1,p2):
    return math.sqrt(math.pow(p1[0]-p2[0],2) + math.pow(p1[1]-p2[1],2))

# Calculate the difference in coordinates between two points
def distance(x1,y1,x2,y2):
    distx = x2 - x1
    disty = y2 - y1
    return (distx, disty)

# Dot product between two vectors
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


# Check if line segment intersects with a circle
# closest_point_on_seg and segment_circle are explained in the reference below 
# (reference) http://doswa.com/2009/07/13/circle-segment-intersectioncollision.html

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
    # Adds offset to the tangent lines, so they are drawn a bit away from the circle
    # You can set a different tangent_line_offset (0,20...) to see the difference
    tangent_line_offset = 5
    x1 = obstacle[0]
    y1 = obstacle[1]
    D = math.sqrt(math.pow(obstacle[0] - quadcopterc[0], 2) + math.pow(obstacle[1] - quadcopterc[1],2))
    L = math.sqrt(math.fabs(math.pow(D,2) - math.pow(R+tangent_line_offset,2)))
    h = (R+tangent_line_offset)*L/D
    a = (math.pow(L,2) - math.pow(R+tangent_line_offset,2) + math.pow(D,2))/(2*D)
    x2 = quadcopterc[0] + a * (x1 - quadcopterc[0]) / D
    y2 = quadcopterc[1] + a * (y1 - quadcopterc[1]) / D
    x31 = x2 - h * (y1 - quadcopterc[1]) / D
    y31 = y2 + h * (x1 - quadcopterc[0]) / D
    x32 = x2 + h * (y1 - quadcopterc[1]) / D
    y32 = y2 - h * (x1 - quadcopterc[0]) / D
    # extend the quadcopter - tangent line segment
    # http://stackoverflow.com/questions/7740507/extend-a-line-segment-a-specific-distance
    # you can return x31,y31,x32,y32 instead of x311,y311,x321,y321 to see the difference
    x311 = x31 + (x31 - quadcopterc[0])/pointDistance((x31,y31),quadcopterc)*50
    y311 = y31 + (y31 - quadcopterc[1])/pointDistance((x31,y31),quadcopterc)*50
    x321 = x32 + (x32 - quadcopterc[0])/pointDistance((x32,y32),quadcopterc)*50
    y321 = y32 + (y32 - quadcopterc[1])/pointDistance((x32,y32),quadcopterc)*50
    return (x311,y311,x321,y321)




# Update the coordinates of the point based on the direction we are supposed to go
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


# Draw a line from the quadcopter to the goal. Finds the first obstacle that the line intersects. 
# Also returns delta2 which shows if the line intersects a circle or the parth is clear.
def closest_obstacle(obstacles_bucket):
    # Initiallize all the variables that we will need
    # By default delta2 is 0, meaning we don't intersect any circle in our path
    delta2 = 0
    min_distance = sys.maxint
    min_obstacle = obstacles_bucket[0][0]
    for bucket in obstacles_bucket:
        for obstacle in bucket:
            # Here we calculate for every circle if it intersects with our path
            delta = segment_circle(quadcopterc,goal,obstacle.center,R)
            if(delta):
                delta2 = 1
                distanc = pointDistance(obstacle, quadcopterc)
                # If it does we also have to check if the distance to it is lower than some other obstacle already found
                if(distanc<min_distance):
                    min_distance = distanc
                    min_obstacle = obstacle
    # The code below solves an edge scenatio. When we find the tangent lines one of the two of them is our path(temporary).
    # However this tangent line can intersect another circle.
    # If it does we set the obstacle that it intersects to be our "min_obstacle"
    x31,y31,x32,y32,current_obstacle1,current_obstacle2 = closest_tangents(min_obstacle)
    for bucket in obstacles_bucket:
        for obstacle in bucket:
            ob1 = segment_circle((x31,y31),quadcopterc,obstacle.center,R)
            ob2 = segment_circle((x32,y32),quadcopterc,obstacle.center,R)
            if (ob1 or ob2):
                min_obstacle = obstacle
    return min_obstacle,delta2

# Here we find the two tangent lines we can go, given that we know which closest obstacle on our way
def closest_tangents(min_obstacle):
    obstcls = []
    # First we find in which bucket our obstacle is in
    for bucket in obstacles_bucket:
        if min_obstacle in bucket:
            obstcls = bucket
    tangents = []
    # We calculate all the tangent lines of all the obstacles in the bucket
    for obstacle in obstcls:
        (x31,y31,x32,y32) = calculateTangentLines(obstacle)
        tangents.append((x31,y31,obstacle))
        tangents.append((x32,y32,obstacle))
    angle = 100
    x31 = 0
    y31 = 0
    x32 = 0
    y32 = 0
    # We have to pick the two tangent lines who goes around all the circle in the bucket
    # An easy way to find these obstacles in to find the two tangent lines who have the biggest angle between them
    for tangent1 in tangents:
        for tangent2 in tangents:
            if(tangent1==tangent2):
                continue
            u1 = tangent1[0] - quadcopterc[0]
            u2 = tangent1[1] - quadcopterc[1]
            v1 = tangent2[0] - quadcopterc[0]
            v2 = tangent2[1] - quadcopterc[1]
            # We can find the largest angle between two lines as cos between 0 and 180 degrees is injective function
            # You can see that from the plot of the cos - http://www.efunda.com/math/trig_functions/images/cos_plot.gif
            # How to find cos between two vectors - http://www.wikihow.com/Find-the-Angle-Between-Two-Vectors
            cos = (u1*v1 + u2*v2)/((math.sqrt(math.pow(u1,2) + math.pow(u2, 2)))*(math.sqrt(math.pow(v1,2) + math.pow(v2,2))))
            if (cos<angle):
                angle = cos
                x31 = tangent1[0]
                y31 = tangent1[1]
                x32 = tangent2[0]
                y32 = tangent2[1]
                current_obstacle1 = tangent1[2]
                current_obstacle2 = tangent2[2]
            else:
                continue
    # Here we return the coordinates of the two tangents as well as the obstacles the tangents are part of
    return x31,y31,x32,y32,current_obstacle1,current_obstacle2


#Test
R = 5
quadcopterc = [3,4]
obstacle = [3,1]
#result - (84.79797338056486, 4.0, -78.79797338056486, 4.0)
goal = (1000, 700)
points = [(100,200),(300,200),(1100,100),(1000,150),(500,200),(900,250),(800,350),(700,450),(500,500),(600,650),(600,550),(300,700),(800,650),(100,600),(150,650)]
obstacles = []
obstacles_bucket = []
pic = pygame.image.load("tree3.png")

for point in points:
    obstacle = pic.get_rect()
    obstacles.append([obstacle,1])

obstacles.append(2);
for obstacle in obstacles:
    obstacles.append(1)
    print(obstacles)

