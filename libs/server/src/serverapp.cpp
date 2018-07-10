#include "serverapp.h"
#include "pbmessage.h"
#include "server.pb.h"

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
	

	return true;
}
void serverapp::shutdown()
{

}

void serverapp::update()
{
}

bool serverapp::registerserver(const std::string&  ip, const int port)
{
	

	return true;
}
void serverapp::on_registerserver(uv_session* session, char* data, size_t length)
{
	
}