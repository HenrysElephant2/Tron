#include "Gameplay.h"

GameState::GameState() {
	wWidth = 0;
	wHeight = 0;
	previousTime = 0;
	fov = 45;
	asp = 1.4;
	split = false;

	
	// set up programs

	bikeProgram = glCreateProgram();
	char shader_name[] = "Shaders/bike.frag";
	int c = CreateShader(GL_FRAGMENT_SHADER, shader_name);
	glAttachShader(bikeProgram,c);
	char shader_name2[] = "Shaders/bike.vert";
	int c2 = CreateShader(GL_VERTEX_SHADER, shader_name2);
	glAttachShader(bikeProgram,c2);
	glLinkProgram(bikeProgram);
	PrintProgramLog(bikeProgram);

	blurProgram = glCreateProgram();
	char blurShaderFrag[] = "Shaders/blur.frag";
	int c3 = CreateShader(GL_FRAGMENT_SHADER, blurShaderFrag);
	glAttachShader(blurProgram,c3);
	char blurShaderVert[] = "Shaders/blur.vert";
	int c6 = CreateShader(GL_VERTEX_SHADER, blurShaderVert);
	glAttachShader(blurProgram,c6);
	glLinkProgram(blurProgram);
	PrintProgramLog(blurProgram);

	blendProgram = glCreateProgram();
	char blendShaderFrag[] = "Shaders/blend.frag";
	int c4 = CreateShader(GL_FRAGMENT_SHADER, blendShaderFrag);
	glAttachShader(blendProgram,c4);
	char blendShaderVert[] = "Shaders/blend.vert";
	int c5 = CreateShader(GL_VERTEX_SHADER, blendShaderVert);
	glAttachShader(blendProgram,c5);
	glLinkProgram(blendProgram);
	PrintProgramLog(blendProgram);

	brightProgram = glCreateProgram();
	char brightShaderFrag[] = "Shaders/bright.frag";
	int c7 = CreateShader(GL_FRAGMENT_SHADER, brightShaderFrag);
	glAttachShader(brightProgram,c7);
	char brightShaderVert[] = "Shaders/bright.vert";
	int c8 = CreateShader(GL_VERTEX_SHADER, brightShaderVert);
	glAttachShader(brightProgram,c8);
	glLinkProgram(brightProgram);
	PrintProgramLog(brightProgram);

	bikeBrightProgram = glCreateProgram();
	char bikeBrightShaderFrag[] = "Shaders/bike_bright.frag";
	int c9 = CreateShader(GL_FRAGMENT_SHADER, bikeBrightShaderFrag);
	glAttachShader(bikeBrightProgram,c9);
	char bikeBrightShaderVert[] = "Shaders/bike_bright.vert";
	int c10 = CreateShader(GL_VERTEX_SHADER, bikeBrightShaderVert);
	glAttachShader(bikeBrightProgram,c10);
	glLinkProgram(bikeBrightProgram);
	PrintProgramLog(bikeBrightProgram);


	// Set up texture to write to for rendering the scene

	glGenFramebuffers(1,&frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// generate the two textures
	glGenTextures(1,&renderNormalTexture);
	// set up first output texture
	glBindTexture(GL_TEXTURE_2D, renderNormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


	// attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderNormalTexture, 0);
	glDrawBuffers(1, colorAttachments);



	// Set up texture to write to for rendering the glowing parts of the scene

	glGenFramebuffers(1,&blurBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer);
	// generate the two textures
	glGenTextures(1,&blurTexture);
	// set up first output texture
	glBindTexture(GL_TEXTURE_2D, blurTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);
	glDrawBuffers(1, colorAttachments);


	

	// generate textures/ frame buffers for ping pong gaussian blur
	glGenFramebuffers(2, pingpongFrameBuffer);
	glGenTextures(2,pingpongTextures);

	// set up 1st frame buffer and texture for gaussian blur
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[0]);
	glBindTexture(GL_TEXTURE_2D, pingpongTextures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextures[0],0);
	glDrawBuffers(1,colorAttachments);




	// set up 2nd frame buffer and texture for gaussian blur
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[1]);
	glBindTexture(GL_TEXTURE_2D, pingpongTextures[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextures[1],0);
	glDrawBuffers(1,colorAttachments);


	// find locations of variables in programs
	horizontalLocation = glGetUniformLocation(blurProgram, "horizontal");
	bloomBoolLoc = glGetUniformLocation(blendProgram, "bloom");
	exposureLoc = glGetUniformLocation(blendProgram, "exposure");
	widthLoc = glGetUniformLocation(blurProgram, "width");
	heightLoc = glGetUniformLocation(blurProgram, "height");
}

// Should return elapsed time in seconds
double GameState::getElapsedTime() {
	Uint64 currentTime = SDL_GetPerformanceCounter();
	double deltaTime = (double)((currentTime - previousTime) * 1000 / (double)SDL_GetPerformanceFrequency());
	previousTime = currentTime;
	return deltaTime / 1000;
}


// Modified Project function from ex9
void GameState::Project( bool split )
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    split ? gluPerspective(fov*2,asp*2,1,2000) : gluPerspective(fov,asp,10,5000);
    // gluPerspective(fov,asp,10,5000);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
}

// reshape function from ex9
void GameState::reshape(int width,int height) {
	wWidth = width;
	wHeight = height;
	if( split ) {
		reshapeSplit(width, height);
		return;
	}
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project(false);
}

// reshape function from ex9 - need to call viewport in screen
void GameState::reshapeSplit(int width,int height) {
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set projection
    Project(true);
}