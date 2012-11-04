#pragma once

#include "SceneObject.h"

using namespace std;
using namespace quaternion;

class Polyhedron : public SceneObject
{
private :
	vector<GLfloat> m_vertices;
	vector<GLfloat> m_normals;
	vector<GLubyte> m_faces;	// index representation, triangle (3 vertex)

public:	
	typedef vector<GLfloat*>::iterator	vertex_it;

	Polyhedron(void) 
		: SceneObject()
	{
	}

	Polyhedron(const Polyhedron & other) 
		: SceneObject(other)
	{
		m_vertices = other.m_vertices;
		m_faces = other.m_faces;
		m_normals = other.m_normals;
		
		transform = other.transform;
	}

	~Polyhedron(void)
	{
	}

	void setVerticesfv(GLfloat* vertices, int n)
	{
		m_vertices.clear();
		for(int i = 0; i < n; i ++)
		{
			m_vertices.push_back(vertices[i]);
		}
	}

	void addVertexfv(GLfloat* point)
	{
		for (int i = 0; i < 3; i++)
		{			
			m_vertices.push_back(point[i]);
		}
	}

	void setNormalfv(GLfloat* normals, int n)
	{
		m_normals.clear();
		for (int i = 0; i < n; i++)
		{
			m_normals.push_back(normals[i]);
		}
	}

	void addNormalfv(GLfloat* normal)
	{
		for (int i = 0; i < 3; i++)
		{
			m_normals.push_back(normal[i]);
		}
	}

	// The bread and butter method, everything must be composed with triangles so its
	// easily rendered by OpenGL. Used to populate arrays that will be used for rendering.
	void addTriangle(GLubyte idx1, GLubyte idx2, GLubyte idx3)
	{
		m_faces.push_back(idx1);
		m_faces.push_back(idx2);
		m_faces.push_back(idx3);
	}

	void setTriangles(GLubyte* triangles, int n)
	{
		m_faces.clear();
		for (int i = 0; i < n; i++)
		{
			m_faces.push_back(triangles[i]);
		} 
	}

	void render()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &m_vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &m_normals[0]);	

		glDrawElements(GL_TRIANGLES, m_faces.size(), GL_UNSIGNED_BYTE, &m_faces[0]);

		glEnableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

};