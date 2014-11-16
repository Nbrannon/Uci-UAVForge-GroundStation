#include "stdafx.h"
#include "OctreeHash.h"
#include <bitset>
OctreeHash::OctreeHash(): initialD( 1.f )
{
	new Node(this);
}
OctreeHash::Node::Node(const OctreeHash::Node &cSource) 
{
	value = cSource.value; 
	id = cSource.id; 
	parent = cSource.parent;
	parent->nodes[id] = this;
	delete &cSource;
};
OctreeHash::Node::Node(OctreeHash* oct)
{
	this->id = 1;
	this->parent = oct;
	this->parent->nodes[this->id]=this;
	this->value = 0;
}

OctreeHash::Node::Node(Node* parent, unsigned char id)
{
	this->id = (parent->id << 3) | id;
	this->parent = parent->parent;
	this->parent->nodes[this->id]=this;
	this->value = 0;
}

void OctreeHash::Node::branch()
{
	value = 0;
	for ( unsigned char i = 0; i < 8; i++ ){
		Node* node = new Node(this, i);
	}
}

float OctreeHash::Node::getHalfD(){
	float result = this->parent->initialD;
	unsigned char bitscount = numberOfBits();
	bitscount /= 3;
	bitscount++;
	result /= 1 << bitscount;
	return result;
}
unsigned char OctreeHash::Node::numberOfBits(){
	unsigned char bitscount = sizeof(unsigned long)*8;
	for( unsigned long i = 1<<(bitscount-1); i != 0; i >>= 1, bitscount-- ){
		if ( i & id ){
			break;
		}
	}
	return bitscount;
}
float OctreeHash::Node::getX(){
	float shift_factor = this->parent->initialD/4;
	float result = this->parent->initialD/2;
	unsigned char bitscount = numberOfBits();
	if ( bitscount < 4 )
		return result;
	for( unsigned long i = 1 << (bitscount-4), bits = 3; bits < bitscount; i >>= 3, bits += 3, shift_factor /= 2){
		if ( i & id ){
			result += shift_factor;
		}
		else{
			result -= shift_factor;
		}
	}
	return result;
}
float OctreeHash::Node::getY(){
	float shift_factor = this->parent->initialD/4;
	float result = this->parent->initialD/2;
	unsigned char bitscount = numberOfBits();
	if ( bitscount < 4 )
		return result;
	for( unsigned long i = 1 << (bitscount-2), bits = 1; bits < bitscount; i >>= 3, bits += 3, shift_factor /= 2){
		if ( i & id ){
			result += shift_factor;
		}
		else{
			result -= shift_factor;
		}
	}
	return result;
}
float OctreeHash::Node::getZ(){
	float shift_factor = this->parent->initialD/4;
	float result = this->parent->initialD/2;
	unsigned char bitscount = numberOfBits();
	if ( bitscount < 4 )
		return result;
	for( unsigned long i = 1 << (bitscount-3), bits = 2; bits < bitscount; i >>= 3, bits += 3, shift_factor /= 2){
		if ( i & id ){
			result += shift_factor;
		}
		else{
			result -= shift_factor;
		}
	}
	return result;
}

OctreeHash::Node& OctreeHash::operator() (float x, float y, float z){
	float shift_factor = initialD/4;
	float guessX,guessY,guessZ;
	guessX=guessY=guessZ= initialD/2;
	unsigned long id = 1;
	float r = 0.000001f;
	while(true){
		if ( x - guessX < r  &&  -r < x - guessX )
		{
			break;
		}
		if (nodes[id] == nullptr)
		{
			return *nodes[id>>3];
		}
		id <<= 3;
		id |= x > guessX ? 1 : 0;
		id |= y > guessY ? 4 : 0;
		id |= z > guessZ ? 2 : 0;
		guessX += x > guessX ? shift_factor : -shift_factor;
		guessY += y > guessY ? shift_factor : -shift_factor;
		guessZ += z > guessZ ? shift_factor : -shift_factor;
		shift_factor /= 2;
	}
	return *nodes[id];
}

OctreeHash::Node& OctreeHash::Node::operator[] (const char nIndex){
	return (*this->parent->nodes[(id << 3 ) | nIndex]); 
}
void OctreeHash::Node::operator=(const float &rhs){
	if (!isLeaf())
		return;
	Graph& g = this->parent->nodes;
	float newval = rhs;
	float oldval = value;
	float copy;
	for( unsigned long id = this->id >> 3; id != 0; id >>= 3 ){
		copy = g[id]->value;
		g[id]->value = g[id]->value + (newval-oldval)/8;
		newval = g[id]->value;
		oldval = copy;
	}
	value = rhs;
}
bool OctreeHash::Node::isLeaf(){
	return !this->parent->nodes[(id << 3 )];
}