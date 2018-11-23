#include "Tile.h"

Tile::Tile(){}

Tile::Tile( int tileType, int mx, int my, int mz ) {
	map_x = mx; map_y = my; map_z = mz;
	type = tileType;

	if( type == FLAT_TILE ) {
		Vector *pos = new Vector( map_x * TILE_SIZE, (map_y - .1) * TILE_SIZE, map_z * TILE_SIZE );
		Vector *uVec = new Vector(0,1,0);
		Vector *fVec = new Vector(1,0,0);
		hitbox = new Hitbox( pos, fVec, uVec, TILE_SIZE, TILE_SIZE, .1*TILE_SIZE );
	}
	else if( type == WALL_TILE ) {
		Vector *pos = new Vector( map_x * TILE_SIZE, map_y * TILE_SIZE, map_z * TILE_SIZE );
		Vector *uVec = new Vector(0,1,0);
		Vector *fVec = new Vector(1,0,0);
		hitbox = new Hitbox( pos, fVec, uVec, TILE_SIZE, TILE_SIZE, TILE_SIZE );
	}
	else {
		throw "ya fucked up\n";
	}
}

Tile::~Tile() {
	delete hitbox;
}

bool Tile::testTileHit( Hitbox *other ) {
	return testCollision( hitbox, other );
}

void Tile::display() {
// 	if( type == FLAT_TILE ) {
// 		glColor3d(1,0,0);
// 		glBegin( GL_POLYGON );
// 			hitbox->getPoint(TBL)->gl();
// 			hitbox->getPoint(TBR)->gl();
// 			hitbox->getPoint(TFR)->gl();
// 			hitbox->getPoint(TFL)->gl();
// 		glEnd();
// 	}
// 	else if( type == WALL_TILE ) {
// 		glColor3d(1,1,0);
// 		glBegin(GL_QUAD_STRIP);
// 			hitbox->getPoint(TBL)->gl();
// 			hitbox->getPoint(BBL)->gl();
// 			hitbox->getPoint(TBR)->gl();
// 			hitbox->getPoint(BBR)->gl();
// 			hitbox->getPoint(TFR)->gl();
// 			hitbox->getPoint(BFR)->gl();
// 			hitbox->getPoint(TFL)->gl();
// 			hitbox->getPoint(BFL)->gl();
// 			hitbox->getPoint(TBL)->gl();
// 			hitbox->getPoint(BBL)->gl();
// 		glEnd();
// 	}
	hitbox->renderSelf(true);
}