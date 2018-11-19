#include "Tile.h"

Tile::Tile(){}

Tile::Tile( int tileType, int mx, int my, int mz ) {
	map_x = mx; map_y = my; map_z = mz;
	type = tileType;

	if( type == FLAT_TILE ) {
		Vector *pos = new Vector( map_x * TILE_SIZE, (map_y - .01) * TILE_SIZE, map_z * TILE_SIZE );
		Vector *uVec = new Vector(0,1,0);
		Vector *fVec = new Vector(1,0,0);
		hitbox = new Hitbox( pos, uVec, fVec, TILE_SIZE, .01 * TILE_SIZE, TILE_SIZE );
	}
	else {
		throw "ya fucked up\n";
	}
}

Tile::~Tile() {
	delete hitbox;
}

void Tile::display() {
	glColor3d(0,0,.3);
	glBegin( GL_POLYGON );
		hitbox->getPoint(TBL)->gl();
		hitbox->getPoint(TBR)->gl();
		hitbox->getPoint(TFR)->gl();
		hitbox->getPoint(TFL)->gl();
	glEnd();
}