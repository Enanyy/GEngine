#ifndef _G_MAIN_H_
#define _G_MAIN_H_
#include "uv_service.h"

#pragma comment (lib, "network.lib")
#pragma comment (lib, "server.lib")

template <class SERVERAPP>
int gmain(int argc, char * argv[])
{
	SERVERAPP app;
	app.initialize();

	uv_service service(&app);
	if (service.initialize("0.0.0.0", 1222, 1223, false))
	{
		service.run();
	}
	
	app.shutdown();
	service.shutdown();
	
	return 0;
}

#endif // !_G_MAIN_H_
