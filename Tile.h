#ifndef TILE_H
#define TILE_H

#include "Hitbox.h"

class Tile {
private:
	int map_x, map_y, map_z; // Tile location
	Hitbox hitboxes[];

public:
	Tile();
	bool isColliding( Player p );
};

#endif