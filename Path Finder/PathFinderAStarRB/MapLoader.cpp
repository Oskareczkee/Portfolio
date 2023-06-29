#include "MapLoader.h"

void MapLoader::loadMap(istream& stream)
{
	//first 2 numbers are width and height of map
	stream >> rows >> cols;

	//MAP LOADING

	//create dynamic map array
	map = new char* [cols];
	for (int x = 0; x < cols; x++)
		map[x] = new char[rows];

	//load map
	for (int x = 0; x < cols; x++)
		for (int y = 0; y < rows; y++)
		{
			stream >> map[x][y];
			if (isCity(map[x][y]))
				citiesCoords.push_back({ x,y });
		}
#ifdef DEBUG_PRINT
	cout << "Map loaded\n";
#endif // DEBUG
#ifdef DEBUG_PRINT_MAP
	//print map
	for (int x = 0; x < cols; x++)
	{
		for (int y = 0; y < rows; y++)
			cout << map[x][y];
		cout << "\n";
	}
#endif // DEBUG_PRINT_MAP

	int flightsNum = 0;
	stream >> flightsNum;

	Flight* flights = new Flight[flightsNum];

	for (int x = 0; x < flightsNum; x++)
	{
		stream >> flights[x].from;
		stream >> flights[x].to;
		stream >> flights[x].time;
	}

	int commandNum = 0;
	stream >> commandNum;

	Command* commands = new Command[commandNum];
	for (int x = 0; x < commandNum; x++)
	{
		stream >> commands[x].from;
		stream >> commands[x].to;
		stream >> commands[x].showPath;
	}
#ifdef DEBUG_PRINT
	cout << "Flights loaded...\n";
#endif

	//create graph representation (just load city names)
	buildGraph(map);
#ifdef DEBUG_PRINT
	cout << "Graph Builded...\n";
#endif // DEBUG_PRINT

	//add flights nodes to the graph
	for (int x = 0; x < flightsNum; x++)
	{
		auto& cityFrom = Cities[flights[x].from];
		auto& cityTo = Cities[flights[x].to];

		Cities[cityFrom->cityName]->flights.push_back({ cityTo->coords, flights[x].time });
	}

	//execute commands
	for (int x = 0; x < commandNum; x++)
		ShortestPath(Cities[commands[x].from]->coords, Cities[commands[x].to]->coords, commands[x].showPath);



	//delete pointers
	for (int x = 0; x < cols; x++)
		delete[] map[x];
	delete[] map;

	delete[] flights;
	delete[] commands;
}

void MapLoader::getCityName(Vertex& cityCoords, City* node)
{
	int x = cityCoords.first;
	int y = cityCoords.second;
	Vertex charCoords;
	//find the letter of city

	//up
	if (x > 0 && isValidCityChar(map[x - 1][y]))
		charCoords = Vertex(x - 1, y);

	//down
	else if (x < cols - 1 && isValidCityChar(map[x + 1][y]))
		charCoords = Vertex(x + 1, y);

	//right
	else if (y < rows - 1 && isValidCityChar(map[x][y + 1]))
		charCoords = Vertex(x, y + 1);
	//left
	else if (y > 0 && isValidCityChar(map[x][y - 1]))
		charCoords = Vertex(x, y - 1);

	//diagonals
	//down left
	else if (x < cols - 1 && y > 0 && isValidCityChar(map[x + 1][y - 1]))
		charCoords = Vertex(x + 1, y - 1);
	//down right
	else if (x < cols - 1 && y < rows - 1 && isValidCityChar(map[x + 1][y + 1]))
		charCoords = Vertex(x + 1, y + 1);
	//up left
	else if (x > 0 && y > 0 && isValidCityChar(map[x - 1][y - 1]))
		charCoords = Vertex(x - 1, y - 1);
	//up right
	else if (x > 0 && y < rows - 1 && isValidCityChar(map[x - 1][y + 1]))
		charCoords = Vertex(x - 1, y + 1);



	//read letters from the right to left <-
	char lettersLeft[15] = {};
	int stringLeftLen = 0;
	//starting position
	int pos = charCoords.second - 1;

	//do not allow position less than 0
	if (pos >= 0)
	{
		char c = map[charCoords.first][pos];
		while (pos >= 0 && isValidCityChar(c))
		{
			lettersLeft[stringLeftLen] = c;
			stringLeftLen++;
			pos--;
			c = map[charCoords.first][pos];
		}
	}

	//read letters from the left to the right ->
	char lettersRight[15] = {};
	int stringRightLen = 0;
	//starting position, one to the right
	pos = charCoords.second;
	//do not allow position higher than rows
	if (pos < rows)
	{
		char c = map[charCoords.first][pos];
		while (pos < rows && isValidCityChar(c))
		{
			lettersRight[stringRightLen] = c;
			stringRightLen++;
			pos++;
			c = map[charCoords.first][pos];
		}
	}

	//merge them into cityName
	int strPos = 0;
	//load letters from the left in reverse order
	for (int x = stringLeftLen - 1; x >= 0; --x)
	{
		node->cityName[strPos] = lettersLeft[x];
		strPos++;
	}
	//load letters from the right
	for (int x = 0; x < stringRightLen; x++)
	{
		node->cityName[strPos] = lettersRight[x];
		strPos++;
	}
}

void MapLoader::buildGraph(char** map)
{
	for (auto& x : citiesCoords)
	{
		City* cityNode = new City();
		getCityName(x, cityNode);
		cityNode->coords = x;
		Cities.insert({ cityNode->cityName, cityNode });
		CitiesByCoords.insert({ cityNode->coords, cityNode });
	}
}

int MapLoader::calculateHValue(Vertex& coords, Vertex& destination)
{
	return abs(coords.first - destination.first) +
		abs(coords.second - destination.second);
}


void MapLoader::ShortestPath(Vertex& src, Vertex& to, bool showPath)
{
	//in comments are previously used data structures tested for performance of algorithm

	//vertex for coords, pair contains fcost and gcost
	//AVLTree<Vertex, Costs> cellDetails;
	//RBTree<Vertex, Costs> cellDetails;
	Costs** cellDetails = new Costs * [cols];
	for (int x = 0; x < cols; x++)
		cellDetails[x] = new Costs[rows];
	//std::map<Vertex, Costs> cellDetails;
	//coords, pair with parent coords, cost of travel
	//AVLTree<Vertex, pair<Vertex, int>>parents;
	//RBTree<Vertex, pair<Vertex, int>>parents;
	pair<Vertex, int>** parents = new pair<Vertex, int>* [cols];
	for (int x = 0; x < cols; x++)
		parents[x] = new pair<Vertex, int>[rows];
	//std::map<Vertex, pair<Vertex, int>>parents;
	//priority_queue<Node, vector<Node>, greater<Node>> pq;
	//this queue struggles when amount of flights is big (in millions) the upper one does not somehow
	Mpriority_queue<Node> pq;
	
	pq.push(Node{ src,0,0,0 });
	cellDetails[src.first][src.second] = { 0,0 };
	//coords -1-1. cost 0
	parents[src.first][src.second] = { {-1,-1},0 };

	while (!pq.empty())
	{
		Node n = pq.top();
		pq.pop();

		Vertex coords = n.coords;
		Vertex neighCoords;
		if (coords == to)
			break;
		int gNew = 0, hNew = 0, fNew = 0;
		//up
		neighCoords = { coords.first - 1, coords.second };
		if (isValid(neighCoords.first, neighCoords.second)
			&& isRoad(map[neighCoords.first][neighCoords.second]))
		{
			int cost = 1;
			gNew = n.g + cost;
			fNew = calculateHValue(neighCoords, to) + gNew;

			if (fNew < cellDetails[neighCoords.first][neighCoords.second].f)
			{
				cellDetails[neighCoords.first][neighCoords.second] = { fNew, gNew };
				parents[neighCoords.first][neighCoords.second] = { coords, cost };
				Node neighNode;
				neighNode.coords = neighCoords;
				neighNode.f = fNew;
				neighNode.g = gNew;
				pq.push(neighNode);
			}
		}

		//down
		neighCoords = { coords.first + 1, coords.second };
		if (isValid(neighCoords.first, neighCoords.second)
			&& isRoad(map[neighCoords.first][neighCoords.second]))
		{
			int cost = 1;
			gNew = n.g + cost;
			fNew = calculateHValue(neighCoords, to) + gNew;

			if (fNew < cellDetails[neighCoords.first][neighCoords.second].f)
			{
				cellDetails[neighCoords.first][neighCoords.second] = { fNew, gNew };
				parents[neighCoords.first][neighCoords.second] = { coords,cost };
				Node neighNode;
				neighNode.coords = neighCoords;
				neighNode.f = fNew;
				neighNode.g = gNew;
				pq.push(neighNode);
			}
		}

		//left
		neighCoords = { coords.first, coords.second - 1 };
		if (isValid(neighCoords.first, neighCoords.second)
			&& isRoad(map[neighCoords.first][neighCoords.second]))
		{
			int cost = 1;
			gNew = n.g + cost;
			fNew = calculateHValue(neighCoords, to) + gNew;

			if (fNew < cellDetails[neighCoords.first][neighCoords.second].f)
			{
				cellDetails[neighCoords.first][neighCoords.second] = { fNew, gNew };
				parents[neighCoords.first][neighCoords.second] = { coords,cost };
				Node neighNode;
				neighNode.coords = neighCoords;
				neighNode.f = fNew;
				neighNode.g = gNew;
				pq.push(neighNode);
			}
		}

		//right
		neighCoords = { coords.first, coords.second + 1 };
		if (isValid(neighCoords.first, neighCoords.second)
			&& isRoad(map[neighCoords.first][neighCoords.second]))
		{
			int cost = 1;
			gNew = n.g + cost;
			fNew = calculateHValue(neighCoords, to) + gNew;

			if (fNew < cellDetails[neighCoords.first][neighCoords.second].f)
			{
				cellDetails[neighCoords.first][neighCoords.second] = { fNew, gNew };
				parents[neighCoords.first][neighCoords.second] = { coords,cost };
				Node neighNode;
				neighNode.coords = neighCoords;
				neighNode.f = fNew;
				neighNode.g = gNew;
				pq.push(neighNode);
			}
		}

		//if city, check flights
		if (isCity(map[coords.first][coords.second]))
		{
			auto& city = CitiesByCoords[coords];
			for (auto& flight : city->flights)
			{
				int cost = flight.cost;
				Vertex dest = flight.to;

				gNew = n.g + cost;
				fNew = calculateHValue(dest, to) + gNew;
				if (fNew < cellDetails[dest.first][dest.second].f)
				{
					cellDetails[dest.first][dest.second] = { fNew, gNew };
					parents[dest.first][dest.second] = { coords,cost };
					Node neighNode;
					neighNode.coords = dest;
					neighNode.f = fNew;
					neighNode.g = gNew;
					pq.push(neighNode);
				}
			}
		}
	}

	myVector<Vertex> res;
	res.push_back(to);
	auto& temp = parents[to.first][to.second];
	int length = 0;
	//check if coordinates are not -1, -1 coordinates indicates we got to the source
	while (temp.first.first != -1)
	{
		length += temp.second;
		res.push_back(temp.first);
		temp = parents[temp.first.first][temp.first.second];
	}

	//actual path is reversed, reverse it
	reverse(res.begin(), res.end());
	cout << length;
	if (showPath)
	{

		cout << " ";
		for (auto& x : res)
		{
			if (x == to || x == src)
				continue;
			//cout << x.first << " " << x.second << "\n";
			auto a = CitiesByCoords.find(x);
			if (a != nullptr)
				cout << a->content->cityName << " ";
		}
		cout << "\n";

	}
	else
		cout << "\n";

	for (int x = 0; x < cols; x++)
	{
		delete[] cellDetails[x];
		delete[] parents[x];
	}

	delete[] cellDetails;
	delete[] parents;

}