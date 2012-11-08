#pragma once
#include "glut.h"

class Material
{
public:
	GLfloat m_diffuse[3];
	GLfloat m_ambient[3];
	GLfloat m_specular[3];
	GLint m_shininess;

	Material(void)
	{
		GLfloat def_ambient = 0.5f;
		GLfloat def_diffuse = 1.0f;
		GLfloat def_specular = 0.2f;

		for (int i = 0; i < 3; i++)
		{
			m_diffuse[i] = def_diffuse;
			m_ambient[i] = def_ambient;
			m_specular[i] = def_specular;
		}
		
		m_shininess = 1;
	}

	void apply()
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
		glMateriali(GL_FRONT, GL_SHININESS, m_shininess);
	}

	void setDiffuse(GLfloat * diffuse)
	{
		for (int i = 0; i < 3; i++)
		{
			m_diffuse[i] = diffuse[i];
		}
	}

	void setAmbient(GLfloat* ambient)
	{
		for (int i = 0; i < 3; i++)
		{
			m_ambient[i] = ambient[i];
		}
	}

	void setSpecular(GLfloat* specular)
	{
		for (int i = 0; i < 3; i++)
		{
			m_specular[i] = specular[i];
		}
	}

	void setShininess(GLint shininess)
	{
		m_shininess = shininess;
	}
};


