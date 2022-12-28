#include <iostream>
#include <random>
#include <unistd.h>

#include "header/user_api.h"

using namespace std;


int main(int argc, char const *argv[])
{
	// PerceptionShmApi api;
	PerceptionShmApi::InitObjShm("tpperception");
	PercetionObjects objct;
	while (1) {
		PerceptionShmApi::PrintShmInfo();
		if (PerceptionShmApi::GetPerceptionData(objct) != false) {
			std::cout << objct.ToString();
		}
		usleep(1000 * 100);
	}


	PerceptionShmApi::UninitObjShm();
	return 0;
}



