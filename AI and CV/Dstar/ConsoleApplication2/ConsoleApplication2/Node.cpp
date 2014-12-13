#include "stdafx.h"
#include "Node.h"


Node::Node(int world_index)
{
	this->world_index = world_index;
	this->k1 = world_index;
}

Node& Node::operator= (const Node& other){
	if (this != &other) // protect against invalid self-assignment
	{
		k1 = other.k1;
		k2 = other.k2;
		world_index = other.world_index;
	}
	return *this;
}

bool Node::operator < (Node& other){
	return this->k1 < other.k1 || (this->k1 == other.k1 && this->k2 < other.k2);
}
bool Node::operator > (Node& other){
	return this->k1 > other.k1 || (this->k1 == other.k1 && this->k2 > other.k2);
}

void Node::recalcKey(DISTANCE k_m){
	k2 = g[world_index] < rhs[world_index] ? g[world_index] : rhs[world_index];
	k1 = k2 + h(start,*this)+k_m;
}
std::ostream& operator<<(std::ostream& os, const Node& node)
{
	os << node.k1 << "," << node.k2;
	return os;
}

DISTANCE h( Node& x, Node& y ){
	return x.k1 + y.k1;
}

Node::~Node(void)
{
}

DISTANCE Node::rhs[Node::PRB_SCOPE] = {0};
DISTANCE Node::g[Node::PRB_SCOPE] = {0};

Node Node::start = Node(0);