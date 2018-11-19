#ifndef MAP_H
#define MAP_H

#include "Tile.h"

class Map {
private:
	int x_size, y_size, z_size, num_tiles; // Dimensions of map in terms of number of tiles in every direction
	Tile** tiles; // Array of tiles in x,y,z order

public:
	Map();
	Map( int xs, int ys, int zs );
	~Map();

	int getXSize();
	int getYSize();
	int getZSize();
	Tile* getTile( int i, int j, int k );

	void display();
};

#endif