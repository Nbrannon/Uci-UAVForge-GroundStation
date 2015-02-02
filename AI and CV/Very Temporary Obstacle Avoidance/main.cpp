/*
 * Copyright (C) 2014  RoboPeak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
/*
 *  RoboPeak Lidar System
 *  Simple Data Grabber Demo App
 *
 *  Copyright 2009 - 2014 RoboPeak Team
 *  http://www.robopeak.com
 *
 *  An ultra simple app to fetech RPLIDAR data continuously....
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <utility>
#include <vector>
#include <limits>
#include <algorithm>
#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#define PI 3.14159265

using namespace rp::standalone::rplidar;
using namespace std;


pair<float, float> quadcopterc = make_pair(0, 0);
pair<float, float> goal = make_pair(1000, 1000);
pair<float, float> min_obstacle;
int delta2 = 0;
float R = 200;
pair<float, float> current_obstacle1;
pair<float, float> current_obstacle2;


bool checkRPLIDARHealth(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_health_t healthinfo;


    op_result = drv->getHealth(healthinfo);
    if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("RPLidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want rplidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}

struct twoPoints {
	std::pair<float, float> point1;
	std::pair<float, float> point2;
};

struct Point {
	std::pair<float, float> point;
	int visited;
	Point(pair<float, float> x, int y) {
		point = x;
		visited = y;
	}
};


float pointDistance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

std::pair<float, float> distance(float x1, float y1, float x2, float y2) {
	float distx = x2 - x1;
	float disty = y2 - y1;
	pair <float, float> dist;
	dist = make_pair(distx, disty);
	return dist;
}

// Dot product between two vectors
float dotProduct(float x1, float y1, float x2, float y2) {
	return x1*x2 + y1*y2;
}

// Length of a vector (used for normalize)
float length(float x1, float y1) {
	return sqrt(pow(x1, 2) + pow(y1, 2));
}

// Normalizing a vector
std::pair<float, float> normalize(float x1, float y1) {
	// turn a total value into a constant amount (hard to describe)
	// (0,50) becomes(0.0,1.0)
	// (50,50) becomes (0.707, 0.707)
	float l = length(x1, y1);
	if (l != 0) {
		x1 = x1 / l;
		y1 = y1 / l;
		pair <float, float> v = make_pair(x1, y1);
		return v;
	}
	pair <float, float> v = make_pair(1, 1);
	return v;
}

// Check if line segment intersects with a circle
// closest_point_on_seg and segment_circle are explained in the reference below 
// (reference) http://doswa.com/2009/07/13/circle-segment-intersectioncollision.html

std::pair<float, float> closest_point_on_seg(pair<float, float> seg_a, pair<float, float> seg_b, pair<float, float> circ_pos) {
	pair <float, float> seg_v = distance(seg_a.first, seg_a.second, seg_b.first, seg_b.second);
	pair <float, float> pt_v = distance(seg_a.first, seg_a.second, circ_pos.first, circ_pos.second);
	if (length(seg_v.first, seg_v.second) <= 0) {
		throw "Invalid segment length";
	}
	pair <float, float> seg_v_unit = make_pair(seg_v.first / length(seg_v.first, seg_v.second), seg_v.second / length(seg_v.first, seg_v.second));
	float proj = dotProduct(pt_v.first, pt_v.second, seg_v_unit.first, seg_v_unit.second);
	if (proj <= 0) {
		return seg_a;
	}
	if (proj >= length(seg_v.first, seg_v.second)) {
		return seg_b;
	}
	pair <float, float> proj_v = make_pair(seg_v_unit.first*proj, seg_v_unit.second*proj);
	pair <float, float> closest = make_pair(proj_v.first + seg_a.first, proj_v.second + seg_a.second);
	return closest;
}


bool segment_circle(pair<float, float> seg_a, pair<float, float> seg_b, pair<float, float> circ_pos, float circ_rad) {
	pair <float, float> closest = closest_point_on_seg(seg_a, seg_b, circ_pos);
	pair <float, float> dist_v = make_pair(circ_pos.first - closest.first, circ_pos.second - closest.second);
	if (length(dist_v.first, dist_v.second) <= circ_rad) {
		return true;
	}
	else {
		return false;
	}
}

twoPoints calculateTangentLines(pair<float, float> obstacle) {
	int tangent_line_offset = 5;
	float x1 = obstacle.first;
	float y1 = obstacle.second;
	float D = sqrt(pow(obstacle.first - quadcopterc.first, 2) + pow(obstacle.second - quadcopterc.second, 2));
	float L = sqrt(fabs(pow(D, 2) - pow(R + tangent_line_offset, 2)));
	float h = (R + tangent_line_offset)*L / D;
	float a = (pow(L, 2) - pow(R + tangent_line_offset, 2) + pow(D, 2)) / (2 * D);
	float x2 = quadcopterc.first + a*(x1 - quadcopterc.first) / D;
	float y2 = quadcopterc.second + a*(y1 - quadcopterc.second) / D;
	float x31 = x2 - h * (y1 - quadcopterc.second) / D;
	float y31 = y2 + h * (x1 - quadcopterc.first) / D;
	float x32 = x2 + h * (y1 - quadcopterc.second) / D;
	float y32 = y2 - h * (x1 - quadcopterc.first) / D;
	// extend the quadcopter - tangent line segment
	// http://stackoverflow.com/questions/7740507/extend-a-line-segment-a-specific-distance
	// you can return x31,y31,x32,y32 instead of x311,y311,x321,y321 to see the difference
	float x311 = x31 + (x31 - quadcopterc.first) / pointDistance(x31, y31, quadcopterc.first, quadcopterc.second) * 50;
	float y311 = y31 + (y31 - quadcopterc.second) / pointDistance(x31, y31, quadcopterc.first, quadcopterc.second) * 50;
	float x321 = x32 + (x32 - quadcopterc.first) / pointDistance(x32, y32, quadcopterc.first, quadcopterc.second) * 50;
	float y321 = y32 + (y32 - quadcopterc.second) / pointDistance(x32, y32, quadcopterc.first, quadcopterc.second) * 50;
	twoPoints result;
	result.point1 = make_pair(x311, y311);
	result.point2 = make_pair(x321, y321);
	return result;
}

std::pair<float, float> updatePoint(pair<float, float> point) {
	if (length(point.first - quadcopterc.first, point.second - quadcopterc.second) == 0) {
		;
	}
	else {
		point.first += (point.first - quadcopterc.first) / length(point.first - quadcopterc.first, point.second - quadcopterc.second);
	}
	if (length(point.first - quadcopterc.first, point.second - quadcopterc.second) == 0) {
		;
	}
	else {
		point.second += (point.second - quadcopterc.second) / length(point.first - quadcopterc.first, point.second - quadcopterc.second);
	}
	return make_pair(point.first, point.second);
}


twoPoints closest_tangents(vector<vector<pair<float, float> > > &obstacles_bucket) {
	std::vector<std::pair<float, float> > obstcls;
	for (int i = 0; i<obstacles_bucket.size(); i++) {
		for (int j = 0; j<obstacles_bucket[i].size(); j++) {
			std::vector<pair<float, float> >::iterator it;
			it = find(obstacles_bucket[i].begin(), obstacles_bucket[i].end(), min_obstacle);
			if (it != obstacles_bucket[i].end()) {
				obstcls = obstacles_bucket[i];
			}
		}
	}
	std::vector<twoPoints> tangents;
	for (int i = 0; i<obstcls.size(); i++) {
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
	for (int i = 0; i<tangents.size(); i++) {
		for (int j = 0; j<tangents.size(); j++) {
			if (i == j) continue;
			float u1 = tangents[i].point1.first - quadcopterc.first;
			float u2 = tangents[i].point1.second - quadcopterc.second;
			float v1 = tangents[j].point1.first - quadcopterc.first;
			float v2 = tangents[j].point1.second - quadcopterc.second;
			float cosi = (u1*v1 + u2*v2) / ((sqrt(pow(u1, 2) + pow(u2, 2)))*(sqrt(pow(v1, 2) + pow(v2, 2))));
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
	result.point1 = make_pair(x31, y31);
	result.point2 = make_pair(x32, y32);
	return result;

}

void closest_obstacle(vector<vector<pair<float, float> > > &obstacles_bucket) {
	delta2 = 0;
	int min_distance = std::numeric_limits<int>::max();
	min_obstacle = obstacles_bucket[0][0];
	for (int i = 0; i<obstacles_bucket.size(); i++) {
		for (int j = 0; j<obstacles_bucket[i].size(); j++) {
			bool delta = segment_circle(quadcopterc, goal, obstacles_bucket[i][j], R);
			if (delta) {
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
	for (int i = 0; i<obstacles_bucket.size(); i++) {
		for (int j = 0; j<obstacles_bucket[i].size(); j++) {
			bool ob1 = segment_circle(tangs.point1, quadcopterc, obstacles_bucket[i][j], R);
			bool ob2 = segment_circle(tangs.point2, quadcopterc, obstacles_bucket[i][j], R);
			if (ob1 || ob2) {
				min_obstacle = obstacles_bucket[i][j];
			}
		}
	}
}

int main(int argc, const char * argv[]) {
	// Tests
	printf("Point distance test, expected 321.00623046912966: Actual %f\n", pointDistance(22, 32, 343, 34));
	pair <float, float> test_dist = distance(34.4, 45, 53, 62.4);
	printf("Difference in coordinates between two points, expected (18.6, 17.4) %f %f\n", test_dist.first, test_dist.second);
	printf("Dot product between two vectors, expected 27.31: Actual %f\n", dotProduct(1.1, 2.3, 5.8, 9.1));
	printf("Length of a vector (used for normalize), expected 5.594640292279745: Actual %f\n", length(5.1, 2.3));
	pair <float, float> test_norm1 = normalize(-3.3, 4.5);
	pair <float, float> test_norm2 = normalize(0, 0);
	printf("Normalizing a vector, expected (-0.5913636636275174, 0.8064049958557056), (1,1): Actual %f %f %f %f\n", test_norm1.first, test_norm1.second, test_norm2.first, test_norm2.second);
	printf("Check if line segment intersects with a circle, expected True(1), False(0): Actual %d %d\n", segment_circle(make_pair(3, 4), make_pair(400, 400), make_pair(200, 200), 50), segment_circle(make_pair(3, 4), make_pair(400, 400), make_pair(500, 200), 50));
	twoPoints test_tang = calculateTangentLines(make_pair(3, 1));
	printf("Calculating Tangent Lines, with R = 5, quadcopterc = (3,4), obstalce (3,1): Expected (84.79797338056486, 4.0, -78.79797338056486, 4.0), Actual %f %f %f %f\n", test_tang.point1.first, test_tang.point1.second, test_tang.point2.first, test_tang.point2.second);
	pair <float, float> point = updatePoint(make_pair(2, 2));
	printf("Expecting points (1.5527864045000421, 1.18985352037725), Actual: %f %f\n", point.first, point.second);

	const char * opt_com_path = NULL;
	_u32         opt_com_baudrate = 115200;
	u_result     op_result;

	

	// read serial port from the command line...
	if (argc > 1) opt_com_path = argv[1]; // or set to a fixed value: e.g. "com3" 

	// read baud rate from the command line if specified...
	if (argc > 2) opt_com_baudrate = strtoul(argv[2], NULL, 10);


	if (!opt_com_path) {
#ifdef _WIN32
		// use default com port
		opt_com_path = "\\\\.\\com3";
#else
		opt_com_path = "/dev/ttyUSB0";
#endif
	}

	// create the driver instance
	RPlidarDriver * drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);

	if (!drv) {
		fprintf(stderr, "insufficent memory, exit\n");
		exit(-2);
	}


	// make connection...
	if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate))) {
		fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
			, opt_com_path);
		goto on_finished;
	}



	// check health...
	if (!checkRPLIDARHealth(drv)) {
		goto on_finished;
	}


	// start scan...
	drv->startScan();

	// fetech result and print it out...
	while (1) {
		std::vector<pair<float, float> > obstacles;
		std::vector<std::vector<pair<float, float > > > obstacles_bucket;
		std::vector<pair<float, float> > temp_bucket;

		rplidar_response_measurement_node_t nodes[360 * 2];
		size_t   count = _countof(nodes);
		op_result = drv->grabScanData(nodes, count);

		if (IS_OK(op_result)) {
			drv->ascendScanData(nodes, count);
			for (int pos = 0; pos < (int)count; ++pos) {
				float x = nodes[pos].distance_q2 / 4.0f;
				float angle = (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f;
				float sine = sin(angle*PI / 180);
				float cosine = cos(angle*PI / 180);
				float a = x*sine/10;
				float b = x*cosine/10;
				obstacles.push_back(make_pair(a, b));
				//printf("%f %f\n", a, b);
				/*printf("%s theta: %03.2f X coordinates: %f Y coordinates: %f \n",
					(nodes[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT) ? "S " : "  ",
					(nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f,
					a,
					b);*/
			}
		}
		std::pair<float, float> direction = make_pair(1, 1);
		pair <float, float> previous_point = obstacles[0];
		temp_bucket.push_back(previous_point);
		int bucket_counter = 0;
		for (int i = 1; i<obstacles.size(); i++) {
			if (pointDistance(previous_point.first, previous_point.second, obstacles[i].first, obstacles[i].second)<2 * R) {
				temp_bucket.push_back(obstacles[i]);
			}
			else {
				obstacles_bucket.push_back(temp_bucket);
				temp_bucket = { obstacles[i] };
			}
			previous_point = obstacles[i];
		}
		if (temp_bucket.size() > 1) {
			obstacles_bucket.push_back(temp_bucket);
		}
		//printf("%i %i \n", obstacles.size(), obstacles_bucket.size());

		pair<float, float> current_point;
		closest_obstacle(obstacles_bucket);
		twoPoints tangents = closest_tangents(obstacles_bucket);

		if (delta2 > 0) {
			if (length(tangents.point1.first - goal.first, tangents.point1.second - goal.second) > length(tangents.point2.first - goal.first, tangents.point2.second) && current_obstacle1.first != current_obstacle2.first && current_obstacle1.second != current_obstacle2.second) {
				tangents.point2 = updatePoint(tangents.point2);
				pair<float, float> dist = distance(quadcopterc.first, quadcopterc.second, tangents.point2.first, tangents.point2.second);
				direction = normalize(dist.first,dist.second);
				current_point = tangents.point2;
			}
			else if (current_obstacle1!=current_obstacle2) {
				tangents.point1 = updatePoint(tangents.point1);
				pair<float, float> dist = distance(quadcopterc.first, quadcopterc.second, tangents.point1.first, tangents.point1.second);
				direction = normalize(dist.first, dist.second);
				current_point = tangents.point1;
			}
			else if (pointDistance(current_point.first, current_point.second, tangents.point1.first, tangents.point1.second) > pointDistance(current_point.first, current_point.second, tangents.point2.first, tangents.point1.second)) {
				tangents.point2 = updatePoint(tangents.point2);
				pair<float, float> dist = distance(quadcopterc.first, quadcopterc.second, tangents.point2.first, tangents.point2.second);
				direction = normalize(dist.first, dist.second);
				current_point = tangents.point2;
			}
			else {
				tangents.point1 = updatePoint(tangents.point1);
				pair<float, float> dist = distance(quadcopterc.first, quadcopterc.second, tangents.point1.first, tangents.point1.second);
				direction = normalize(dist.first, dist.second);
				current_point = tangents.point1;
			}
		}
		else {
			pair<float, float> dist = distance(quadcopterc.first, quadcopterc.second, goal.first, goal.second);
			direction = normalize(dist.first, dist.second);
		}

		printf("%f %f\n", direction.first, direction.second);
	}

    
	
    // done!
on_finished:
    RPlidarDriver::DisposeDriver(drv);
	
	double param, result;
	param = 350.0;
	result = sin(param*PI / 180);
	printf("The sine of %f degrees is %f.\n", param, result);
	
	std::cin.get();
    return 0;
}
