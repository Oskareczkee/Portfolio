#include <iostream>
#include <SFML/Graphics.hpp>
#include "Maze Generator.h"
#include "MazeDrawer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Maze generator");
    window.clear(Color::Black);

    MazeGenerator mg(20, 20);
    MazeDrawer md(window);
    md.loadMaze(mg.generateMaze());

    while (window.isOpen())
    {
        int x = 0, y = 0;
        cout << "Please enter maze size\nwidth: ";
        cin >> x;
        cout << "height: ";
        cin >> y;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        mg.setWidth(x);
        mg.setHeight(y);
        md.loadMaze(mg.generateMaze());

        window.clear(Color::Black);
        md.draw();
        window.display();
    }

    return 0;
}