#include "Model.h"

Model::Model(char * filename)
{
	vertices = NULL;
	uv = NULL;
	normals = NULL;
	faces = NULL;
	num_vertices = 0;
	num_faces = 0;
	num_uv = 0;
	num_normal = 0;
	texture = -1;
	child = NULL;
	LoadModel(filename, &vertices, &num_vertices, &uv, &num_uv, &normals, &num_normal, &faces, &num_faces);
	
}

Model::Model(char * filename, char * texturename)
{
	vertices = NULL;
	uv = NULL;
	normals = NULL;
	faces = NULL;
	num_vertices = 0;
	num_faces = 0;
	num_uv = 0;
	num_normal = 0;
	child = NULL;
	texture = LoadTexBMP(texturename);
	LoadModel(filename, &vertices, &num_vertices, &uv, &num_uv, &normals, &num_normal, &faces, &num_faces);
	
}

Model::~Model()
{
	delete vertices;
	delete uv;
	delete normals;
	delete faces;
	if(child != NULL)
		delete child;
}


// maybe change the calculations to normalize these later. might not be needed
// set the color before displaying the model
void Model::display(Vector * v, Vector * direction, Vector * up, double s, bool texOn = true)
{
	double dx = direction->getX();
	double dy = direction->getY();
	double dz = direction->getZ();

	double ux = up->getX();
	double uy = up->getY();
	double uz = up->getZ();

	//  Unit vector in direction of flght
	double D0 = sqrt(dx*dx+dy*dy+dz*dz);
	double X0 = dx/D0;
	double Y0 = dy/D0;
	double Z0 = dz/D0;
	//  Unit vector in "up" direction
	double D1 = sqrt(ux*ux+uy*uy+uz*uz);
	double X1 = ux/D1;
	double Y1 = uy/D1;
	double Z1 = uz/D1;
	//  Cross product gives the third vector
	double X2 = Y0*Z1-Y1*Z0;
	double Y2 = Z0*X1-Z1*X0;
	double Z2 = X0*Y1-X1*Y0;
	//  Rotation matrix
	double mat[16];
	mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
	mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
	mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
	mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

	//  Save current transforms
	glPushMatrix();
	

	glTranslated(v->getX(),v->getY(),v->getZ());
	glMultMatrixd(mat);
	glScaled(s,s,s);

	displayBasic(texOn);
	glPopMatrix();
}

void Model::append(Model * m) // add a model to the linked list
{
	if(child == NULL)
		child = m;
	else child->append(m);
}

void Model::append(char * name, char * texname)
{
	if(child == NULL)
		child = new Model(name,texname);
	else child->append(name,texname);
}

void Model::append(char * name)
{
	if(child == NULL)
		child = new Model(name);
	else child->append(name);
}


void Model::displayBasic(bool texOn)
{
	if(texOn && texture != -1)
		glBindTexture(GL_TEXTURE_2D,texture);
	else glBindTexture(GL_TEXTURE_2D, 0); // need to place call to glTexEnvi somewhere in the setup for opengl

	if(texOn)
	{	
		glEnable(GL_TEXTURE_2D); 
	}
		glBegin(GL_TRIANGLES);
	for(int i = 0; i < num_faces*9; i+=3)
	{ // i * 3 is due to num_faces*3, can be switched to i * 9 if num_faces is not multiplied by 3
		// vertex one of face
		int uv_index = (faces[i + 1]-1) * 2;
		glTexCoord2f(uv[uv_index],uv[uv_index + 1]);
		int normal_index = (faces[i + 2]-1) * 3;
		glNormal3f(normals[normal_index],normals[normal_index+1],normals[normal_index+2]);
		int vertex_index = (faces[i]-1) * 3;
		glVertex3d(vertices[vertex_index],vertices[vertex_index+1],vertices[vertex_index+2]);
	}
	glEnd();

	if(texOn)
	{
		glDisable(GL_TEXTURE_2D);
	}

	if(child != NULL)
	{
		child->displayBasic(texOn);
	}
}

Model * Model::getNext()
{
	return child;
}

void Model::displayRainbow(Vector * v, Vector * direction, Vector * up, double s, bool texOn = true)
{
	double dx = direction->getX();
	double dy = direction->getY();
	double dz = direction->getZ();

	double ux = up->getX();
	double uy = up->getY();
	double uz = up->getZ();

	//  Unit vector in direction of flght
	double D0 = sqrt(dx*dx+dy*dy+dz*dz);
	double X0 = dx/D0;
	double Y0 = dy/D0;
	double Z0 = dz/D0;
	//  Unit vector in "up" direction
	double D1 = sqrt(ux*ux+uy*uy+uz*uz);
	double X1 = ux/D1;
	double Y1 = uy/D1;
	double Z1 = uz/D1;
	//  Cross product gives the third vector
	double X2 = Y0*Z1-Y1*Z0;
	double Y2 = Z0*X1-Z1*X0;
	double Z2 = X0*Y1-X1*Y0;
	//  Rotation matrix
	double mat[16];
	mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
	mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
	mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
	mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

	//  Save current transforms
	glPushMatrix();

	glTranslated(v->getX(),v->getY(),v->getZ());
	glMultMatrixd(mat);
	glScaled(s,s,s);


	displayBasicRainbow(texOn);
	glPopMatrix();
}

void Model::displayBasicRainbow(bool texOn)
{
	//  Offset, scale and rotate
	if(texOn && texture != -1)
		glBindTexture(GL_TEXTURE_2D,texture);
	else glBindTexture(GL_TEXTURE_2D, 0);

	
	if(texOn)
	{
		glEnable(GL_TEXTURE_2D); // need to place call to glTexEnvi somewhere in the setup for opengl
	}

	float time = SDL_GetPerformanceCounter()/1000000000.0;
	
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < num_faces*9; i+=3)
	{ // i * 3 is due to num_faces*3, can be switched to i * 9 if num_faces is not multiplied by 3
		// vertex one of face
		int uv_index = (faces[i + 1]-1) * 2;
		glTexCoord2f(uv[uv_index],uv[uv_index + 1]);
		int normal_index = (faces[i + 2]-1) * 3;
		glNormal3f(normals[normal_index],normals[normal_index+1],normals[normal_index+2]);
		int vertex_index = (faces[i]-1) * 3;

		
		setRainbowColor(vertices[vertex_index], time);
		glVertex3d(vertices[vertex_index],vertices[vertex_index+1],vertices[vertex_index+2]);
	}
	glEnd();
	if(texOn)
		glDisable(GL_TEXTURE_2D);
	if(child != NULL)
		child->displayBasicRainbow(texOn);
}

void Model::setRainbowColor(float x, float time)
{
	double frequency = 2;
	double rawI = frequency * (time + x);
	int colorI = (int)rawI % 7;
	double weight = rawI - (int)rawI;
	double red   = rainbowColors[(colorI+1)%7][0] * weight + rainbowColors[colorI][0] * (1-weight);
	double green = rainbowColors[(colorI+1)%7][1] * weight + rainbowColors[colorI][1] * (1-weight);
	double blue  = rainbowColors[(colorI+1)%7][2] * weight + rainbowColors[colorI][2] * (1-weight);

	// double frequency = .2;
	// double speed = 20;
	// double red   = sin(frequency * (x + time*speed) + 0) * .5 + .5;
	// double green = sin(frequency * (x + time*speed) + 2) * .5 + .5;
	// double blue  = sin(frequency * (x + time*speed) + 4) * .5 + .5;

	glColor3d(red, green, blue);
}
