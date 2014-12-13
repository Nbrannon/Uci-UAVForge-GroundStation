#include "stdafx.h"
#include "MinHeap.h"

MinHeap::MinHeap(Node* array, int length) : _vector(length+1)
{
	for ( unsigned long i = 0; i < Node::PRB_SCOPE; i++ ){
		reverse_index[i] = -1;
	}
    for(int i = 0; i < length; ++i)
    {
		reverse_index[array[i].world_index] = i;
        _vector[i] = array[i];
    }
	_vector[0] = Node::start;
	reverse_index[0] = 0;
    Heapify();
}

MinHeap::MinHeap(const vector<Node>& vector) : _vector(vector)
{
	for ( unsigned long i = 0; i < Node::PRB_SCOPE; i++ ){
		reverse_index[i] = -1;
	}
	for ( unsigned long i = 0; i < vector.size(); i++ ){
		reverse_index[vector[i].world_index] = i;
	}
	_vector.push_back(Node::start);
	reverse_index[0] = vector.size();
    Heapify();
}

MinHeap::MinHeap() : _vector(1)
{
	for ( unsigned long i = 0; i < Node::PRB_SCOPE; i++ ){
		reverse_index[i] = -1;
	}
	_vector.push_back(Node::start);
	reverse_index[0] = 1;
}

void MinHeap::Heapify()
{
    int length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(i);
    }
}

void MinHeap::BubbleDown(int index)
{
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int minIndex = index;

    if(_vector[index] > _vector[leftChildIndex])
    {
        minIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[minIndex] > _vector[rightChildIndex]))
    {
        minIndex = rightChildIndex;
    }

    if(minIndex != index)
    {
        //need to swap
        Node temp = _vector[index];
        _vector[index] = _vector[minIndex];
		reverse_index[_vector[index].world_index] = index;
        _vector[minIndex] = temp;
		reverse_index[_vector[minIndex].world_index] = minIndex;
        BubbleDown(minIndex);
    }
}

void MinHeap::BubbleUp(int index)
{
    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_vector[parentIndex] > _vector[index])
    {
        Node temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
		reverse_index[_vector[parentIndex].world_index] = parentIndex;
        _vector[index] = temp;
		reverse_index[_vector[index].world_index] = index;
        BubbleUp(parentIndex);
    }
}

void MinHeap::Insert(const Node& newValue)
{
    int length = _vector.size();
    _vector[length] = newValue;
	reverse_index[_vector[length].world_index] = length;
    BubbleUp(length);
}

Node& MinHeap::GetMin()
{
    return _vector[0];
}
    
void MinHeap::DeleteMin()
{
    int length = _vector.size();

    if(length == 0)
    {
        return;
    }
	reverse_index[_vector[0].world_index] = -1;
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    BubbleDown(0);
}

void MinHeap::Delete(int index)
{
    int length = _vector.size();

    if(index >= length)
    {
        return;
    }
	if (index == length-1)
	{
		_vector.pop_back();
		return;
	}
	
	reverse_index[_vector[index].world_index] = -1;
	_vector[index] = _vector[length-1];

	_vector.pop_back();

	BubbleDown(index);
	BubbleUp(index);
}

std::string::size_type MinHeap::Size(){
	return _vector.size();
}

std::ostream& operator<<(std::ostream& os, const MinHeap& heap)
{
	os << " ' " ;
	for ( int i = 0; i < heap._vector.size(); i++ ) 
		os << heap._vector[i] << "\n";
	os << " ' " ;
	return os;
}

bool MinHeap::Contains(const Node& value){
	return reverse_index[value.world_index] > -1;
}
void MinHeap::Update(const Node& value){
	Delete(reverse_index[value.world_index]);
	Insert(value);
}