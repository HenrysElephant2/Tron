#ifndef MAP_H
#define MAP_H

#include "Tile.h"
#include "Hitbox.h"
#include "LoadModel.h"
#include "TransparentRenderer.h"

#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#define Cos(x) cos(3.1415926535/180*(x))
#define Sin(x) sin(3.1415926535/180*(x))
#endif

class Map {
private:
	int x_size, y_size, z_size, num_tiles, num_walls; // Dimensions of map in terms of number of tiles in every direction
	unsigned int skyboxTex;
	Tile** tiles; // Array of tiles and walls in some order (see getTile function)

public:
	Map();
	Map( int xs, int ys, int zs, unsigned int floorTex, unsigned int wallTex, unsigned int skyTex );
	~Map();

	int getXSize();
	int getYSize();
	int getZSize();
	Tile* getTile( int i, int j, int k );
	Tile* getWall( int i );

	bool testWallHits( Hitbox *other );

	void displayTiles();
	void displayWalls();
	void displaySkybox();
};

#endif