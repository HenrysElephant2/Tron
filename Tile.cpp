#include "Tile.h"

Tile::Tile(){}

Tile::Tile( int tileType, int mx, int my, int mz, unsigned int t ) {
	map_x = mx; map_y = my; map_z = mz;
	type = tileType;
	texture = t;

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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	if( type == FLAT_TILE ) {
		glColor3d(.5,.8,1);
		glBegin( GL_POLYGON );
			glTexCoord2f(0,0); hitbox->getPoint(TBL)->gl();
			glTexCoord2f(1,0); hitbox->getPoint(TBR)->gl();
			glTexCoord2f(1,1); hitbox->getPoint(TFR)->gl();
			glTexCoord2f(0,1); hitbox->getPoint(TFL)->gl();
		glEnd();
	}
	else if( type == WALL_TILE ) {
		glColor3d(.5,.8,1);
		glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0,0); hitbox->getPoint(TBL)->gl();
			glTexCoord2f(0,1); hitbox->getPoint(BBL)->gl();
			glTexCoord2f(1,0); hitbox->getPoint(TBR)->gl();
			glTexCoord2f(1,1); hitbox->getPoint(BBR)->gl();
			glTexCoord2f(0,0); hitbox->getPoint(TFR)->gl();
			glTexCoord2f(0,1); hitbox->getPoint(BFR)->gl();
			glTexCoord2f(1,0); hitbox->getPoint(TFL)->gl();
			glTexCoord2f(1,1); hitbox->getPoint(BFL)->gl();
			glTexCoord2f(0,0); hitbox->getPoint(TBL)->gl();
			glTexCoord2f(0,1); hitbox->getPoint(BBL)->gl();
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	// hitbox->renderSelf(true);
}