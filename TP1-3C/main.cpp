/* cube.c        */



/* MODIFIÉ PAR D. AUDET */
/*   LE PROGRAMME A ÉTÉ SIMPLIFIÉ POUR QUE LE CUBE NE BOUGE PAS */



/* Rotating cube with color interpolation */

/* E. Angel, Interactive Computer Graphics */
/* A Top-Down Approach with OpenGL, Third Edition */
/* Addison-Wesley Longman, 2003 */

/* Demonstration of use of homogeneous coordinate 
transformations and simple data structure for representing
cube from Chapter 4 */

/* Colors are assigned to the vertices */
/* Mouse buttons control direction of rotation */

#include <stdio.h>
#include <stdlib.h>
#include "glut.h"

#define DIM 3
#define COLOR 3
#define ROTATION_ANGLE 30.0

enum ProgramState {
	Numero_3B,
	Numero_3C,
	STATE_COUNT
};

ProgramState _State = ProgramState::Numero_3B;

GLfloat vertices[][DIM] = {
	{-1.0,-1.0,-1.0}, 
	{1.0,-1.0,-1.0}, 
	{1.0,1.0,-1.0}, 
	{-1.0,1.0,-1.0}, 
	{-1.0,-1.0,1.0}, 
	{1.0,-1.0,1.0}, 
	{1.0,1.0,1.0}, 
	{-1.0,1.0,1.0}
};

GLfloat colors[][DIM] = {
	{0.5,0.5,0.5},
	{1.0,0.0,0.0},
	{1.0,1.0,0.0},
	{0.0,1.0,0.0},
	{0.0,0.0,1.0},
	{1.0,0.0,1.0}, 
	{1.0,1.0,1.0}, 
	{0.0,1.0,1.0}
};

void toggle_rotation(ProgramState next_state)
{
	if (next_state == _State) {
		return;
	}

	GLfloat vector_length = 1.0f;
	
	if (_State == ProgramState::Numero_3B)
	{
		glRotatef(ROTATION_ANGLE, vector_length, vector_length, vector_length);
	} else {
		glRotatef(-ROTATION_ANGLE, vector_length, vector_length, vector_length);
	}
}

//-- Menu implementation --//
void onMenu_Clicked(int value) {
	ProgramState next_state = ProgramState::STATE_COUNT;
	switch(value)
	{
	case 0:
		next_state = ProgramState::Numero_3B;
		toggle_rotation(next_state);
		break;
	case 1:
		next_state = ProgramState::Numero_3C;
		toggle_rotation(next_state);
		break;
	default: break;
	}
	
	if (next_state != ProgramState::STATE_COUNT) {
		_State = next_state;
		glutPostRedisplay();
	}
}

//-- End menu implementation --//

GLfloat *subPoints(GLfloat *a, GLfloat *b, int dimension)
{
	GLfloat *retPoint = (GLfloat *)malloc(dimension * sizeof(GLfloat));

	for (int i = 0; i < dimension; i++)
	{
		retPoint[i] = a[i] - b[i];
	}

	return retPoint;
}

GLfloat *addPoints(GLfloat *a, GLfloat *b, int dimension)
{
	GLfloat *retPoint = (GLfloat *)malloc(dimension * sizeof(GLfloat));

	int i = 0;
	for (; i < dimension; i++)
	{
		retPoint[i] = a[i] + b[i];
	}

	return retPoint;
}

GLfloat *divPoints(GLfloat* dividand, GLfloat divider, int dimension)
{
	GLfloat *retPoint = (GLfloat *)malloc(dimension * sizeof(GLfloat));
	
	int i = 0;
	for(; i < dimension; i++)
	{
		retPoint[i] = dividand[i] / divider;
	}

	return retPoint;
}

void polygon(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, int idx_color)
{
	/* draw a polygon via list of vertices */
	glBegin(GL_POLYGON);
		glColor3fv(colors[idx_color]);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(d);
		glVertex3fv(c);
	glEnd();
}

//-- Sierpinsky impl. --//
void sierpinski(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int rec_depth, int idx_color)
{
	// swap d -> c
	GLfloat* tmp_swap = c;
	c = d;
	d = tmp_swap;

	if (rec_depth == 0) {
		// draw the polygon
		polygon(a, b, c, d, idx_color);
		return;
	}

	const GLfloat DIVISOR = 3.0f;
	/* The final point array will be the following (in hex)
		0 1 2 3 
		4 5 6 7 
		8 9 A B
		C D E F 
	*/

	GLfloat **sierpVertices = (GLfloat **)malloc(16 * sizeof(GLfloat *));

	// Step 0, the starting points
	sierpVertices[0] = a;
	sierpVertices[3] = b;
	sierpVertices[12] = c;
	sierpVertices[15] = d;

	// Step 1, find the outer points (8)
	GLfloat *dAB = subPoints(b, a, DIM);
	GLfloat *dAC = subPoints(c, a, DIM);

	GLfloat *a_b = divPoints(dAB, DIVISOR, DIM);
	GLfloat *a_c = divPoints(dAC, DIVISOR, DIM);
	
	sierpVertices[1] = addPoints(a, a_b, DIM);
	sierpVertices[2] = subPoints(b, a_b, DIM);

	sierpVertices[4] = addPoints(a, a_c, DIM);
	sierpVertices[8] = subPoints(c, a_c, DIM);

	sierpVertices[13] = addPoints(c, a_b, DIM);
	sierpVertices[14] = subPoints(d, a_b, DIM);

	sierpVertices[7] = addPoints(b, a_c, DIM);
	sierpVertices[11] = subPoints(d, a_c, DIM);

	// Step 2, find the inner points (4)
	sierpVertices[5] = addPoints(sierpVertices[4], a_b, DIM);
	sierpVertices[6] = subPoints(sierpVertices[7], a_b, DIM);

	sierpVertices[9] = addPoints(sierpVertices[8], a_b, DIM);
	sierpVertices[10] = subPoints(sierpVertices[11], a_b, DIM);

	// Recursive calls
	int next_depth = rec_depth - 1;

	GLfloat **sv = sierpVertices; // shorcut

	sierpinski(sv[0], sv[1], sv[5], sv[4], next_depth, idx_color);
	sierpinski(sv[1], sv[2], sv[6], sv[5], next_depth, idx_color);
	sierpinski(sv[2], sv[3], sv[7], sv[6], next_depth, idx_color);

	sierpinski(sv[4], sv[5], sv[9], sv[8], next_depth, idx_color);
	sierpinski(sv[6], sv[7], sv[11], sv[10], next_depth, idx_color);

	sierpinski(sv[8], sv[9], sv[13], sv[12], next_depth, idx_color);
	sierpinski(sv[9], sv[10], sv[14], sv[13], next_depth, idx_color);
	sierpinski(sv[10], sv[11], sv[15], sv[14], next_depth, idx_color);

	// clean memory
	free(sierpVertices);
	free(dAB);
	free(dAC);
	free(a_b);
	free(a_c);
}

// call sierpinski from the vertices array;
void sierpinskiFace(int a, int b, int c, int d, int depth, int idx_color)
{
	sierpinski(vertices[a], vertices[b], vertices[c], vertices[d], depth, idx_color);	
}

void renderSierpinski(int depth)
{
	/* map vertices to faces */
	switch(_State)
	{
	case ProgramState::Numero_3B:
		sierpinskiFace(0,3,2,1, depth, COLOR);
		break;
	case ProgramState::Numero_3C:
		sierpinskiFace(0,3,2,1, depth, 0);	
		sierpinskiFace(2,3,7,6, depth, 1);
		sierpinskiFace(0,4,7,3, depth, 2);
		sierpinskiFace(1,2,6,5, depth, 3);
		sierpinskiFace(4,5,6,7, depth, 4);
		sierpinskiFace(0,1,5,4, depth, 5);
		break;
	default: break;
	}
}
//-- End sierpinsky impl. --//

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderSierpinski(4);
	glFlush();
}


void init_glutmenu()
{
	// menu
	glutCreateMenu(& onMenu_Clicked);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddMenuEntry("Voir numéro 3B", 0);
	glutAddMenuEntry("Voir numéro 3C", 1);
}

void init_glutdisplay()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("colorcube");
	glutDisplayFunc(display);
}

void init_opengl()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{ 
	glutInit(&argc, argv);
	init_glutdisplay();
	init_opengl();
	init_glutmenu();

	glutMainLoop();
}
