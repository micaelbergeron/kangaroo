#pragma once

// #include "Quaternion.h" not used anymore
#include <willperone\Math\quaternion.h>
#include <glut.h>

using namespace std;

typedef quaternion Quaternion;

class Transform
{
private:
	matrix4		m_matrix;

public:	
	Transform(void) 
	{
		m_matrix.identity();
	}

	Transform(const Transform & other)
	{
		m_matrix = other.m_matrix;
	}

	~Transform(void) {}

	void reset()
	{
		m_matrix.identity();
	}

	void translate(GLfloat x, GLfloat y, GLfloat z)
	{
		matrix4::translation(m_matrix, vector3f(x,y,z));
	}

	void rotate(Quaternion q)
	{
		m_matrix = m_matrix * matrix4((matrix3)q, vector3f(0,0,0));
	}

	void scale(GLfloat x, GLfloat y, GLfloat z)
	{
		m_matrix *= matrix4(vector3f(x, 0, 0), vector3f(0, y, 0), vector3f(0, 0, z), vector3f(0,0,0));
	}

	void apply()
	{
		glMultMatrixf(m_matrix.elem);
	}
};