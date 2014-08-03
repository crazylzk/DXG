#ifndef DXGSCENEOBJECT_H
#define DXGSCENEOBJECT_H
#include "DXGEnv.h"
#include <vector>
#include <string>

class CDXRenderable;
class CDXGSceneObject
{
	typedef std::vector<CDXGSceneObject *> SOBJ_VEC;
private:
     XMMATRIX m_WorldMat;
	 XMMATRIX m_LocalMat;
	 bool m_LocalMatDirty;
	 bool m_WorldMatDirty;
	 CDXGSceneObject *m_Parent;
	 SOBJ_VEC m_Children;
	 CDXRenderable * m_Renderable; //����
	 std::string m_Name;

private:
	void markDesdentWorldDirty();

public:
	CDXGSceneObject();
	virtual ~CDXGSceneObject();
	const char * getName();
	void setName(const char *vName);
	void attachToParent(CDXGSceneObject * vParent);
	void detachFromParent(CDXGSceneObject *vParent);
	void setRenderable(CDXRenderable * vRenderable);
	void getWorldMat(XMMATRIX * out_World);
	void getLocalMat(XMMATRIX * out_Local);
	void setWorldMat(const XMMATRIX & vWorld );
	void setLocalMat(const XMMATRIX & vLocal );
public:
	virtual void update();

};

#endif