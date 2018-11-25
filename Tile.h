#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "Hitbox.h"

#define FLAT_TILE 0
#define WALL_TILE 1

#define TILE_SIZE 100 // decimeters

class Tile {
private:
	int map_x, map_y, map_z; // Tile location
	int type;
	Hitbox *hitbox;

public:
	Tile();
	Tile( int tileType, int mx, int my, int mz );
	~Tile();

	bool testTileHit( Hitbox *other );

	void display();
};

#endif