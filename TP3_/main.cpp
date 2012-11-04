// Micael Bergeron - BERM09129007
// TP2: Infographie.
// But: Faire une molécule à l'aide d'openGL pour ensuite pouvoir controler
//		caméra avec la souris.

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
GLfloat LightPosition[]= { 0.0f, 0.0f, -10.0f, 1.0f };    // Light Position

float ORIGIN[] = {0, 0, 0};
float AXIS_X[] = {1, 0, 0};
float AXIS_Y[] = {0, 1, 0}; 
float AXIS_Z[] = {0, 0, 1};

Quaternion worldRot; // identity
const float MOUSE_ROT_FACTOR = 0.01f;
const float PIPE_LENGTH = 3.0f;

float ITERATION_DELAY = 500; // ms
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
		rotate(p->getOrientation().Normalize());
		gluCylinder(quadric, 0.5, 0.5, p->getLength(), 20, 20);
		glTranslatef(0, 0, -(p->getLength()) );
	}
}

void addPipeToList(int pipe_length)
{
	// calculate a random vector from 3 complete quarter rotation
	float qx = floorf(rand() % 3) * 90.0f;
	float qy = floorf(rand() % 3) * 90.0f;
	float qz = floorf(rand() % 3) * 90.0f;

	Quaternion rot = Quaternion(qx, qy, qz);
	PIPE_LIST.push_back(new Pipe(rot, pipe_length));

	cout << qx << " " << qy << " " << qz << endl;

	// register again
	glutTimerFunc(ITERATION_DELAY, addPipeToList, PIPE_LENGTH);
}

void renderMolecule(Quaternion rot, int iterations, int rot_x, int rot_y, int rot_z) 
{	
	if (iterations == 0) {		
		return;
	}
	
	float linkLength = ((float)iterations / 2);

	float linkMat[] = {0.5, 0, 0, 1};

	// glColor3f(inv_iterations, 1 - inv_iterations, inv_iterations);
	gluSphere(quadric, 0.60, 20, 20);
	glPushMatrix();	
	
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);
	glRotatef(rot_z, 0, 0, 1);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, linkMat);
	gluCylinder(quadric, 0.1, 0.1, linkLength, 20, 20);
	glTranslatef(0, 0, linkLength);

	int half_x_rot = rot_x / 2;
	int half_y_rot = rot_y / 2;
	int half_z_rot = rot_z / 2;
	renderMolecule(rot, iterations - 1, half_x_rot, half_y_rot, half_z_rot);
	glPopMatrix();

	glRotatef(-rot_x, 1, 0, 0);
	glRotatef(-rot_y, 0, 1, 0);
	glRotatef(-rot_x, 0, 0, 1);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, linkMat);
	gluCylinder(quadric, 0.1, 0.1, linkLength, 20, 20);
	glTranslatef(0, 0, linkLength);
		
	renderMolecule(rot, iterations - 1, -half_x_rot, -half_y_rot, -half_z_rot);
}

void renderLightning() {
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void apply_world_rotation ()
{	
	glPushMatrix ();
	glTranslatef (-8,-8, 0);
	glScalef(1.5f, 1.5f, 1.5f);
	
	rotate(worldRot);

	glLineWidth (2.0);

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

	glPopMatrix ();	

	rotate(worldRot);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	apply_world_rotation();
	
	renderLightning();

	//
	
	gluLookAt(0, 0, 0, 0, 0, -5, 0, 2, 0);
		
	rotate(Quaternion(-90, AXIS_X));
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
	
	glOrtho(-10.0, 10.0, -10.0, 10.0, -20, 20.0);
	
	//glTranslatef(0, 0, -10.0);
	//gluPerspective(10, 1.0, 1.0, 10);

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
	srand(0);
	Quaternion quat = Quaternion(1, 1, 1, 1);

	std::cout << quat.Normalize() << std::endl;
	std::cout << quat + quat << std::endl;
	std::cout << quat.GetMagnitude() << std::endl;
	std::cout << quat.Conjugate() << std::endl;
	std::cout << quat.Normalize() * quat.Normalize().Conjugate() << std::endl;

	GLfloat axis[] = {0,0,1};
	GLfloat v[] = {1,2,0};
	
	std::cout << quat << " rotated = [" << v[0] << ", " << v[1] <<", " << v[2] << "]";

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