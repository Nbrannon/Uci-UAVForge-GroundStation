using namespace std;
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <utility>
#include <stdlib.h>
#include <vector>
#include <limits>
#include <algorithm>


struct twoPoints {
    std::pair<float,float> point1;
    std::pair<float,float> point2;
} ;

struct Point {
    std::pair<float,float> point;
    int visited;
    Point(pair<float,float> x, int y) {
        point = x;
        visited = y;
    }
} ;

pair<float,float> quadcopterc = make_pair(3,4);
pair<float,float> goal = make_pair(1000,700);
pair<float,float> min_obstacle;
int delta2 = 0;
float R = 5;
pair<float,float> current_obstacle1;
pair<float,float> current_obstacle2;

float pointDistance(float x1,float y1, float x2, float y2) {
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

std::pair<float,float> distance(float x1,float y1,float x2,float y2) {
    float distx = x2 - x1;
    float disty = y2 - y1;
    pair <float,float> dist;
    dist = make_pair (distx,disty);
    return dist;
}

// Dot product between two vectors
float dotProduct(float x1,float y1,float x2,float y2) {
    return x1*x2 + y1*y2;
}

// Length of a vector (used for normalize)
float length(float x1,float y1) {
    return sqrt(pow(x1,2) + pow(y1,2));
}

// Normalizing a vector
std::pair<float,float> normalize(float x1, float y1) { 
	// turn a total value into a constant amount (hard to describe)
    // (0,50) becomes(0.0,1.0)
    // (50,50) becomes (0.707, 0.707)
    float l = length(x1,y1);
    if (l != 0) {
    	x1 = x1/l;
    	y1 = y1/l;
    	pair <float,float> v = make_pair(x1,y1);
        return v;
    }
    pair <float,float> v = make_pair(1,1);
    return v;
}

// Check if line segment intersects with a circle
// closest_point_on_seg and segment_circle are explained in the reference below 
// (reference) http://doswa.com/2009/07/13/circle-segment-intersectioncollision.html

std::pair<float,float> closest_point_on_seg(pair<float,float> seg_a, pair<float,float> seg_b, pair<float,float> circ_pos) {
    pair <float,float> seg_v = distance(seg_a.first,seg_a.second,seg_b.first,seg_b.second);
    pair <float,float> pt_v = distance(seg_a.first,seg_a.second,circ_pos.first,circ_pos.second);
    if (length(seg_v.first,seg_v.second) <= 0) {
        throw "Invalid segment length";
    }
    pair <float,float> seg_v_unit = make_pair(seg_v.first/length(seg_v.first,seg_v.second),seg_v.second/length(seg_v.first,seg_v.second));
   	float proj = dotProduct(pt_v.first,pt_v.second,seg_v_unit.first,seg_v_unit.second);
    if (proj <= 0) {
        return seg_a;
    }
    if (proj >= length(seg_v.first,seg_v.second)) {
        return seg_b;
    }
    pair <float,float> proj_v = make_pair(seg_v_unit.first*proj, seg_v_unit.second*proj);
    pair <float,float> closest = make_pair(proj_v.first + seg_a.first, proj_v.second + seg_a.second);
    return closest;
}


bool segment_circle(pair<float,float> seg_a, pair<float,float> seg_b, pair<float,float> circ_pos,float circ_rad) {
    pair <float,float> closest = closest_point_on_seg(seg_a, seg_b, circ_pos);
    pair <float,float> dist_v = make_pair(circ_pos.first - closest.first,circ_pos.second - closest.second);
    if (length(dist_v.first,dist_v.second) <= circ_rad) {
        return true;
    }
    else {
        return false;
    }
}

twoPoints calculateTangentLines(pair<float,float> obstacle) {
    int tangent_line_offset = 5;
    float x1 = obstacle.first;
    float y1 = obstacle.second;
    float D = sqrt(pow(obstacle.first - quadcopterc.first,2) + pow(obstacle.second - quadcopterc.second,2));
    float L = sqrt(fabs(pow(D,2) - pow(R + tangent_line_offset,2)));
    float h = (R + tangent_line_offset)*L/D;
    float a = (pow(L,2) - pow(R + tangent_line_offset,2) + pow(D,2))/(2*D);
    float x2 = quadcopterc.first + a*(x1 - quadcopterc.first)/D;
    float y2 = quadcopterc.second + a*(y1 - quadcopterc.second)/D;
    float x31 = x2 - h * (y1 - quadcopterc.second) / D;
    float y31 = y2 + h * (x1 - quadcopterc.first) / D;
    float x32 = x2 + h * (y1 - quadcopterc.second) / D;
    float y32 = y2 - h * (x1 - quadcopterc.first) / D;
    // extend the quadcopter - tangent line segment
    // http://stackoverflow.com/questions/7740507/extend-a-line-segment-a-specific-distance
    // you can return x31,y31,x32,y32 instead of x311,y311,x321,y321 to see the difference
    float x311 = x31 + (x31 - quadcopterc.first)/pointDistance(x31,y31,quadcopterc.first,quadcopterc.second)*50;
    float y311 = y31 + (y31 - quadcopterc.second)/pointDistance(x31,y31,quadcopterc.first,quadcopterc.second)*50;
    float x321 = x32 + (x32 - quadcopterc.first)/pointDistance(x32,y32,quadcopterc.first,quadcopterc.second)*50;
    float y321 = y32 + (y32 - quadcopterc.second)/pointDistance(x32,y32,quadcopterc.first,quadcopterc.second)*50;
    twoPoints result;
    result.point1 = make_pair(x311,y311);
    result.point2 = make_pair(x321,y321);
    return result;
}

std::pair<float,float> updatePoint(pair<float,float> point) {
    if(length(point.first - quadcopterc.first,point.second - quadcopterc.second)==0) {
        ;
    }
    else {
        point.first += (point.first - quadcopterc.first)/length(point.first - quadcopterc.first,point.second - quadcopterc.second);
    }
    if(length(point.first - quadcopterc.first,point.second - quadcopterc.second)==0) {
        ;
    }
    else {
        point.second += (point.second - quadcopterc.second)/length(point.first - quadcopterc.first,point.second - quadcopterc.second);
    }
    return make_pair(point.first,point.second);
}


twoPoints closest_tangents(vector<vector<pair<float,float> > > &obstacles_bucket) {
    std::vector<std::pair<float,float> > obstcls;
    for (int i=0;i<obstacles_bucket.size();i++) {
        for (int j=0;j<obstacles_bucket[i].size();j++) {
            std::vector<pair<float,float> >::iterator it;
            it = find(obstacles_bucket[i].begin(), obstacles_bucket[i].end(), min_obstacle);
            if(it != obstacles_bucket[i].end()) {
                obstcls = obstacles_bucket[i];
            }
        }
    }
    std::vector<twoPoints> tangents;
    for (int i=0;i<obstcls.size();i++) {
        twoPoints tang = calculateTangentLines(obstcls[i]);
        twoPoints tang1;
        tang1.point1 = tang.point1,
        tang1.point2 = obstcls[i];
        tangents.push_back(tang1);
        twoPoints tang2;
        tang2.point1 = tang.point2,
        tang2.point2 = obstcls[i];
        tangents.push_back(tang2);
    }
    float angle = 100;
    float x31 = 0;
    float y31 = 0;
    float x32 = 0;
    float y32 = 0;
    for (int i=0;i<tangents.size();i++) {
        for (int j=0;j<tangents.size();j++) {
            if (i==j) continue;
            float u1 = tangents[i].point1.first - quadcopterc.first;
            float u2 = tangents[i].point1.second - quadcopterc.second;
            float v1 = tangents[j].point1.first - quadcopterc.first;
            float v2 = tangents[j].point1.second - quadcopterc.second;
            float cosi = (u1*v1 + u2*v2)/((sqrt(pow(u1,2) + pow(u2, 2)))*(sqrt(pow(v1,2) + pow(v2,2))));
            if (cosi<angle) {
                angle = cosi;
                x31 = tangents[i].point1.first;
                y31 = tangents[i].point1.second;
                x32 = tangents[j].point1.first;
                y32 = tangents[j].point1.second;
                current_obstacle1 = tangents[i].point2;
                current_obstacle2 = tangents[j].point2;
            }
            
        }
    }

    twoPoints result;
    result.point1 = make_pair(x31,y31);
    result.point2 = make_pair(x32,y32);
    return result;

}

void closest_obstacle(vector<vector<pair<float,float> > > &obstacles_bucket) {
    delta2 = 0;
    int min_distance = std::numeric_limits<int>::max();
    min_obstacle = obstacles_bucket[0][0];
    for (int i=0;i<obstacles_bucket.size();i++) {
        for (int j=0;j<obstacles_bucket[i].size();j++) {
            bool delta = segment_circle(quadcopterc, goal, obstacles_bucket[i][j], R);
            if(delta) {
                delta2 = 1;
                float distanc = pointDistance(obstacles_bucket[i][j].first, obstacles_bucket[i][j].second, quadcopterc.first, quadcopterc.second);
                if (distanc<min_distance) {
                    min_distance = distanc;
                    min_obstacle = obstacles_bucket[i][j];
                }
            }
        }
    }
    twoPoints tangs = closest_tangents(obstacles_bucket);
    for (int i=0;i<obstacles_bucket.size();i++) {
        for (int j=0;j<obstacles_bucket[i].size();j++) {
            bool ob1 = segment_circle(tangs.point1,quadcopterc,obstacles_bucket[i][j],R);
            bool ob2 = segment_circle(tangs.point2,quadcopterc,obstacles_bucket[i][j],R);
            if (ob1 || ob2) {
                min_obstacle = obstacles_bucket[i][j];
            }
        }
    }
}





main()
{

    // Tests
    printf("Point distance test, expected 321.00623046912966: Actual %f\n", pointDistance(22,32,343,34));
    pair <float,float> test_dist = distance(34.4,45,53,62.4);
    printf("Difference in coordinates between two points, expected (18.6, 17.4) %f %f\n", test_dist.first, test_dist.second);
    printf("Dot product between two vectors, expected 27.31: Actual %f\n", dotProduct(1.1,2.3,5.8,9.1));
	printf("Length of a vector (used for normalize), expected 5.594640292279745: Actual %f\n", length(5.1,2.3));
	pair <float,float> test_norm1 = normalize(-3.3,4.5);
	pair <float,float> test_norm2 = normalize(0,0);
	printf("Normalizing a vector, expected (-0.5913636636275174, 0.8064049958557056), (1,1): Actual %f %f %f %f\n", test_norm1.first,test_norm1.second,test_norm2.first,test_norm2.second);
	printf("Check if line segment intersects with a circle, expected True(1), False(0): Actual %d %d\n", segment_circle(make_pair(3,4),make_pair(400,400),make_pair(200,200),50),segment_circle(make_pair(3,4),make_pair(400,400),make_pair(500,200),50));
    twoPoints test_tang = calculateTangentLines(make_pair(3,1)); 
    printf("Calculating Tangent Lines, with R = 5, quadcopterc = (3,4), obstalce (3,1): Expected (84.79797338056486, 4.0, -78.79797338056486, 4.0), Actual %f %f %f %f\n", test_tang.point1.first, test_tang.point1.second, test_tang.point2.first, test_tang.point2.second);
    pair <float,float> point = updatePoint(make_pair(2,2));
    printf("Expecting points (1.5527864045000421, 1.18985352037725), Actual: %f %f\n", point.first, point.second);


    // Actual code
    std::vector<pair<float,float> > obstacles = {make_pair(100,200),make_pair(300,200),make_pair(1100,100),make_pair(1000,150),make_pair(500,200),make_pair(900,250),make_pair(800,350),make_pair(700,450),make_pair(500,500),make_pair(600,650),make_pair(600,550),make_pair(300,700),make_pair(800,650),make_pair(100,600),make_pair(150,650)};
    std::vector<std::vector<pair<float,float > > > obstacles_bucket;
    pair <float, float> previous_point = obstacles[0];
    std::vector<pair<float,float> > temp_bucket;
    temp_bucket.push_back(previous_point);
    for (int i=1;i<obstacles.size();i++) {
        if (pointDistance(previous_point.first,previous_point.second,obstacles[i].first,obstacles[i].second)<2*R) {
            temp_bucket.push_back(obstacles[i]);
        }
        else {
            obstacles_bucket.push_back(temp_bucket);
            temp_bucket = {obstacles[i]};
        }
        previous_point = obstacles[i];
    }
    
    
}