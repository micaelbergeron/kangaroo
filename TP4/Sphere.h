#pragma once
#include "SceneObject.h"

class Sphere : public SceneObject
{
private:
	GLfloat m_radius;
	GLint	m_resolution;
	GLUquadricObj * m_quadric; // does not own

public:
	Sphere(GLUquadricObj * quadric) 
		: SceneObject()
	{
		m_quadric = quadric;
		m_radius = 1.0f;
		m_resolution = 10;
	}

	Sphere(GLUquadricObj * quadric, GLfloat radius, GLint resolution) 
		: SceneObject()
	{
		m_quadric = quadric;
		m_radius = radius;
		m_resolution = resolution;
	}

	~Sphere(void) {}

	void render()
	{
		gluSphere(m_quadric, m_radius, m_resolution, m_resolution);
	}
};

