#include "MapLoader.h"
//#define DEBUG_PRINT

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
			stream >> map[x][y];

#ifdef DEBUG_PRINT
	//print map
	for (int x = 0; x < cols; x++)
	{
		for (int y = 0; y < rows; y++)
			cout << map[x][y];
		cout<<"\n";
	}
#endif // DEBUG_PRINT

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


	//create graph representation

	buildGraph(map);

	//add flights nodes to the graph
	for (int x = 0; x < flightsNum; x++)
	{
		Node* cityFrom = Cities[flights[x].from];
		Node* cityTo = Cities[flights[x].to];

		cityFrom->neighbors.push_back({ Nodes[cityTo->id], flights[x].time });
	}


#ifdef DEBUG_PRINT
	for (auto& x : Nodes.getContent())
	{
		int id = x->id;
		Node node = *x;

		cout << "id: " << id << "  " << node.cityName << "\n";
		cout << "Neighbors:\n";
		for (auto& a : node.neighbors)
			cout << a.first->id << " ";

		cout << "\n\n";
	}
#endif  DEBUG_PRINT

	//execute commands
	for (int x = 0; x < commandNum; x++)
		ShortestPath(Cities[commands[x].from]->id, Cities[commands[x].to]->id, commands[x].showPath);



	//delete pointers
	for (int x = 0; x < cols; x++)
		delete[] map[x];
	delete[] map;

	delete[] flights;
	delete[] commands;
}


//TODO CHANGE CITY NAME LOADING
void MapLoader::getCityName(Vertex cityCoords, Node* node)
{
	int x = cityCoords.first;
	int y = cityCoords.second;
	Vertex charCoords;
	bool isReversed = false;
	bool readLeftRight = false;
	//find the letter of city
	
	//up
	if (x > 0 && isalpha(map[x - 1][y]))
	{
		charCoords = Vertex(x - 1, y);
		readLeftRight = true;
	}
	//down
	else if (x < cols - 1 && isalpha(map[x + 1][y]))
	{
		charCoords = Vertex(x + 1, y);
		readLeftRight = true;
	}
	//right
	else if (y < rows - 1 && isalpha(map[x][y + 1]))
		charCoords = Vertex(x, y + 1);
	//left
	else if (y > 0 && isalpha(map[x][y - 1]))
	{
		isReversed = true;
		charCoords = Vertex(x, y - 1);
	}

	//diagonals
	//down left
	else if (x < cols-1 && y > 0 && isalpha(map[x + 1][y - 1]))
	{
		isReversed = true;
		charCoords = Vertex(x + 1, y - 1);
	}
	//down right
	else if (x < cols-1 && y < rows - 1 && isalpha(map[x + 1][y + 1]))
		charCoords = Vertex(x + 1, y + 1);
	//up left
	else if (x >0 && y > 0 && isalpha(map[x - 1][y - 1]))
	{
		isReversed = true;
		charCoords = Vertex(x - 1, y - 1);
	}
	//up right
	else if (x > 0 && y < rows - 1 && isalpha(map[x - 1][y + 1]))
		charCoords = Vertex(x - 1, y + 1);

	
	if (readLeftRight)
	{
		//keep the first letter
		char letter = map[charCoords.first][charCoords.second];


		//read letters from the right to left <-
		char lettersLeft[15];
		int stringLeftLen = 0;
		//starting position, one to the left
		int pos = charCoords.second-1;

		//do not allow position less than 0
		if (pos >= 0)
		{
			char c = map[charCoords.first][pos];
			while (c != '.' && c != '#' && pos >=0)
			{
				lettersLeft[stringLeftLen] = c;
				stringLeftLen++;
				pos --;
				c = map[charCoords.first][pos];
			}
		}

		//read letters from the left to the right ->
		char lettersRight[15];
		int stringRightLen = 0;
		//starting position, one to the right
		pos = charCoords.second + 1;
		//do not allow position higher or equal to rows
		if (pos < rows)
		{
			char c = map[charCoords.first][pos];
			while (c != '.' && c != '#' && pos <rows)
			{
				lettersRight[stringRightLen] = c;
				stringRightLen++;
				pos++;
				c = map[charCoords.first][pos];
			}
		}

		//reverse letters loaded from the right to left - they are reversed
		reverse(lettersLeft, lettersLeft + stringLeftLen);
		//merge them into string
		char cityName[30];
		copy(lettersLeft, lettersLeft + stringLeftLen, cityName);
		//copy one sad letter at the beginning
		cityName[stringLeftLen] = letter;
		copy(lettersRight, lettersRight + stringRightLen, cityName + stringLeftLen + 1);

		//wow
		copy(cityName, cityName + stringLeftLen + stringRightLen + 1, node->cityName);
		return;

	}
	


	int stringLen = 0;
	//load from left to right
	if (!isReversed)
	{
		char c = map[charCoords.first][charCoords.second];
		stringLen = 0;
		int pos = charCoords.second + stringLen;
		while (c != '.' && c != '#' && pos<rows)
		{
			node->cityName[stringLen] = c;
			stringLen++;
			c = map[charCoords.first][charCoords.second + stringLen];
			pos ++;
		}
	}
	//load from right to left
	else
	{
		char c = map[charCoords.first][charCoords.second];
		stringLen = 0;
		int pos = charCoords.second - stringLen;
		while (c != '.' && c != '#' && pos>=0)
		{
			node->cityName[stringLen] = c;
			stringLen++;
			c = map[charCoords.first][charCoords.second - stringLen];
			pos--;
		}

		//text is reversed it, reverse it to the normal state
		reverse(node->cityName, node->cityName + stringLen);
	}
}

void MapLoader::buildGraph(char** map)
{

	for (int x = 0; x < cols; x++)
	{
		for (int y = 0; y < rows; y++)
		{
			if (map[x][y] == '.' || isalpha(map[x][y]))
				continue;

			Vertex vert(x, y);
			//id is hash function, each node should have unique id
			//id shall never be 0 by definition
			int nodeID = x * rows + y +1;

			Node* node = new Node{ nodeID };

			//get node neighbours
			//up
			if (x > 0 && isRoad(map[x - 1][y]))
			{
				int neighborID = (x - 1) * rows + y+1;
				Node* neighbour = new Node{ neighborID };
				node->neighbors.push_back({ neighbour,1 });
			}
			//down
			if (x < cols-1 && isRoad(map[x + 1][y]))
			{
				int neighborID = (x + 1) * rows + y+1;
				Node* neighbour = new Node{ neighborID };
				node->neighbors.push_back({ neighbour,1 });
			}
			//right
			if (y < rows - 1 && isRoad(map[x][y + 1]))
			{
				int neighborID = x * rows + (y + 1)+1;
				Node* neighbour = new Node{ neighborID };
				node->neighbors.push_back({ neighbour,1 });
			}
			//left
			if (y > 0 && isRoad(map[x][y - 1]))
			{
				int neighborID = x * rows + (y - 1)+1;
				Node* neighbour = new Node{ neighborID };
				node->neighbors.push_back({ neighbour,1 });
			}

			//if node is a city
			if (map[x][y] == '*')
			{
				getCityName(vert, node);
				Cities.insert({ node->cityName, node });

			}

			Nodes.insert({ nodeID, node });
		}
	}
}

void MapLoader::ShortestPath(int src, int to, bool showPath)
{
	priority_queue<iPair, vector<iPair>, greater<iPair>>pq;

	AVLTree<int, int>dist;
	AVLTree<int, int>parents;
	//unordered_map<int, int> dist;
	//unordered_map<int, int> parents;

	for (auto x : Nodes.getContent())
		dist.insert({ x->id, infinity });

	parents[src] = 0;

	pq.push(make_pair(0, src));
	dist[src] = 0;
	while (!pq.empty())
	{
		int u = pq.top().second;
		pq.pop();

		if (u == to)
			break;

		for (auto& x : Nodes[u]->neighbors)
		{
			int id = x.first->id;
			int weight = x.second;

			if (dist[id] > dist[u] + weight)
			{
				dist[id] = dist[u] + weight;
				pq.push(make_pair(dist[id], id));
				parents[id] = u;
			}
		}
	}

	cout << dist[to];

	if (showPath)
	{
		cout << " ";
		myVector<int> res;
		res.push_back(to);
		int temp = to;
		while (temp !=0)
		{
			res.push_back(temp);
			temp = parents[temp];
		}

		//actual path is reversed, reverse it
		reverse(res.begin(), res.end());


		for (auto& x : res)
		{
			if (x == to || x == src)
				continue;
			auto a = Cities.find(Nodes[x]->cityName);
			if (a != nullptr)
				cout << a->key << " ";
		}
	}
	cout << "\n";
}