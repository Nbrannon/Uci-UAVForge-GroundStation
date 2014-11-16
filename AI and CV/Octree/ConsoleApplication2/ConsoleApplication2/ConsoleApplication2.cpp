// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OctreeHash.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	OctreeHash m;
	OctreeHash::Node node = m.getRoot();
	node = 5;
	node.branch();
	for ( int j = 0; j < 8; j++ ){
		node[j].branch();
		for ( int i = 0; i < 8; i++ ){
			node[j][i] = i+8*j;
			cout << m(node[j][i].getX()+0.05f,node[j][i].getY()+0.05f,node[j][i].getZ()-0.05f) << "\n";
		}
	}
	for ( int j = 0; j < 8; j++ ){
		cout << "node:" << j << " " << node[j] << "\n";
	}
	cout << "root node:" << node << "\n";
	cout << "root node:" << m(0.5,0.5,0.5) << "\n";
	system("pause");
	return 0;
}

