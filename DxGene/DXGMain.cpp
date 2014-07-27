#include "DXGEnv.h"
int main()
{
    CDXGEnv *env = new CDXGEnv();

	env->initEnv( 100, 100, 1024, 768, true);
	env->goMainLoop();

	delete env;

}
