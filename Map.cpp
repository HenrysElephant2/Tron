#include "Map.h"

Map::Map(){}

Map::Map( int xs, int ys, int zs ) {
	x_size = xs;
	y_size = ys;
	z_size = zs;
	num_tiles = xs * ys * zs;
	num_walls = 2*(xs+zs);

	tiles = new Tile*[num_tiles+num_walls];
	// Floor tiles
	for( int i=0; i<x_size; i++ ) {
		for( int j=0; j<y_size; j++ ) {
			for( int k=0; k<z_size; k++ ) {
    			tiles[i*x_size*y_size + j*y_size + k] = new Tile( FLAT_TILE, i, j, k );
    		}
		}
	}

	// Wall tiles
	for( int i=num_tiles; i<num_tiles+num_walls; i+=4 ) {
		tiles[i] = new Tile( WALL_TILE, (i-num_tiles)/4, 0, -1 );
		tiles[i+1] = new Tile( WALL_TILE, (i-num_tiles)/4, 0, zs );
		tiles[i+2] = new Tile( WALL_TILE, -1, 0, (i-num_tiles)/4 );
		tiles[i+3] = new Tile( WALL_TILE, xs, 0, (i-num_tiles)/4 );
	}
}

Map::~Map() {
	for( int i=0; i<num_tiles+num_walls; i++ )
		delete tiles[i];
	delete[] tiles;
}

int Map::getXSize() { return x_size; }
int Map::getYSize() { return y_size; }
int Map::getZSize() { return z_size; }
Tile* Map::getTile( int i, int j, int k ) { return tiles[i*x_size*y_size + j*y_size + k]; }
Tile* Map::getWall( int i ) { return tiles[num_tiles+i]; }

bool Map::testWallHits( Hitbox *other ) {
	for( int i=num_tiles; i<num_tiles+num_walls; i++ ) {
		if( tiles[i]->testTileHit(other) )
			return true;
	}
	return false;
}

void Map::display() {
	for( int i=0; i<num_tiles+num_walls; i++ ) {
		tiles[i]->display();
	}
}