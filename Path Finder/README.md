# Path Finder
Project written as an assignment job. It main purpose was to create algorithm that can find the shortest path from point A to point B. This project contains implementation of 2 algorithms to solve the problem. It also uses
hand-written data structures including **RB Tree**, **Vector**, **Priority Queue** and **String**

## Input
```
# - Road  
* - City (Name of city should be near)  
. - Empty
```
Input consists of:
+ Map size(x, y)
+ Map
+ Number of flight connections
+ Flight connections (From, To, Cost)
+ Number of paths we want to get
+ Paths (From, To, Bool: Show  Path - Cities we have  to go through to get to out destination)  
### Example input:  
```
20 20
.........GDANSK.....
........*...........
........#...........
........#...........
*##################.
#SZCZECIN.........#.
#.................#.
##................#.
.############*#####.
.#...WARSZAWA.......
.#..................
.#############......
.#...........#......
.#..WROCLAW.##......
.#..*.......*.......
.####.......#KIELCE.
......*##.#########.
.OPOLE..#.*.......#.
........#.KRAKOW..#.
........###########.
2
KRAKOW GDANSK 10
WARSZAWA WROCLAW 3
3
KRAKOW GDANSK 1
WARSZAWA WROCLAW 1
SZCZECIN KRAKOW 1
```
### Output:
```
10
3
29 KIELCE
```

# Algorithms and Containers
## Dijkstra
Dijkstra's algorithm was the first approach to solve the problem. While it solved the problem and was easy to implement, it did not pass time constraints. This is because Dijkstra's algorithm finds 
all possible paths from every point to every point, which resulted in a lot of computation.

## A*
A* was the second approach to solve the problem. It turned out to be perfect solution to the problem, passing time constraints and being able to find proper path even on very lage maps

## Red-Black Tree
RB tree was primaly designed for fast storage of node data. Array approach turned out to outperform it, so only static data, like city coordinates, names are stored in it.

## Priority Queue
Very simple implementation for the purpose of algorithm. I think that this data structure is the reason why this program canot pass the hardest last test. Maybe implementation using Fibonacci heap 
could solve the problem.

## Vector
Vector is very simple data structure to store small amount of data. Custom string implementation is just the vector of chars. It has also C++ iterator implemented, so code looks really clean.

# Further Development
No further development is planned. This code works really good and is really good foundation for other implementations for other purposes (like 2D game engines)
