#include "Button.h"

Button::Button(){}

Button::Button( double _cx, double _cy, double w, double h, unsigned int tex, double tl, double tr, double tt, double tb, Vector c ) {
	cx = _cx;
	cy = _cy;
	width = w;
	height = h;
	texture = tex;
	color = Vector(c.getX(), c.getY(), c.getZ());
	alpha = .9;

	lx = cx - width/2.0;
	rx = cx + width/2.0;
	ty = cy - height/2.0;
	by = cy + height/2.0;

	texl = tl;
	texr = tr;
	text = tt;
	texb = tb;
}

void Button::display() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	// glColor4d(color.getX(), color.getY(), color.getZ(), alpha);
	glColor3d(color.getX(), color.getY(), color.getZ());
	glBegin( GL_QUADS );
		glTexCoord2d(texl,texb); glVertex2d(lx,by);
		glTexCoord2d(texl,text); glVertex2d(lx,ty);
		glTexCoord2d(texr,text); glVertex2d(rx,ty);
		glTexCoord2d(texr,texb); glVertex2d(rx,by);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

bool Button::testClicked( double mx, double my, int wWidth, int wHeight ) {
	double testx = (mx / (double)wHeight) - (((wWidth / (double)wHeight) - 1) / 2);
	double testy = 1-(my / (double)wHeight);
	return testx >= lx && testx <= rx && testy <= by && testy >= ty;
}

void Button::setText( double newVal ) { text = newVal; }
void Button::setTexb( double newVal ) { texb = newVal; }