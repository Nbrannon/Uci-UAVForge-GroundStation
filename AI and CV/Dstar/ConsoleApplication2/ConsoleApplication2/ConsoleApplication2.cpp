// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MinHeap.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

int _tmain(int argc, _TCHAR* argv[])
{
	srand (time(NULL));

	Node array[] = {Node(1),Node(2),Node(3),Node(4),Node(5),Node(6),Node(7),Node(8),Node(9),Node(10),Node(11),Node(12),Node(13),Node(14),Node(15),Node(16),Node(18),Node(19)};

    MinHeap minHeap(array,18);

    for(int i=0; i<10; ++i)
    {
        cout << minHeap.GetMin() << "\n";
		int r = minHeap.Size() > 0 ? rand()%minHeap.Size() : 0;
        minHeap.Delete(r);
		cout << r << "\n" << minHeap << "\n";
    }
	system("pause");
	return 0;
}