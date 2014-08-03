#include "DXGSceneObject.h"


CDXGSceneObject::CDXGSceneObject()
{
   m_WorldMat = XMMatrixIdentity();
   m_LocalMat = XMMatrixIdentity();
   m_Renderable = NULL;
   m_LocalMatDirty = false;
   m_WorldMatDirty = false;
}
CDXGSceneObject::~CDXGSceneObject()
{

}
const char * CDXGSceneObject::getName()
{

	return m_Name.c_str();
}
void CDXGSceneObject::setName(const char *vName)
{
	Ret_If_Fail(vName != NULL);
	m_Name = vName;
}

void CDXGSceneObject::markDesdentWorldDirty()
{
	SOBJ_VEC::iterator itr;

	for(itr = m_Children.begin(); itr != m_Children.end(); ++itr)
	{
		(*itr)->m_WorldMatDirty = true;
		(*itr)->markDesdentWorldDirty();
	}
}
void CDXGSceneObject::attachToParent(CDXGSceneObject * vParent)
{
	Ret_If_Fail(vParent != NULL);

	vParent->m_Children.push_back(this);
}

void CDXGSceneObject::detachFromParent(CDXGSceneObject *vParent)
{
	Ret_If_Fail(vParent != NULL);
	
	SOBJ_VEC::iterator itr;

	for(itr = m_Children.begin(); itr != m_Children.end(); itr++)
	{
		if(*itr == vParent)
		{
				m_Children.erase(itr);
				break;
		}
	}

}

void CDXGSceneObject::setRenderable(CDXRenderable * vRenderable)
{
	Ret_If_Fail(vRenderable != NULL);

	m_Renderable = vRenderable;
}

void CDXGSceneObject::update()
{
    XMMATRIX parent;
	Ret_If_Fail((m_WorldMatDirty == false || m_LocalMatDirty == false) && " don't know how to caculate the mat" );
	
	parent = XMMatrixIdentity();
	if(m_Parent)
	{
		m_Parent->getWorldMat(&parent);
	}

	if(m_WorldMatDirty)
	{
		SOBJ_VEC::iterator itr;
		m_WorldMat = XMMatrixMultiply(m_LocalMat, parent);
		m_WorldMatDirty = false;

		for(itr = m_Children.begin(); itr != m_Children.end(); ++itr)
		{
			(*itr)->m_WorldMatDirty = true;
		}

	}

	if(m_LocalMatDirty)
	{
		XMMATRIX  parentInv;
		XMVECTOR  vec;
		parentInv = XMMatrixInverse(&vec, parent);
		m_LocalMat = XMMatrixMultiply(m_WorldMat, parentInv);
		m_LocalMatDirty = false;
	}
}

void CDXGSceneObject::getWorldMat(XMMATRIX * out_World)
{
	XMMATRIX parentMat;

	Ret_If_Fail(out_World != NULL && (m_WorldMatDirty == false || m_LocalMatDirty == false));
	if(m_WorldMatDirty)
	{
		if (m_Parent)
		{
			m_Parent->getWorldMat(&parentMat);
		}
		else
		{
			parentMat = XMMatrixIdentity();
		}
		m_WorldMat = XMMatrixMultiply(m_LocalMat, parentMat);
		
		m_WorldMatDirty = false;	
	}
	*out_World = m_WorldMat;
}
void CDXGSceneObject::getLocalMat(XMMATRIX * out_Local)
{
	XMMATRIX parent;
	Ret_If_Fail(out_Local != NULL && (m_WorldMatDirty ==false || m_LocalMatDirty == false) );
	
	parent = XMMatrixIdentity();
	if(m_Parent)
	{
		m_Parent->getWorldMat(&parent);
	}
	
	if(m_LocalMatDirty)
	{
		XMMATRIX  parentInv;
		XMVECTOR  vec;
		parentInv = XMMatrixInverse(&vec, parent);
		m_LocalMat = XMMatrixMultiply(m_WorldMat, parentInv);
		m_LocalMatDirty = false;    
	}
	
	*out_Local = m_LocalMat;

}
void CDXGSceneObject::setWorldMat(const XMMATRIX & vWorld )
{
	m_WorldMat = vWorld;
	m_WorldMatDirty = false;
	m_LocalMatDirty = true;
	markDesdentWorldDirty();
}
void CDXGSceneObject::setLocalMat(const XMMATRIX & vLocal )
{
	m_LocalMat = vLocal;
	m_LocalMatDirty = false;
	m_WorldMatDirty = true;
	markDesdentWorldDirty();
}