#include "Menu.h"

Menu::Menu( int w, int h ) : GameState(w,h) {
	state = MAIN_MENU;

	p1color = 6;
	p2color = 2;

	player1 = new Player( 0, 0, 0, 0, 0, 1, bike, NULL, colors[p1color] );
	player2 = new Player( (MAP_LENGTH-1)*TILE_SIZE, 0, (MAP_WIDTH-1)*TILE_SIZE, 0, 0, -1, bike, NULL, colors[p2color] );

	player1->setLoc( Vector( (MAP_LENGTH-1)/2.0*TILE_SIZE+25, 0, (MAP_LENGTH-1)/2.0*TILE_SIZE ) );
	player1->setDir( Vector(-1,0,-1) );
	player2->setLoc( Vector( (MAP_LENGTH-1)/2.0*TILE_SIZE-25, 0, (MAP_LENGTH-1)/2.0*TILE_SIZE ) );
	player2->setDir( Vector(1,0,-1) );

	unsigned int floorTex = LoadTexBMP("Textures/tile.bmp");
	unsigned int wallTex = LoadTexBMP("Textures/wall.bmp");
	unsigned int skyTex = LoadTexBMP("Textures/sky.bmp");
	map = new Map( MAP_LENGTH, 1, MAP_WIDTH, floorTex, wallTex, skyTex );


	tronLogo = Button(.48,.72,.7,.175,labelTextures,0,1,.75,1,menuButtonColor);
	playButton = Button(.5,.52,.5,.125,buttonTexture,0,1,.75,1,menuButtonColor);
	colorsButton = Button(.5,.37,.5,.125,buttonTexture,0,1,.5,.75,menuButtonColor);
	quitButton = Button(.5,.22,.5,.125,buttonTexture,0,1,0,.25,menuButtonColor);

	backButton = Button(.5,.9,.4,.1,buttonTexture,0,1,.25,.5,menuButtonColor);

	for( int i=0; i<NCOLORS; i++ ) {
		p1cButtons[i] = Button(.4,.8-(i*.08),.18,.06,buttonTexture2,0,1.005,0,.25,colors[i]);
		p2cButtons[i] = Button(.6,.8-(i*.08),.18,.06,buttonTexture2,0,1.005,0,.25,colors[i]);
	}
	p1cButtons[p1color].setText(.25); p1cButtons[p1color].setTexb(.5);
	p2cButtons[p2color].setText(.25); p2cButtons[p2color].setTexb(.5);
}

Menu::~Menu() {
	delete player1;
	delete player2;
	delete map;
}

void Menu::keyDown(SDL_Keycode key) {
	switch(key) {
		case SDLK_1: bloomOn = true; break;
		case SDLK_2: bloomOn = false; break;
	}
}
void Menu::keyUp(SDL_Keycode key) {}

void Menu::mouseDown(int x, int y) {
	if( state == MAIN_MENU ) {
		if( playButton.testClicked(x,y,wWidth,wHeight) )
			mouseState = PLAY_DOWN;
		else if( colorsButton.testClicked(x,y,wWidth,wHeight) ) 
			mouseState = COLORS_DOWN;
		else if( quitButton.testClicked(x,y,wWidth,wHeight) ) 
			mouseState = QUIT_DOWN;
	}
	else if( state == COLOR_SELECT ) {
		if( backButton.testClicked(x,y,wWidth,wHeight) )
			mouseState = BACK_DOWN;
		else {
			for( int i=0; i<NCOLORS; i++ ) {
				if( p1cButtons[i].testClicked(x,y,wWidth,wHeight) )
					mouseState = BUTTON(1,i);
				if( p2cButtons[i].testClicked(x,y,wWidth,wHeight) )
					mouseState = BUTTON(2,i);
			}
		}
	}
}

void Menu::mouseUp(int x, int y) {
	if( state == MAIN_MENU ) {
		if( mouseState == PLAY_DOWN && playButton.testClicked(x,y,wWidth,wHeight) ) {
			GameState *newState = new Gameplay(wWidth, wHeight, colors[p1color], colors[p2color]);
			setNextState( newState );
		}
		else if( mouseState == COLORS_DOWN && colorsButton.testClicked(x,y,wWidth,wHeight) )
			state = COLOR_SELECT;
		else if( mouseState == QUIT_DOWN && quitButton.testClicked(x,y,wWidth,wHeight) )
			setQuit(true);
	}
	else if( state == COLOR_SELECT ) {
		if( mouseState == BACK_DOWN && backButton.testClicked(x,y,wWidth,wHeight) )
			state = MAIN_MENU;
		else {
			for( int i=0; i<NCOLORS; i++ ) {
				if( mouseState == BUTTON(1,i) && p1cButtons[i].testClicked(x,y,wWidth,wHeight) )
					p1color = i;
				if( mouseState == BUTTON(2,i) && p2cButtons[i].testClicked(x,y,wWidth,wHeight) )
					p2color = i;
			}
			player1->setColor(colors[p1color]);
			player2->setColor(colors[p2color]);
		}
		
		for( int i=0; i<NCOLORS; i++ ) {
			p1cButtons[i].setTexb(i==p1color?.5:.25); p1cButtons[i].setText(i==p1color?.25:0);
			p2cButtons[i].setTexb(i==p2color?.5:.25); p2cButtons[i].setText(i==p2color?.25:0);
		}
	}
	mouseState = NONE_DOWN;
}

void Menu::display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	Vector viewLoc;
	Vector targetLoc;
	double Ex = (MAP_LENGTH-1)/2.0*TILE_SIZE;
	double Ey = 3;
	double Ez = -60 + (MAP_LENGTH-1)/2.0*TILE_SIZE;

	double Tx = (MAP_LENGTH-1)/2.0*TILE_SIZE;
	double Ty = 10;
	double Tz = (MAP_LENGTH-1)/2.0*TILE_SIZE;

	viewLoc = Vector(Ex, Ey, Ez);
	targetLoc = Vector(Tx, Ty, Tz);
	gluLookAt(Ex,Ey,Ez , Tx,Ty, Tz, 0,1,0);

	postProcessingSetup();
	displayAll( &viewLoc, &targetLoc, false );
	postProcessing();
}

void Menu::update() {}

void Menu::displayAll( Vector *cameraPos, Vector *targetLoc, bool renderBloom) {

	glUseProgram(renderBloom ? brightProgram:0);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

	map->displayWalls();
	map->displaySkybox();

	// Reflections/Underside of map
	Vector reflectCamera = Add0( *cameraPos, Vector(0,-2*cameraPos->getY(), 0) );
	TransparentRenderer *tr = new TransparentRenderer();
	glPushMatrix();
	glTranslated(0,-.12,0);
	glScaled(1,-1,1);
	glUseProgram(bikeProgram);
	player1->display(tr, &reflectCamera);
	glUseProgram(bikeProgram);
	player2->display(tr, &reflectCamera);
	glUseProgram(0);
	tr->display( &reflectCamera );
	glPopMatrix();
	delete tr;

	// Floor tiles
	map->displayTiles();

	// Top of map
	tr = new TransparentRenderer();
	glUseProgram(bikeProgram);
	player1->display(tr, cameraPos);
	glUseProgram(bikeProgram);
	player2->display(tr, cameraPos);
	glUseProgram(0);
	tr->display( cameraPos );
	delete tr;



	switchToOverlay();

	if( state == MAIN_MENU ) {
		tronLogo.display();
	   	playButton.display();
	   	colorsButton.display();
	   	quitButton.display();
	}
	else if( state == COLOR_SELECT ) {
		backButton.display();
		for( int i=0; i<NCOLORS; i++ ) {
			p1cButtons[i].display();
			p2cButtons[i].display();
		}
	}

	switchTo3D();
}
