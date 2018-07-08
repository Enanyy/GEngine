#include "serverapp.h"
serverapp::serverapp()
{
}

serverapp::~serverapp()
{
}
void serverapp::on_newsession(uv_session* session)
{
	
}

void serverapp::on_tcp_receive(uv_session* session, char* data, size_t length)
{

}

void serverapp::on_udp_receive(sockaddr_in* addr, char* data, size_t length)
{

}

bool serverapp::initialize()
{
	printf("aSSSSSSSS\n");

 bool r =	networkinterface::instance()->listen<serverapp>(1, this, &serverapp::on_message);

	networkinterface::instance()->dispatch(NULL, 1, "aaa", 3);
	
	return true;
}
void serverapp::shutdown()
{

}

void  serverapp::on_message(void* session, const int id, const char* data, const size_t length)
{
	printf("id= %d, data=%s\n", id, data);
}