#pragma once
#include "Quaternion.h"
#include "glut.h"

using namespace std;
using namespace quaternion;

class Transform
{
private:
	GLfloat		m_position[3];
	GLfloat		m_scale[3];
	Quaternion	m_rotation;

public:	
	typedef void (*FUNC_ROTATE)(quaternion::Quaternion q);
	static FUNC_ROTATE rotationFunc;

	Transform(void)
	{
		m_position[0] = m_position[1] = m_position[2] = 0;
		m_scale[0] = m_scale[1] = m_scale[2] = 1;
	}

	Transform(const Transform & other)
	{
		memcpy(m_position, other.m_position, 3);
		memcpy(m_scale, other.m_scale, 3);
		m_rotation = other.m_rotation;
	}

	Transform(GLfloat* pos, Quaternion rotation, GLfloat* scale = NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			m_position[i] = pos[i];
			m_rotation = rotation;
			if (scale != NULL)
			{
				m_scale[i] = scale[i];
			}
		}
	}

	~Transform(void) {}

	void setPosition(GLfloat x, GLfloat y, GLfloat z)
	{
		m_position[0] = x;
		m_position[1] = y;
		m_position[2] = z;
	}

	void setRotation(quaternion::Quaternion q)
	{
		m_rotation = q;
	}

	void setScale(GLfloat x, GLfloat y, GLfloat z)
	{
		m_scale[0] = x;
		m_scale[1] = y;
		m_scale[2] = z;
	}

	void apply()
	{
		glTranslatef(m_position[0], m_position[1], m_position[2]);
		if (rotationFunc != NULL)
		{
			rotationFunc(m_rotation);
		}
		glScalef(m_scale[0], m_scale[1], m_scale[2]);
	}
};

Transform::FUNC_ROTATE Transform::rotationFunc = NULL;
