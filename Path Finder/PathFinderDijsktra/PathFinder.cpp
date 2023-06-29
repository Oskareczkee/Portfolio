#include <iostream>
#include <fstream>
#include "MapLoader.h"
#include "AVLTree.h"
#include "vector.h"
#include <chrono>

using namespace std;

int main()
{
	try
	{
		auto start = chrono::high_resolution_clock::now();

		//AVLTree<int, int> tree;
		//tree[25] = 30;

		/*
		for (int x = 0; x < 20; x++)
			tree.insert({ x, x + 10 });
		*/



		/*
		AVLTree<int, string> tree;
		tree.insert({1, "siema"});
		tree.insert({ 3, "siema witam" });
		tree.insert({ 50, "witam witam wszystkich" });
		tree.insert({ 100, "mogus mogus amogus" });
		tree[25] = "o co tu chodzi";
		tree[25] = "";
		auto content = tree.getContent();
		//tree.printContent();

		
		for (auto& x : content)
			cout << x << "\n";
		*/
		//cout <<tree[50];
		
		
		ifstream ifs{"test13.txt"};
		if (!ifs)
			throw exception("File could not be opened");
    
		MapLoader loader;
		loader.loadMap(ifs);

		auto end  = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::seconds>(end-start);
		cout <<"\n" <<duration.count() << " seconds";
	}
	catch (const std::exception& e)
	{
		cerr << e.what();
	}
}
