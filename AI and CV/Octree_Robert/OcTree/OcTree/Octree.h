#ifndef OCTREE_H
#define OCTREE_H
#include <vector>

namespace UAVFORGE
{
	struct Axis3D
	{
		float x;
		float y;
		float z;
		Axis3D(){}
		Axis3D(float _x, float _y, float _z):x(_x), y(_y), z(_z){ }
		
	};
	
	Axis3D operator*(Axis3D const& axis3D, float r){
		return Axis3D(axis3D.x*r, axis3D.y*r, axis3D.z*r);
	}

	Axis3D operator-(Axis3D pointA, Axis3D pointB)
	{
		return Axis3D(pointA.x - pointB.x, pointA.y - pointB.y, pointA.z - pointB.z);
	}

	Axis3D operator+(Axis3D pointA, Axis3D pointB)
	{
		return Axis3D(pointA.x + pointB.x, pointA.y + pointB.y, pointA.z + pointB.z);
	}


	//// point structure as float 
	class OctreePoint{
		Axis3D point;
	public:
		OctreePoint(){}
		OctreePoint(float xvalue, float yvalue, float zvalue)
		{
			
			point.x = xvalue;
			point.y = yvalue;
			point.z = zvalue;
		}
		inline void setPosition(float xvalue,float yvalue,float zvalue )
		{
			
			point.x = xvalue;
			point.y = yvalue;
			point.z = zvalue;
		}
		inline const Axis3D getPoint()const { return point; }

		~OctreePoint(){ 
			
		}
	
	};


	class Octree{
		// Set ALIGNED box from two low/high vertices. Box axes are aligned with 
		// global frame axes that verticies are specified in.

		// lowVertex = vertex[0]
		// highVertex = vertex[6]
		//
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

		Axis3D origin, halfDimension;
		OctreePoint *data = nullptr; 
		Octree *children[8];
		float potentialValue;

	public:
		
		Octree(const Axis3D& origin, const Axis3D& halfDimension) :origin(origin), halfDimension(halfDimension), data(NULL)
		{
			for (int i = 0; i<8; ++i)
				children[i] = NULL;
		}
		Octree(const Octree& copy) : origin(copy.origin), halfDimension(halfDimension), data(copy.data){}
		
		~Octree(){
			for (int i = 0; i < 8; i++){
				delete children[i];
			}
		}

		bool isLeafNode() const
		{
			return children[0] == NULL;
		}

		void insert(OctreePoint* point)
		{
			if (isLeafNode())
			{
				if (data == NULL){
					data = point;
					return;
				}
				else{
					OctreePoint *oldPoint = data;
					data = NULL;
					for (int i = 0; i < 8; i++)
					{
						Axis3D newOrigin = origin;
						newOrigin.x += halfDimension.x * (i & 4 ? .5f : -.5f);
						newOrigin.y += halfDimension.y * (i & 2 ? .5f : -.5f);
						newOrigin.z += halfDimension.z * (i & 1 ? .5f : -.5f);
						
						children[i] = new Octree(newOrigin, halfDimension *.5f);
					}

					children[getOctantContainingPoint(oldPoint->getPoint())]->insert(oldPoint);
					children[getOctantContainingPoint(point->getPoint())]->insert(point);
				}
			}
			else
			{
				int octant = getOctantContainingPoint(point->getPoint());
				children[octant]->insert(point);
			}
		}

		// Determine which octant of the tree would contain 'point'
		int getOctantContainingPoint(const Axis3D& point) const {
			int oct = 0;
			if (point.x >= origin.x) oct |= 4;
			if (point.y >= origin.y) oct |= 2;
			if (point.z >= origin.z) oct |= 1;
			return oct;
		}

		// This routine will provide all points contained with the box
		// defined by min/max points (bmin, bmax). bmin,bmax should be opposing vertices
		// All results are pushed into 'results'
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
		void getPointsInsideBox(const Axis3D& bmin, const Axis3D& bmax, std::vector<OctreePoint*>& results) {
			// If we're at a leaf node, just see if the current data point is inside
			// the query bounding box
			if (isLeafNode()) {
				if (data != NULL) {
					const Axis3D& p = data->getPoint();
					if (p.x>bmax.x || p.y>bmax.y || p.z>bmax.z) return;
					if (p.x<bmin.x || p.y<bmin.y || p.z<bmin.z) return;
					results.push_back(data);
				}
			}
			else {
				// We're at an interior node of the tree. We will check to see if
				// the query bounding box lies outside the octants of this node.
				for (int i = 0; i<8; ++i) {
					// Compute the min/max corners of this child octant
					Axis3D cmax = children[i]->origin + children[i]->halfDimension;
					Axis3D cmin = children[i]->origin - children[i]->halfDimension;
					// If the query rectangle is outside the child's bounding box,
					// then continue
					if (cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
					if (cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;
					// At this point, we've determined that this child is intersecting
					// the query bounding box
					children[i]->getPointsInsideBox(bmin, bmax, results);
				}
			}
		}
	
	
	
	};
}
#endif