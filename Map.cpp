#include "Map.h"

Map::Map(){}

Map::Map( int xs, int ys, int zs ) {
	x_size = xs;
	y_size = ys;
	z_size = zs;
	num_tiles = xs * ys * zs;

	tiles = new Tile*[num_tiles];
	for( int i=0; i<x_size; i++ ) {
		for( int j=0; j<y_size; j++ ) {
			for( int k=0; k<z_size; k++ ) {
    			tiles[i*x_size*y_size + j*y_size + k] = new Tile( FLAT_TILE, i*TILE_SIZE, j*TILE_SIZE, k*TILE_SIZE );
    		}
		}
	}
}

Map::~Map() {
	for( int i=0; i<num_tiles; i++ )
		delete tiles[i];
	delete[] tiles;
}

int Map::getXSize() { return x_size; }
int Map::getYSize() { return y_size; }
int Map::getZSize() { return z_size; }
Tile* Map::getTile( int i, int j, int k ) { return tiles[i*x_size*y_size + j*y_size + k]; }

void Map::display() {
	for( int i=0; i<num_tiles; i++ ) {
		tiles[i]->display();
	}
}