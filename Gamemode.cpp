class Gamemode{
	void key(/* TODO*/) = 0; // need to format this specifically for SDL instead of GLUT
	void special(int key, int x, int y) = 0; /*NEEDED in GLUT, maybe different in other system */
	void mouse(/*TODO*/) = 0;
	void display() = 0;
}