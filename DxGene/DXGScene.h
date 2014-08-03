#ifndef DXGSCENE_H
#define DXGSCENE_H
#include <vector>
#include <map>

class CDXGSceneObject;
class IDXGSceneVisitor
{

public:
	virtual void visit(CDXGSceneObject *vObj) = 0;
};
class CDXGScene
{
public:
	typedef CDXGSceneObject * CREATOR_FUNC(void * vContex);
	typedef std::vector<CDXGSceneObject *> SOBJ_VEC;
	typedef std::map<const char *, CREATOR_FUNC> CREATOR_MAP;
private:
	SOBJ_VEC m_Objs;
public:
	
public:
	CDXGScene(void);
	~CDXGScene(void);
	void update();
	void onVisit(IDXGSceneVisitor * vVistor);
	
	CDXGSceneObject * getSceneObject( const char * vName);
	CDXGSceneObject * getSceneRoot();

public:
	// factor method
	static bool registerClass(const char *vClassName, CREATOR_FUNC vFunc);
	static void unRegsiterClass(const char *vClassName);
	static CDXGSceneObject * create(CDXGScene *scene, const char *vClassName, void *vContex);

private:
	static CREATOR_MAP creatorMap;

};

#endif
