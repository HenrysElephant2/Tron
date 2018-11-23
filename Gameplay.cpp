#include "Gameplay.h"

Gameplay::Gameplay() {
	player1 = new Player( 0, 0, 0, 0, 0, 1 );
	player2 = new Player( 4*TILE_SIZE, 0, 4*TILE_SIZE, 0, 0, -1 );
	map = new Map( 5, 1, 5 );

	state = STATE_WAITING;

	ph = 0;	  //  Elevation of view angle
	th = 0;	  //  Azimuth of view angle

	// Booleans to control window movement
	thup = false;
	thdown = false;
	phup = false;
	phdown = false;
}

Gameplay::~Gameplay() {
	delete player1;
	delete player2;
	delete map;
}

void Gameplay::keyDown( SDL_Keycode key, int x, int y ) {
	switch( key ) {
		case SDLK_a: player1->setLeft(true); break;
		case SDLK_d: player1->setRight(true); break;

		case SDLK_j: player2->setLeft(true); break;
		case SDLK_l: player2->setRight(true); break;


		case SDLK_t: phup = true; break;
		case SDLK_g: phdown = true; break;
		case SDLK_f: thdown = true; break;
		case SDLK_h: thup = true; break;

		case SDLK_SPACE:
			if( state == STATE_WAITING ) {
				state = STATE_PLAYING;
				player1->beginTrail(true);
				player2->beginTrail(true);
			}
		break;

		case SDLK_r:
			if( state == STATE_P1_WIN || state == STATE_P2_WIN || state == STATE_TIE ) {
				reset();
			}
		break;
	}
}

void Gameplay::keyUp( SDL_Keycode key, int x, int y ) {
	switch( key ) {
		case SDLK_a: player1->setLeft(false); break;
		case SDLK_d: player1->setRight(false); break;

		case SDLK_j: player2->setLeft(false); break;
		case SDLK_l: player2->setRight(false); break;


		case SDLK_t: phup = false; break;
		case SDLK_g: phdown = false; break;
		case SDLK_f: thdown = false; break;
		case SDLK_h: thup = false; break;
	}
}

void Gameplay::special(int key, int x, int y) { /*NEEDED in GLUT, maybe different in other system */

}

void Gameplay::mouse(/*TODO*/) {
	
}

void Gameplay::update() {
	double tDiff = getElapsedTime();

	th += (thup-thdown)*50*tDiff;
	ph += (phup-phdown)*50*tDiff;

	if( state == STATE_PLAYING ) {
		player1->movePlayer(tDiff);
		player2->movePlayer(tDiff);

		if( testCollision( player1->getHitbox(), player2->getHitbox() ) ) {
			state = STATE_TIE;
			player1->setAlive(false);
			player2->setAlive(false);
		}
		if( player1->getTrail()->testTrailHit( player2->getHitbox() ) || map->testWallHits( player2->getHitbox() ) ) {
			state = STATE_P1_WIN;
			player2->setAlive(false);
		}
		if( player2->getTrail()->testTrailHit( player1->getHitbox() ) || map->testWallHits( player1->getHitbox() ) ) {
			if( state == STATE_P1_WIN )
				state = STATE_TIE;
			else
				state = STATE_P2_WIN;
			player1->setAlive(false);
		}
	}
}

void Gameplay::reset() {
	delete player1;
	delete player2;
	player1 = new Player( 0, 0, 0, 0, 0, 1 );
	player2 = new Player( 4*TILE_SIZE, 0, 4*TILE_SIZE, 0, 0, -1 );

	state = STATE_WAITING;
}

void Gameplay::display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	double Ex = -500*Sin(th)*Cos(ph) + 2*TILE_SIZE;
	double Ey = +500*Sin(ph);
	double Ez = +500*Cos(th)*Cos(ph) + 2*TILE_SIZE;
	gluLookAt(Ex,Ey,Ez , 2*TILE_SIZE,0,2*TILE_SIZE , 0,Cos(ph),0);
	
	map->display();
	player1->display();
	player2->display();
}