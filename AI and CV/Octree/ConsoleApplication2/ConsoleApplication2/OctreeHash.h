#pragma once
#include<unordered_map>
using namespace std;
class OctreeHash
{
public:
	const float initialD;
	class Node{
		public:
			Node(Node* parent, unsigned char id);
			Node(OctreeHash* oct);
			Node(const Node &cSource);
			Node& operator[] (const char nIndex);
			void branch(void);
			void operator=(const float& rhs);
			operator float(){return value;};
			bool isLeaf();
			float getX();
			float getY();
			float getZ();
			float getHalfD();
		private:
			float value;
			unsigned long id;
			OctreeHash* parent;
			unsigned char numberOfBits();
	};
	typedef unordered_map<int,Node*> Graph;
	Graph nodes; 
public:
	Node& getRoot(){ return *nodes[1]; }
	Node& operator() (float x, float y, float z);
	OctreeHash();
};
