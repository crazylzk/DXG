#ifndef DXGSCENE_H
#define DXGSCENE_H
#include<vector>

class CDXGSceneObject;
class IDXGSceneVisitor
{

public:
	virtual void visit(CDXGSceneObject *vObj) = 0;
};
class CDXGScene
{
private:
	std::vector<CDXGSceneObject *> m_Objs;
public:
	typedef CDXGSceneObject * CREATOR_FUNC(void * vContex);
public:
	CDXGScene(void);
	~CDXGScene(void);
	void update();
	void onVisit(IDXGSceneVisitor * vVistor);
	CDXGSceneObject * getSceneObject( const char * vName);

public:
	// factor method
	static bool registerClass(const char *vClassName, CREATOR_FUNC vFunc);
	static void unRegsiterClass(const char *vClassName);

	static CDXGSceneObject * create(const char *vClassName, void *vContex);

};

#endif
