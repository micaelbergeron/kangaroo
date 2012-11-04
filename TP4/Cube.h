#pragma once
#include "Polyhedron.h"

class Cube : public Polyhedron
{
private:
	void buildOrthoCube()
	{
		GLfloat vertices[] =  { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
								1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
								1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
							   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
							   -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
								1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)

		GLfloat normals[]  =  { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
								1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
								0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
							   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
								0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
								0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)	

		GLubyte indices[]  = { 0, 1, 2,   2, 3, 0,      // front
							   4, 5, 6,   6, 7, 4,      // right
							   8, 9,10,  10,11, 8,      // top
							  12,13,14,  14,15,12,      // left
							  16,17,18,  18,19,16,      // bottom
							  20,21,22,  22,23,20 };    // back

		setVerticesfv(vertices, 6 * 12);
		setNormalfv(normals, 6 * 12);
		setTriangles(indices, 6 * 6);
	}

public:

	Cube(void)
		: Polyhedron()
	{
		buildOrthoCube();
	}
	
	Cube(GLfloat height, GLfloat width, GLfloat depth)
		: Polyhedron()
	{
		buildOrthoCube();
		transform.setScale(width, height, depth);
	}

	Cube(const Cube & other)
		: Polyhedron(other)
	{
	}
};

