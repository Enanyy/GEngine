
#include "uv_service.h"
#include "gmain.h"
#include "serverapp.h"
int main(int argc, char * argv[])
{
	return gmain<serverapp>(argc,argv);
}