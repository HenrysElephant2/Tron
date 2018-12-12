#include "Map.h"

Map::Map(){}

Map::Map( int xs, int ys, int zs, unsigned int floorTex, unsigned int wallTex, unsigned int skyTex ) {
	x_size = xs;
	y_size = ys;
	z_size = zs;
	num_tiles = xs * ys * zs;
	num_walls = 2*(xs+zs);
	skyboxTex = skyTex;

	tiles = new Tile*[num_tiles+num_walls];
	// Floor tiles
	for( int i=0; i<x_size; i++ ) {
		for( int j=0; j<y_size; j++ ) {
			for( int k=0; k<z_size; k++ ) {
    			tiles[i*x_size*y_size + j*y_size + k] = new Tile( FLAT_TILE, i, j, k, floorTex );
    		}
		}
	}

	// Wall tiles
	for( int i=num_tiles; i<num_tiles+num_walls; i+=4 ) {
		tiles[i] = new Tile( WALL_TILE, (i-num_tiles)/4, 0, -1, wallTex, i/16.0, i/16.0+.25, 0, 1 );
		tiles[i+1] = new Tile( WALL_TILE, (i-num_tiles)/4, 0, zs, wallTex, i/16.0+.25, i/16.0, 0, 1 );
		tiles[i+2] = new Tile( WALL_TILE, -1, 0, (i-num_tiles)/4, wallTex, i/16.0+.25, i/16.0, 0, 1 );
		tiles[i+3] = new Tile( WALL_TILE, xs, 0, (i-num_tiles)/4, wallTex, i/16.0, i/16.0+.25, 0, 1 );
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

void Map::displayTiles() {
	for( int i=0; i<num_tiles; i++ ) {
		tiles[i]->display();
	}
}

void Map::displayWalls() {
	for( int i=num_tiles; i<num_tiles+num_walls; i++ ) {
		tiles[i]->display();
	}
}

void Map::displaySkybox() {
	glColor3d(0,.3,.5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,skyboxTex);
	glBegin(GL_QUAD_STRIP);
	for( int i=0; i<=360; i+=5 ) {
		if( i%90 == 0 ) {
			glTexCoord2d(1,0); glVertex3d(350+3000*Sin(i),0,350+3000*Cos(i));
			glTexCoord2d(1,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
		}
		glTexCoord2d((i%90)/90.0,0); glVertex3d(350+3000*Sin(i),0,350+3000*Cos(i));
		glTexCoord2d((i%90)/90.0,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2d(.5,0); glVertex3d(350,4000,350);
	for( int i=0; i<=360; i+=5 ) {
		if( i%90 == 0 )
			glTexCoord2d(1,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
		glTexCoord2d((i%90)/90.0,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
	}
	glEnd();
	glPushMatrix();
	glScaled(1,-1,1);
	glBegin(GL_QUAD_STRIP);
	for( int i=0; i<=360; i+=5 ) {
		if( i%90 == 0 ) {
			glTexCoord2d(1,0); glVertex3d(350+3000*Sin(i),0,350+3000*Cos(i));
			glTexCoord2d(1,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
		}
		glTexCoord2d((i%90)/90.0,0); glVertex3d(350+3000*Sin(i),0,350+3000*Cos(i));
		glTexCoord2d((i%90)/90.0,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glTexCoord2d(.5,0); glVertex3d(350,4000,350);
	for( int i=0; i<=360; i+=5 ) {
		if( i%90 == 0 ) {
			glTexCoord2d(1,0); glVertex3d(350,4000,350);
			glTexCoord2d(1,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
		}
		glTexCoord2d((i%90)/90.0,0); glVertex3d(350,4000,350);
		glTexCoord2d((i%90)/90.0,1); glVertex3d(350+3000*Sin(i),3000,350+3000*Cos(i));
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
