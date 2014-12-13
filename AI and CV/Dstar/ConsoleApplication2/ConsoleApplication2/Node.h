#pragma once
#include<iostream>
typedef float DISTANCE;
class Node
{
	friend class MinHeap;
private:
	static const int PRB_SCOPE = 100;
	static DISTANCE g[PRB_SCOPE];
	static DISTANCE rhs[PRB_SCOPE];
	DISTANCE k1;
	DISTANCE k2;
	int world_index;
public:
	void recalcKey(DISTANCE k_m);
	bool operator < (Node& other);
	bool operator > (Node& other);
	Node(int world_index);
	Node(){};
	Node& operator= (const Node& other);
	~Node(void);
	friend std::ostream& operator<<(std::ostream& os, const Node& node);
	friend DISTANCE h( Node& x, Node& y );
	static Node start;
};
