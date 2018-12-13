#include "Explosion.h"

Explosion::Explosion()
{

	calcProgram = glCreateProgram();
	char shader_name[] = "Shaders/particleCalculate.frag";
	int c = CreateShader(GL_FRAGMENT_SHADER, shader_name);
	glAttachShader(calcProgram,c);
	char shader_name2[] = "Shaders/particleCalculate.vert";
	int c2 = CreateShader(GL_VERTEX_SHADER, shader_name2);
	glAttachShader(calcProgram,c2);
	glLinkProgram(calcProgram);
	PrintProgramLog(calcProgram);

	displayProgram = glCreateProgram();
	char shader_name3[] = "Shaders/particleDisplay.frag";
	int c3 = CreateShader(GL_FRAGMENT_SHADER, shader_name3);
	glAttachShader(displayProgram,c3);
	char shader_name4[] = "Shaders/particleDisplay.vert";
	int c4 = CreateShader(GL_VERTEX_SHADER, shader_name4);
	glAttachShader(displayProgram,c4);
	glLinkProgram(displayProgram);
	PrintProgramLog(displayProgram);


	// generate textures/ frame buffers for ping pong gaussian blur
	glGenFramebuffers(2, pingpongFrameBuffer);
	glGenTextures(2,pingpongTextures);

	// set up 1st frame buffer and texture for gaussian blur
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[0]);
	glBindTexture(GL_TEXTURE_2D, pingpongTextures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, PARTICLE_TEXTURE_SIZE, PARTICLE_TEXTURE_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, PARTICLE_TEXTURE_SIZE, PARTICLE_TEXTURE_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextures[1],0);
	glDrawBuffers(1,colorAttachments);

	init = true;
	activeCalcTexture = 0;

	initLocation = glGetUniformLocation(calcProgram, "init");
	timeLocation = glGetUniformLocation(calcProgram, "dTime");
	rainbowLocation = glGetUniformLocation(displayProgram, "rainbow");

}

Explosion::~Explosion(){}


void Explosion::calculate()
{
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	float currentTime = SDL_GetTicks()/1000.0;
	glDisable( GL_DEPTH_TEST );
	glUseProgram(calcProgram);
	glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[activeCalcTexture]); 
    glActiveTexture(GL_TEXTURE0);
    glUniform1ui(initLocation, init);
    glUniform1f(timeLocation, currentTime - previousTime);



    glBindTexture(GL_TEXTURE_2D, pingpongTextures[!activeCalcTexture]); 
    render2DScreen();
    glBindTexture(GL_TEXTURE_2D, 0); 
    
    if (init)
        init = false;
    activeCalcTexture = !activeCalcTexture;
	

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	previousTime = currentTime;
}

void Explosion::render2DScreen()
{
	// switch to 2D displaying
	int vPort[4];
	glGetIntegerv(GL_VIEWPORT, vPort);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
   	glLoadIdentity();

   //display texture
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0,0);
	glVertex2d(0,0);
	glTexCoord2f(1,0);
	glVertex2d(PARTICLE_TEXTURE_SIZE,0);
	glTexCoord2f(1,1);
	glVertex2d(PARTICLE_TEXTURE_SIZE,PARTICLE_TEXTURE_SIZE);


	glTexCoord2f(1,1);
	glVertex2d(PARTICLE_TEXTURE_SIZE,PARTICLE_TEXTURE_SIZE);
	glTexCoord2f(0,1);
	glVertex2d(0,PARTICLE_TEXTURE_SIZE);
	glTexCoord2f(0,0);
	glVertex2d(0,0);
	glEnd();

	
	//go back to 3d
	glMatrixMode(GL_PROJECTION);
   	glPopMatrix();   
   	glMatrixMode(GL_MODELVIEW);
   	glPopMatrix();
}

void Explosion::display()
{
	if(!init)
	{	
		glUseProgram(displayProgram);
		//glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);//:GL_MODULATE);
		
		glBindTexture(GL_TEXTURE_2D, pingpongTextures[!activeCalcTexture]);
		glUniform1ui(rainbowLocation, !rainbow);
		
		//glEnable(GL_TEXTURE_2D);
		
		glBegin(GL_POINTS);
		for(int i = 0; i < 50000; i++)
		{
			glVertex4d(0,0,0,i);
		}
		//glDisable(GL_TEXTURE_2D);
	
		//glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
}



void Explosion::reset()
{
	init = true;
}

void Explosion::setRainbow(bool r)
{
	rainbow = r;
}
