#include "DXGScene.h"
#include <queue>
typedef std::queue<CDXGSceneObject *> SOBJ_QUE;
CDXGScene::CREATOR_MAP CDXGScene::creatorMap;
CDXGScene::CDXGScene(void)
{
	m_Root = new CDXGSceneObject();
}

CDXGScene::~CDXGScene(void)
{
	delete m_Root;
}
void CDXGScene::update()
{
	SOBJ_QUE squeue;
	squeue.push(m_Root);
	while(!squeue.empty())
	{
		CDXGSceneObject * obj = squeue.front();
		squeue.pop();
		obj->update();
		for(int i = 0; i < obj->getChildrenSize(); i++)
		{
			squeue.push( obj->getChild(i));
		}
	}
}
void CDXGScene::onVisit(IDXGSceneVisitor * vVistor)
{
	SOBJ_VEC::iterator itr;
	for(itr = m_Objs.begin();)
}
	
	CDXGSceneObject * getSceneObject(const char * vName);
	CDXGSceneObject * getSceneRoot();

public:
	// factor method
	static bool registerClass(const char *vClassName, CREATOR_FUNC vFunc);
	static void unRegsiterClass(const char *vClassName);
	static CDXGSceneObject * create(CDXGScene *scene, const char *vClassName, void *vContex);

private:
	static CREATOR_MAP creatorMap;