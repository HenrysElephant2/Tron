#include "Tile.h"

Tile::Tile(){}

Tile::Tile( int tileType, int mx, int my, int mz, unsigned int t, double tl, double tr, double tb, double tt ) {
	map_x = mx; map_y = my; map_z = mz;
	type = tileType;
	texture = t;
	texl = tl;
	texr = tr;
	texb = tb;
	text = tt;

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
		throw "oof\n";
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
	glPushMatrix();
	glTranslated(0,-.12,0);
	if( type == FLAT_TILE ) {
		glDepthMask(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

		glColor4d(.8,.9,1,FLOOR_TILE_ALPHA);
		glBegin( GL_POLYGON );
			glTexCoord2f(texl,texb); hitbox->getPoint(TBL)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(TBR)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(TFR)->gl();
			glTexCoord2f(texb,text); hitbox->getPoint(TFL)->gl();
		glEnd();

		glDisable(GL_BLEND);
		glDepthMask(1);
	}
	else if( type == WALL_TILE ) {
		glColor3d(.8,.9,1);
		glBegin(GL_QUADS);
			glTexCoord2f(texl,texb); hitbox->getPoint(BBL)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(BBR)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(TBR)->gl();
			glTexCoord2f(texl,text); hitbox->getPoint(TBL)->gl();

			glTexCoord2f(texl,texb); hitbox->getPoint(BBR)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(BFR)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(TFR)->gl();
			glTexCoord2f(texl,text); hitbox->getPoint(TBR)->gl();
			
			glTexCoord2f(texl,texb); hitbox->getPoint(BFR)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(BFL)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(TFL)->gl();
			glTexCoord2f(texl,text); hitbox->getPoint(TFR)->gl();
			
			glTexCoord2f(texl,texb); hitbox->getPoint(BFL)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(BBL)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(TBL)->gl();
			glTexCoord2f(texl,text); hitbox->getPoint(TFL)->gl();
		glEnd();

		glPushMatrix();
		glTranslated(0,-TILE_SIZE,0);
		glColor3d(.8,.9,1);
		glBegin(GL_QUADS);
			glTexCoord2f(texl,text); hitbox->getPoint(BBL)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(BBR)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(TBR)->gl();
			glTexCoord2f(texl,texb); hitbox->getPoint(TBL)->gl();

			glTexCoord2f(texl,text); hitbox->getPoint(BBR)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(BFR)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(TFR)->gl();
			glTexCoord2f(texl,texb); hitbox->getPoint(TBR)->gl();
			
			glTexCoord2f(texl,text); hitbox->getPoint(BFR)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(BFL)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(TFL)->gl();
			glTexCoord2f(texl,texb); hitbox->getPoint(TFR)->gl();
			
			glTexCoord2f(texl,text); hitbox->getPoint(BFL)->gl();
			glTexCoord2f(texr,text); hitbox->getPoint(BBL)->gl();
			glTexCoord2f(texr,texb); hitbox->getPoint(TBL)->gl();
			glTexCoord2f(texl,texb); hitbox->getPoint(TFL)->gl();
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}