#pragma once
#include "Transform.h"
#include "Material.h"
#include <assert.h>
#include <list>
#include <vector>

class SceneObject
{
private:
	bool m_hidden;
	bool m_ownChilds;

	Material * m_material;

	list<SceneObject*>	m_childs;
	list<bool>			m_ownedChilds;	

public:
	typedef list<SceneObject*>::iterator child_it;
	typedef list<int>::iterator			 owned_it;

	Transform transform;

	SceneObject(void)
	{	
		m_material = NULL;
		m_hidden = false;
		m_ownChilds = false;
	}

	SceneObject(const SceneObject & other)
	{
		m_hidden = other.m_hidden;
		m_ownChilds = other.m_ownChilds;
		m_childs = other.m_childs;
		m_material = other.m_material;
	}

	~SceneObject(void)
	{		
		if (m_ownChilds)
		{
			for(child_it c_it = m_childs.begin();
				c_it != m_childs.end();
				c_it++)
			{
				if (*c_it != NULL)
				{
					delete *c_it;
					*c_it = NULL;
				}
			}
		}
	}

	virtual void render() = 0;
	virtual void draw(bool recursive)
	{
		glPushMatrix();
		transform.apply();

		if (!m_hidden)
		{
			if (m_material != NULL)
			{
				m_material->apply();
			}
			render();
		}

		if (recursive)
		{
			for (child_it c_it = m_childs.begin();
				c_it != m_childs.end();
				c_it ++)
			{
				if (*c_it != NULL)
				{
					(*c_it)->draw(true);
				}
			}
		}
		glPopMatrix();
	}

	void setMaterial(Material * material) 
	{
		m_material = material;
	}

	void attach(SceneObject * other, bool is_owned)
	{
		assert(other != NULL && "Cannot attach to a NULL scene object.");
		assert(other != this && "Cannot attack to itself.");

		(*other).m_childs.push_back(this);
		int idx_added = (*other).m_childs.size() - 1;
		if (is_owned) 
		{
			(*other).m_ownedChilds.push_back(idx_added);
		}
	}

	bool is_hidden()
	{
		return m_hidden;
	}
};

