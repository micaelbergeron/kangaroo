// Micael Bergeron - BERM09129007
// TP4: Infographie.

#include <cstdlib>
#include "glut.h"
#include "Polyhedron.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"

using namespace std;

GLUquadricObj * quadric;

GLfloat LightAmbient[]  =  { 0.5f, 0.5f, 0.5f, 1.0f };    // Ambient Light Values
GLfloat LightDiffuse[]  =  { 1.0f, 1.0f, 1.0f, 1.0f };    // Diffuse Light Values
GLfloat LightPosition[] = { 3.0f, 3.0f, 8.0f, 1.0f };     //	     Light Position

float ORIGIN[] = {0, 0, 0};
float AXIS_X[] = {1, 0, 0};
float AXIS_Y[] = {0, 1, 0}; 
float AXIS_Z[] = {0, 0, 1};

Transform main_camera;
Transform worldOrientation;

// Matrices for views
float MAT_PROJ_ORTHO[16];
float MAT_PROJ_PERSPECTIVE[16];

Polyhedron* cube;
Polyhedron* bigcube;

SceneObject * vessel;

const float MOUSE_ROT_FACTOR = 0.1f;
const float PIPE_LENGTH = 2.0f;

float ITERATION_DELAY = 250.0; // ms
int PIPE_BY_ADD = 1;

void renderLightning() 
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);	
	glEnable(GL_LIGHT0);
}

void applyWorldTransform ()
{		
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(MAT_PROJ_ORTHO);
	// We are now orthogonal;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	glScalef(1.5f, 1.5f, 1.5f);
	glTranslatef(-8, -8, 0);

	glLineWidth (2.0);

	// draw the indicator
	glBegin (GL_LINES);
	glColor3f (1,0,0); // X axis is red.
	glVertex3fv (ORIGIN);
	glVertex3fv (AXIS_X);
	glColor3f (0,1,0); // Y axis is green.
	glVertex3fv (ORIGIN);
	glVertex3fv (AXIS_Y);
	glColor3f (0,0,1); // z axis is blue.
	glVertex3fv (ORIGIN);
	glVertex3fv (AXIS_Z);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(MAT_PROJ_PERSPECTIVE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	worldOrientation.apply();
	
	glEnable(GL_LIGHTING);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	renderLightning();	
	applyWorldTransform();	

	//glTranslatef(0, 0, -10);
	
	glPushMatrix();

	vessel->draw(true);

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

/*

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	Cube * c = new Cube();
	c->transform.translate(0,0,0);
	c->draw(true);
	delete c;
	
	

	gluSphere(quadric, 10.0f, 10, 10);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

*/
void idle() {
	const float SMOOTHING = 0.01f;
	vessel->transform.rotate(Quaternion(0, SMOOTHING, 0));
}

void init_glutDisplay() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutCreateWindow("Starship Micael X III");
	glutDisplayFunc(display);
}

void init_opengl() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// calculate the worldOrtho
	glOrtho(-10.0, 10.0, -10, 10, -5, 5); 
	glGetFloatv(GL_PROJECTION_MATRIX, MAT_PROJ_ORTHO);
	
	glLoadIdentity();
	gluPerspective(90, 1.0, 0.1, 500);

	glGetFloatv(GL_PROJECTION_MATRIX, MAT_PROJ_PERSPECTIVE);
	glMatrixMode(GL_MODELVIEW);
	
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
}

int mouse_x;
int mouse_y;
void mouseMove(int x, int y) {
	float delta_x = x - mouse_x;
	float delta_y = y - mouse_y;

	// compute a vector from current deltas
	vessel->transform.rotate(Quaternion(delta_x * MOUSE_ROT_FACTOR, delta_y * MOUSE_ROT_FACTOR, 0));

	mouse_x = x;
	mouse_y = y;
}

SceneObject * buildVessel()
{
	Sphere * base = new Sphere(quadric, 1.25f, 20);

	Sphere * cockpit = new Sphere(quadric, 0.75f, 10);
	cockpit->transform.translate(-0.75f, -0.25f, 0);
	cockpit->attach(base, true);
	
	// wing attachement
	Cylinder * l_wing = new Cylinder(quadric, 0.75f, 0.2f, 3, 15);

	Cylinder * l_gattling = new Cylinder(quadric, 0.4f, 0.25f, 2, 10);	
	l_gattling->transform.translate(0.7f, 0, 1.8f);
	l_gattling->transform.rotate(Quaternion(90,0,0));
	l_gattling->attach(l_wing, true);

	// solar panel
	Cube * l_wing_panel = new Cube(1.0f, 2.0f, 0.1f);
	l_wing_panel->transform.translate(0, 0, 3);

	Cube * l_wing_center = new Cube(0.40f, 0.75f, 2.0f);
	l_wing_center->transform.translate(0, 0, 1.1f);
	l_wing_center->attach(l_wing_panel, true);

	Cube * l_wing_panel_bot = new Cube();

	float rot_flap = 40;
	float rot_flap_rad = rot_flap * PI / 180;
	float x_smooth = 0.4;
	float y_smooth = 0.8;

	l_wing_panel_bot->transform.rotate(Quaternion(0, 0, rot_flap));
	l_wing_panel_bot->transform.translate(0, 1 + sinf(rot_flap_rad), 3 - cosf(rot_flap_rad));
	l_wing_panel_bot->transform.scale(2.0f, 1.0f, 0.1f);

	Cube * l_wing_panel_top = new Cube();
	l_wing_panel_top->transform.rotate(Quaternion(0, 0, -rot_flap));
	l_wing_panel_top->transform.translate(0, -1 + sinf(-rot_flap_rad), 3 - cosf(-rot_flap_rad));
	l_wing_panel_top->transform.scale(2.0f, 1.0f, 0.1f);

	l_wing_panel->attach(l_wing, true);
	l_wing_panel_top->attach(l_wing, true);
	l_wing_panel_bot->attach(l_wing, true);

	Cylinder * l_wing_2 = new Cylinder(*l_wing);
	l_wing_2->transform.scale(2,2,2);

	Cylinder * r_wing = new Cylinder(*l_wing);
	r_wing->transform.scale(1,-1,-1);

	Cylinder * r_wing_2 = new Cylinder(*r_wing);
	r_wing_2->transform.scale(2,2,2);

	
	l_wing->attach(base, true);
	r_wing->attach(base, true);	

	base->transform.translate(0, 0, 0);

	return base;
}

Polyhedron* buildCube()
{
	Polyhedron* c = new Polyhedron();
	return c;
}

void mouseClick(int button, int state, int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

int main(int argc, char** argv) {
	srand(5);

	glutInit(& argc, argv);
	init_glutDisplay();
	init_opengl();

	glutIdleFunc(idle);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);

	main_camera.translate(0, 0, 0);
	main_camera.rotate(Quaternion(1, 0, 0, 0));
	
	worldOrientation.translate(0, 0, -10);
	worldOrientation.rotate(Quaternion(-90,0,0));

	vessel = buildVessel();

	glutMainLoop();
	return 0;
}