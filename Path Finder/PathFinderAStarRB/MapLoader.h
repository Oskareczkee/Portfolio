#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include "vector.h"
#include "AVLTree.h"
#include "RBTree.h"
#include "myString.h"
#include "priority_queue.h"
using namespace std;

typedef pair<int, int> Vertex;
#define infinity INT_MAX

//#define DEBUG_PRINT

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
	Vertex coords;
	//only for cities, to find them
	int f = infinity, g = infinity, h = infinity;
	bool operator>(const Node& other) const { return f > other.f; }
	bool operator<(const Node& other)const { return f < other.f; }
	bool operator>=(const Node& other)const { return f >= other.f; }
	bool operator<=(const Node& other)const { return f <= other.f; }
	bool operator==(const Node& other)const { return g == other.g; }
};

struct Costs
{
	int f = infinity, g = infinity;
};

struct CityFlight
{
	Vertex to;
	int cost = 0;
};

struct City
{
	char cityName[30] = {};
	Vertex coords;

	//vector keeping coords of destination cities and costs
	myVector<CityFlight> flights;
};

class MapLoader
{
private:
	char** map = {};
	myVector<Vertex> citiesCoords;
	RBTree<myString, City*>Cities;
	RBTree<Vertex, City*>CitiesByCoords;

public:
	int rows = 0, cols = 0;
	void loadMap(istream& stream);
	void buildGraph(char** map);
	bool isRoad(const char& c)const { return (c == '#' || c == '*'); }
	bool isCity(const char& c)const { return c == '*'; }
	inline void getCityName(Vertex& cityCoords, City* node);

	//from and to have to be coords of the nodes
	inline void ShortestPath(Vertex& from, Vertex& to, bool showPath = false);

	//it calculates "Manhattan Distance", heuristic function, the simplest one
	inline int calculateHValue(Vertex& coords, Vertex& destination);
	inline bool isValid(int col, int row) { return (row >= 0) && (row < rows) && (col >= 0) && (col < cols); }
	inline bool isValidCityChar(const char& ch) { return (ch != '*' && ch != '#' && ch != '.'); }
};
