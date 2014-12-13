#pragma once
#include "vector"
#include "Node.h"
using namespace std;

class MinHeap
{
private:
    vector<Node> _vector;
    void BubbleDown(int index);
    void BubbleUp(int index);
    void Heapify();
	void swap(int i, int j);
	int reverse_index[Node::PRB_SCOPE];
public:
    MinHeap(Node* array, int length);
    MinHeap(const vector<Node>& vector);
    MinHeap();

    void Insert(const Node& newValue);
	bool Contains(const Node& value);
	void Update(const Node& value);
    Node& GetMin();
    void DeleteMin();
	void Delete(int index);
	std::string::size_type MinHeap::Size();
	friend std::ostream& operator<<(std::ostream& os, const MinHeap& heap);
};
