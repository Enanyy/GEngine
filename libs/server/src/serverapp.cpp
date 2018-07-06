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