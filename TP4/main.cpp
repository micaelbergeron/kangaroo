// Micael Bergeron - BERM09129007
// TP4: Infographie.

#include <cstdlib>
#include "glut.h"
#include "Polyhedron.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"

using namespace quaternion;
using namespace std;

typedef Quaternion GLQuaternion;

GLUquadricObj * quadric;

GLfloat LightAmbient[]  =  { 0.5f, 0.5f, 0.5f, 1.0f };    // Ambient Light Values
GLfloat LightDiffuse[]  =  { 1.0f, 1.0f, 1.0f, 1.0f };    // Diffuse Light Values
GLfloat LightPosition[] = { 3.0f, 3.0f, 8.0f, 1.0f };     //	     Light Position

float ORIGIN[] = {0, 0, 0};
float AXIS_X[] = {1, 0, 0};
float AXIS_Y[] = {0, 1, 0}; 
float AXIS_Z[] = {0, 0, 1};

float worldPos[] = {-4.0f, 2.0f, -20.0f};
Quaternion worldRot; // identity
Polyhedron* cube;
Polyhedron* bigcube;

SceneObject * vessel;

const float MOUSE_ROT_FACTOR = 0.02;
const float PIPE_LENGTH = 2.0f;

float ITERATION_DELAY = 250.0; // ms
int PIPE_BY_ADD = 1;

void rotate(Quaternion rot) {
	rot.Normalize();
	float* rotMatrix = rot.toRotationMatrix();
	glMultMatrixf(rotMatrix);
}

void renderLightning() 
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);	
	glEnable(GL_LIGHT0);
}

float worldOrtho[16];
void applyWorldTransform ()
{		
	glDisable(GL_LIGHTING);	

	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadMatrixf(worldOrtho);
	// We are now orthogonal;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef (-8,-8, 0);

	glScalef(1.5f, 1.5f, 1.5f);
	rotate(worldRot);
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
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix ();	

	glTranslatef(worldPos[0], worldPos[1], worldPos[2]);
	rotate(worldRot);	
	
	glEnable(GL_LIGHTING);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	renderLightning();	
	applyWorldTransform();	

	//gluLookAt(0, 0, 0, 0, 0, -5, 0, 2, 0);		
	//rotate(Quaternion(-90, AXIS_X));
	glPushMatrix();

	vessel->draw(true);

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void idle() {
	const float SMOOTHING = 0.05f;
	// worldRot = worldRot * Quaternion(SMOOTHING, 0, SMOOTHING);
}

void init_glutDisplay() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutCreateWindow("Molecule");
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
	glGetFloatv(GL_PROJECTION_MATRIX, worldOrtho);
	
	glLoadIdentity();
	gluPerspective(90, 1.0, 0.1, 500);

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
	worldRot = worldRot * Quaternion(-delta_y * MOUSE_ROT_FACTOR, -delta_x * MOUSE_ROT_FACTOR, 0);

	mouse_x = x;
	mouse_y = y;
}

SceneObject * buildVessel()
{
	Sphere * base = new Sphere(quadric, 1.0f, 20);

	// wing attachement
	Cylinder * l_wing = new Cylinder(quadric, 0.75f, 0.2f, 3, 15);

	// solar panel
	Cube * l_wing_panel = new Cube(1.0f, 2.0f, 0.1f);
	Cube * l_wing_panel_top = new Cube(*l_wing_panel);
	Cube * l_wing_panel_bot = new Cube(*l_wing_panel);

	l_wing_panel_top->attach(l_wing_panel, true);
	l_wing_panel_bot->attach(l_wing_panel, true);

	l_wing_panel_bot->transform.setScale(1, 1, 1);
	l_wing_panel_bot->transform.setRotation(Quaternion(30, 0, 0));
	l_wing_panel_bot->transform.setPosition(0, 2.3, 0);
	
	l_wing_panel_top->transform.setScale(1, 1, 1);
	l_wing_panel_top->transform.setPosition(0, -2.3, 0);

	l_wing_panel->attach(l_wing, true);
	l_wing_panel->transform.setPosition(0, 0, 3);
	l_wing_panel->transform.setRotation(Quaternion(0, 0, 90));

	Cylinder * r_wing = new Cylinder(*l_wing);

	r_wing->transform.setScale(-1, -1, -1);
	
	l_wing->attach(base, true);
	r_wing->attach(base, true);	

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

	Transform::rotationFunc = &rotate;

	cube = buildCube();
	bigcube = buildCube();
	cube->attach(bigcube, true);
		
	bigcube->transform.setRotation(Quaternion(0, 0, 45));
	cube->transform.setPosition(0, 5, 0);
	cube->transform.setScale(1, 1, 0.1);

	Polyhedron *cube2 = new Polyhedron(*cube);
	cube2->transform.setPosition(0, -3, 0);
	cube2->transform.setScale(2, 0.1, 0.5);
	cube2->attach(bigcube, true);

	Sphere * s = new Sphere(quadric, 1.0f, 10);
	s->transform.setPosition(0, 3, 0);
	s->attach(bigcube, false);

	vessel = buildVessel();

	glutMainLoop();
	return 0;
}