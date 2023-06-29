#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include "vector.h"
#include "AVLTree.h"
#include "myString.h"
using namespace std;

typedef pair<int, int> Vertex;
typedef pair<int, int> iPair;
#define infinity INT_MAX

struct Flight
{
	char from[30] = {};
	char to[30] = {};
	int time = 0;
};

struct Command
{
	char from[30] = {};
	char to[30] = {};
	bool showPath = false;
};

struct Node
{
	int id;
	char cityName[30];

	//neighbors, and their weight from the node
	myVector<pair<Node*, int>> neighbors;
};

class MapLoader
{
	private:
		char** map = {};
		AVLTree<int, Node*>Nodes;
		AVLTree<myString, Node*>Cities;
		//unordered_map<int, Node*> Nodes;
		//unordered_map<string, Node*> Cities;

	public:
		int rows = 0,cols = 0;
		void loadMap(istream& stream);
		void buildGraph(char** map);
		bool isRoad(const char& c)const { return (c == '#' || c == '*'); }
		void getCityName(Vertex cityCoords, Node* node);
		//from and to have to be id's of the nodes
		void ShortestPath(int from, int to, bool showPath=false);
};
