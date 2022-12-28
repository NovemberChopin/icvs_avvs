#include <iostream>
#include <random>
#include <unistd.h>

#include "header/user_api.h"

using namespace std;
default_random_engine e;
void GenerateData(PercetionObjects& objct) {
	objct.index = 0;

	for (int i = 0; i < 10; i++) {

		Object obj;
		obj.id = i;
		obj.x = (float)e() / e.max();
		if (i == 0)
			std::cout << "obj.x " << obj.x << std::endl;
		obj.y = (float)e() / e.max();
		obj.z = (float)e() / e.max();
		obj.vxrel = (float)e() / e.max();
		obj.vyrel = (float)e() / e.max();
		obj.vzrel = (float)e() / e.max();
		obj.vabs = (float)e() / e.max();
		objct.objs[objct.index++] = obj;
	}
}


int main(int argc, char const *argv[])
{
	// PerceptionShmApi api;
	PerceptionShmApi::InitObjShm("tpperception");
	PercetionObjects objct;
	while (1) {
		GenerateData(objct);
		PerceptionShmApi::SetPerceptionData(objct);
		std::cout << "publish ok" << std::endl;
		PerceptionShmApi::PrintShmInfo();
		usleep(1000 * 1000);
	}


	PerceptionShmApi::UninitObjShm();
	return 0;
}



