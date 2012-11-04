// Micael Bergeron - BERM09129007
// TP4: Infographie.

#include <cstdlib>
#include "glut.h"
#include "Pipe.h"
#include <vector>

using namespace quaternion;
using namespace std;

typedef Quaternion GLQuaternion;
typedef vector<Pipe *> PipeList;

GLUquadricObj * quadric;

GLfloat LightAmbient[]=  { 0.5f, 0.5f, 0.5f, 1.0f };    // Ambient Light Values
GLfloat LightDiffuse[]=  { 1.0f, 1.0f, 1.0f, 1.0f };    // Diffuse Light Values
GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };   //		   Light Position

float ORIGIN[] = {0, 0, 0};
float AXIS_X[] = {1, 0, 0};
float AXIS_Y[] = {0, 1, 0}; 
float AXIS_Z[] = {0, 0, 1};

float worldPos[] = {-4.0f, 2.0f, -20.0f};
Quaternion worldRot; // identity
const float MOUSE_ROT_FACTOR = 0;
const float PIPE_LENGTH = 2.0f;

float ITERATION_DELAY = 250.0; // ms
int PIPE_BY_ADD = 1;
PipeList PIPE_LIST;

void rotate(quaternion::Quaternion rot) {
	float* rotMatrix = rot.toRotationMatrix();
	glMultMatrixf(rotMatrix);
}

void renderPipeList()
{	
	float inv_iterations = 1.0f / (float)PIPE_LIST.size();	
	float pipeMat[] = { 0.1, 0.5, inv_iterations, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pipeMat);

	PipeList::const_iterator pipe_it;
	for (pipe_it = PIPE_LIST.begin(); pipe_it != PIPE_LIST.end(); pipe_it++)
	{
		Pipe * p = *pipe_it;
		rotate(p->getOrientation());
		gluCylinder(quadric, 0.5, 0.5, p->getLength(), 20, 20);
		gluSphere(quadric, 0.5, 20, 20);
		glTranslatef(0, 0, p->getLength());
	}
}

Quaternion POSSIBLE_ROTATIONS[] = {
	Quaternion(90, AXIS_X),
	Quaternion(90, AXIS_Y),
	Quaternion(90, AXIS_Z)
};

void addPipeToList(int pipe_length)
{
	// calculate a random vector from 3 complete quarter rotation
	for (int i = 0; i < PIPE_BY_ADD; i++)
	{
		int rotationIdx = rand() % 3;
		PIPE_LIST.push_back(new Pipe(POSSIBLE_ROTATIONS[rotationIdx], pipe_length));
	}

	// register again
	glutTimerFunc(ITERATION_DELAY, addPipeToList, PIPE_LENGTH);
}

void renderLightning() {
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

float worldOrtho[16];
void applyWorldTransform ()
{	
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
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	applyWorldTransform();	
	renderLightning();	

	//gluLookAt(0, 0, 0, 0, 0, -5, 0, 2, 0);		
	//rotate(Quaternion(-90, AXIS_X));
	glPushMatrix();
	renderPipeList();
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
	glutTimerFunc(ITERATION_DELAY, addPipeToList, PIPE_LENGTH);

	glutMainLoop();
	return 0;
}