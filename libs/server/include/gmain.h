#ifndef _G_MAIN_H_
#define _G_MAIN_H_
#include "uv_service.h"
#ifdef _MSC_VER
#pragma comment (lib, "network.lib")
#pragma comment (lib, "server.lib")
#pragma comment (lib, "pb.lib")
#pragma comment (lib, "libprotobuf.lib")
#else

#endif
template <class SERVERAPP>
int gmain(int argc, char * argv[])
{
	int tcp_port = 7000;
	int udp_port = 8000;

	SERVERAPP app(1,APP_NONE);

	uv_service service(&app);
	if (service.initialize("0.0.0.0", tcp_port, udp_port, false))
	{
		printf("service listen on tcp:%d udp:%d\n", tcp_port, udp_port);

		service.run();
	}
	
	service.shutdown();
	
	return 0;
}

#define GENGINE_MAIN(SERVERAPP)										\
int main(int argc, char * argv[])									\
{																	\
	return gmain<SERVERAPP>(argc, argv);							\
}													

#endif // !_G_MAIN_H_
