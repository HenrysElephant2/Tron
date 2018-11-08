#ifndef MAP_H
#define MAP_H

#define FLAT_TILE 0

class Map {
private:
	int x_size, y_size, z_size; // Dimensions of map in terms of number of tiles in every direction
	Tile map_tiles[][][]; // Array of tiles in x,y,z order

public:
	Map();

	int getXSize();
	int getYSize();
	int getZSize();
};

#endif