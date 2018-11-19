#include "Gameplay.h"

Gameplay::Gameplay() {
	player1 = new Player();
	player2 = new Player();
	map = new Map();

	state = STATE_WAITING;
}

Gameplay::~Gameplay() {
	delete player1;
	delete player2;
	delete map;
}

void Gameplay::keyDown( SDL_Keycode key, int x, int y ) {
	//switch( key ) {
	//	case
	//}
}

void Gameplay::keyUp( SDL_Keycode key, int x, int y ) {
	//switch( key ) {
	//	case
	//}
}

void Gameplay::special(int key, int x, int y) { /*NEEDED in GLUT, maybe different in other system */

}

void Gameplay::mouse(/*TODO*/) {
	
}

void Gameplay::update() {
	double tDiff = getElapsedTime();

	if( state == STATE_PLAYING ) {
		player1->movePlayer(tDiff);
		player2->movePlayer(tDiff);

		if( testCollision( player1->getHitbox(), player2->getHitbox() ) ) {
			state = STATE_TIE;
		}
		else if( player1->getTrail()->testTrailHit( player2->getHitbox() ) ) {
			state = STATE_P1_WIN;
		}
		else if( player2->getTrail()->testTrailHit( player1->getHitbox() ) ) {
			state = STATE_P2_WIN;
		}
	}
}

void Gameplay::display() {
	map->display();
	player1->display();
	player2->display();
}