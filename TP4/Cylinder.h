#pragma once
#include "SceneObject.h"

class Cylinder : public SceneObject
{
private:
	GLfloat m_topRadius;
	GLfloat m_baseRadius;
	GLfloat m_height;
	GLint	m_resolution;
	GLUquadricObj * m_quadric; // does not own
	
public:
	bool has_caps;

	Cylinder(void)
		: SceneObject()
	{
	}

	Cylinder(GLUquadricObj * quadric, GLfloat baseRadius, GLfloat topRadius, GLfloat height, GLint resolution)
		: SceneObject()
	{
		m_quadric = quadric;
		m_baseRadius = baseRadius;
		m_topRadius = topRadius;
		m_height = height;
		m_resolution = resolution;
	}

	~Cylinder(void)
	{
	}

	void render()
	{
		gluCylinder(m_quadric, m_baseRadius, m_topRadius, m_height, m_resolution, m_resolution);
		if (has_caps)
		{
			glPushMatrix();			
			gluDisk(m_quadric, 0, m_baseRadius, m_resolution, m_resolution);
			glTranslatef(0, 0, m_height);
			gluDisk(m_quadric, 0, m_topRadius, m_resolution, m_resolution);
			glPopMatrix();
		}
	}
};

