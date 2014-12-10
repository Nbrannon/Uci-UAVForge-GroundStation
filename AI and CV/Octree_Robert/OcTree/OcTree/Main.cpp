#include "Octree.h"
#include <ctime>
#include <iostream>
int main(int argc, char *argv[])
{
	int pointCount = 0;
	UAVFORGE::Octree *Octree;
	Octree = new UAVFORGE::Octree(UAVFORGE::Axis3D(0, 0, 0), UAVFORGE::Axis3D(15000, 15000, 15000));
	std::cout << "Enter number of points to store:";
	std::cin >> pointCount;
	UAVFORGE::OctreePoint *GPoint = new UAVFORGE::OctreePoint[pointCount];
	unsigned int start = clock();
	for (int i = 0; i < pointCount; i++)
	{	
		
		float x = rand() % 30000 + (-15000);
		float y = rand() % 30000 + (-15000);
		float z = rand() % 30000 + (-15000);
		

		GPoint[i].setPosition(x,y,z);
		Octree->insert(GPoint + i);
	}
	std::cout << "Time taken in millisecs: " << clock() - start;

	//    y
	//    |
	//    |
	//    |________x
	//   /  3-------2
	//  /  /|      /| 
	// z  7-------6 | 
	//    | 0-----|-1 
	//    |/      |/ 
	//    4-------5 


	// Test all points returned from forward looking half of tree.
	UAVFORGE::Axis3D bmin(0, 0, 0);
	UAVFORGE::Axis3D bmax(15000, 15000, 15000);
	std::vector<UAVFORGE::OctreePoint*> pointsInBox;
	Octree->getPointsInsideBox(bmin, bmax, pointsInBox);

	std::cin.ignore();

}