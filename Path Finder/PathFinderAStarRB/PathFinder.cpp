#pragma once
#include <iostream>
#include <fstream>
#include "MapLoader.h"
#include "AVLTree.h"
#include "RBTree.h"
#include "vector.h"
#include "priority_queue.h"
#include <chrono>


//DEBUG DEFINES
//#define DEBUG_PRINT
#define DEBUG

using namespace std;

int main()
{
	try
	{
		#ifdef  DEBUG_PRINT
				auto start = chrono::high_resolution_clock::now();
		#endif //  DEBUG_PRINT	
		#ifdef DEBUG
				ifstream ifs{"test12.txt"};
				if (!ifs)
					throw exception("File could not be opened");
    
				MapLoader loader;
				loader.loadMap(ifs);
		#else
				MapLoader loader;
				loader.loadMap(cin);
		#endif //DEBUG

		#ifdef DEBUG_PRINT
				auto end  = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::seconds>(end-start);
				cout <<"\n" <<duration.count() << " seconds";
		#endif // DEBUG_PRINT
		
	}
	catch (const std::exception& e)
	{
		cerr << e.what();
	}
}
