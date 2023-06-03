#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Maze Generator.h"

using namespace sf;

using Maze = std::vector<std::vector<MazeTile>>;

class MazeDrawer
{
private:
	Maze maze;
	RenderWindow& window;

	int mazeTileWidth = 0;
	int mazeTileHeight = 0;

	int TileSize = 30;
	int TileWallThickness = 2;
public:
	MazeDrawer(RenderWindow& _window): window(_window){}
	void loadMaze(const Maze& _maze) 
	{ 
		maze = _maze;
		mazeTileWidth = maze.size();
		mazeTileHeight = maze[0].size();

		//make tile sizes corresponding to the screen size
		FloatRect visibleArea(0, 0, (mazeTileWidth * TileSize) + TileWallThickness, (mazeTileHeight * TileSize) + TileWallThickness);

		window.setView(View(visibleArea));
	}

	void draw()
	{	
		for(int x =0;x<mazeTileWidth;x++)
			for (int y = 0; y < mazeTileHeight; y++)
			{
				int posX = x * TileSize;
				int posY = y * TileSize;

				drawTile(window, maze[x][y], posX, posY);
			}
		
	}

private: 
	void drawTile(RenderWindow& window, MazeTile tile, int posX, int posY)
	{
		//draw rectangle without proper walls
		if (tile.up)
		{
			RectangleShape line(Vector2f(TileSize + TileWallThickness, TileWallThickness));
			line.setPosition(posX, posY);
			line.setFillColor(Color::White);
			window.draw(line);
		}

		if (tile.down)
		{
			RectangleShape line(Vector2f(TileSize + TileWallThickness, TileWallThickness));
			line.setPosition(posX, posY + TileSize);
			line.setFillColor(Color::White);
			window.draw(line);
		}

		if (tile.right)
		{
			RectangleShape line(Vector2f(TileWallThickness, TileSize));
			line.setPosition(posX + TileSize, posY);
			line.setFillColor(Color::White);
			window.draw(line);
		}

		if (tile.left)
		{
			RectangleShape line(Vector2f(TileWallThickness, TileSize));
			line.setPosition(posX, posY);
			line.setFillColor(Color::White);
			window.draw(line);
		}
	}


};