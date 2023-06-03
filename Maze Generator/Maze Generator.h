#pragma once
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

struct MazeTile {
	//maze walls, boolean means if they exist
	bool left = true;
	bool right = true;
	bool up = true;
	bool down = true;

	bool isVisited = false;
};

struct Coords
{
	int x=0, y=0;
	bool operator==(const Coords& other)
	{
		return x == other.x && y == other.y;
	}
};

enum class Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

class MazeGenerator
{
private:
	int tileWidth;
	int tileHeight;

	vector<vector<MazeTile>> tiles;
	vector<Coords> unvisitedTiles;

	random_device rd;
	mt19937 mt;
public:
	MazeGenerator(const int& width, const int& height) : tileWidth(width), tileHeight(height) { mt = mt19937(rd());}

	void setWidth(const int& width) { tileWidth = width; }
	void setHeight(const int& height) { tileHeight = height; }
	int getWidth() { return tileWidth;}
	int getHeight() { return tileHeight; }

	//function that generates maze, will return vector of vectors (it's like double array [][])
	vector<vector<MazeTile>> generateMaze()
	{
		//use vector constructor with memory allocation, because we know to size of output vector
		//this will make allocation operations as fast as array allocation
		tiles = vector<vector<MazeTile>>(tileWidth);
		for (auto& vec : tiles)
			vec=vector<MazeTile>(tileHeight);//preallocate rows

		//we have to keep track of tiles that we have not visited yet
		unvisitedTiles = vector<Coords>();
		for (int x = 0; x < tileWidth; x++)
			for (int y = 0; y < tileHeight; y++)
				unvisitedTiles.push_back({ x,y });

		//GENERATION
		Coords tileCoords = unvisitedTiles[getRandomNumber(0, unvisitedTiles.size() - 1)]; //get random coordinates to start

		bool iterationBegin = true;//this bool used to indicate if 
		vector<Coords> actualIteration;

		while (unvisitedTiles.size() != 0)
		{
			//at the beginning of each iteration try to find visited cell and make path with it
			if (iterationBegin)
			{
				auto visitedCells = getVisitedNeighbors(tileCoords);
				if (visitedCells.size() != 0)
				{
					Coords rn = visitedCells[getRandomNumber(0, visitedCells.size()-1)]; // get random visited cell
					makePath(tileCoords, rn);
				}
				iterationBegin = false;
			}
			

			tiles[tileCoords.x][tileCoords.y].isVisited = true; //mark tile as visited
			unvisitedTiles.erase(remove(unvisitedTiles.begin(), unvisitedTiles.end(), tileCoords), unvisitedTiles.end()); //remove tile from unvisited tiles

			auto neighbors = getNeighbors(tileCoords);


			if (neighbors.size() == 0)// tile has no free neighbors
			{
				if (unvisitedTiles.size() == 0)//there are no tiles left, break, do not get random next tile
					break;

				//when on the end try to connect to the tile that has not been traversed this iteration and is visited
				auto visitedNeighbors = getVisitedNeighbors(tileCoords);
				for (auto& v : visitedNeighbors)
				{
					if (find(actualIteration.begin(), actualIteration.end(), v) == actualIteration.end())//if not found, (v is unique)
					{
						makePath(tileCoords, v);
						break;
					}
				}

				actualIteration.clear();

				tileCoords = unvisitedTiles[getRandomNumber(0, unvisitedTiles.size()-1)]; //get random unvisited tile
				iterationBegin = true;
				continue;
			}

			Coords rn = neighbors[getRandomNumber(0, neighbors.size()-1)];//get random neighbor

			makePath(tileCoords, rn);
			actualIteration.push_back(tileCoords);

			tileCoords = { rn.x, rn.y };
		}

		return tiles;
	}

private:

	//make path between 2 tiles
	void makePath(Coords& from, Coords& to)
	{
		Direction direction = getMoveDirection(from, to); // get the direction of the move
		markWall(from, direction);
		markWall(to, getOppositeDirection(direction));
	}

	void markWall(const Coords& tileCoords, const Direction& direction)
	{
		if (direction == Direction::LEFT)
			tiles[tileCoords.x][tileCoords.y].left = false;
		else if (direction == Direction::RIGHT)
			tiles[tileCoords.x][tileCoords.y].right = false;
		else if (direction == Direction::UP)
			tiles[tileCoords.x][tileCoords.y].up = false;
		else if (direction == Direction::DOWN)
			tiles[tileCoords.x][tileCoords.y].down = false;
	}

	Direction getMoveDirection(const Coords& from, const Coords& to)
	{
		//check vertical axis
		if (from.y - to.y >0)
			return Direction::UP;
		else if (from.y - to.y < 0)
			return Direction::DOWN;

		//check horizontal axis
		else if (from.x - to.x > 0)
			return Direction::LEFT;
		else if (from.x - to.x < 0)
			return Direction::RIGHT;

		//from == to, no movement
		return Direction::NONE;
	}

	Direction getOppositeDirection(const Direction& dir)
	{
		if (dir == Direction::LEFT)
			return Direction::RIGHT;
		else if (dir == Direction::RIGHT)
			return Direction::LEFT;
		else if (dir == Direction::UP)
			return Direction::DOWN;
		else if (dir == Direction::DOWN)
			return Direction::UP;

		return Direction::NONE;
	}

	int getRandomNumber(const int& from, const int& to)
	{
		uniform_int_distribution <int> dist (from, to);
		return dist(mt);
	}

	bool isValidCoordinate(const Coords& coord)
	{
		return coord.x >= 0 && coord.x < tileWidth && coord.y >= 0 && coord.y < tileHeight;
	}

	bool isVisited(const Coords& coords)
	{
		return tiles[coords.x][coords.y].isVisited;
	}

	vector<Coords> getVisitedNeighbors(const Coords& tileCoords)
	{
		vector<Coords> neighbors =
		{
			Coords{tileCoords.x + 1, tileCoords.y},
			Coords{tileCoords.x - 1 , tileCoords.y},
			Coords{tileCoords.x, tileCoords.y + 1},
			Coords{tileCoords.x, tileCoords.y - 1}
		};

		vector<Coords> validNeighbors; //neighbors that are accessible (are not out of bounds, and are not visited)
		for (auto& n : neighbors)
			if (isValidCoordinate(n) && isVisited(n))
				validNeighbors.push_back(n);

		return validNeighbors;
	}

	vector<Coords> getNeighbors(const Coords& tileCoords)
	{
		vector<Coords> neighbors =
		{
			Coords{tileCoords.x + 1, tileCoords.y},
			Coords{tileCoords.x - 1 , tileCoords.y},
			Coords{tileCoords.x, tileCoords.y + 1},
			Coords{tileCoords.x, tileCoords.y - 1}
		};

		vector<Coords> validNeighbors; //neighbors that are accessible (are not out of bounds, and are not visited)
		for (auto& n : neighbors)
			if (isValidCoordinate(n) && !isVisited(n))
				validNeighbors.push_back(n);

		return validNeighbors;
	}

};