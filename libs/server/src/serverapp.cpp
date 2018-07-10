#include "serverapp.h"
#include "pbmessage.h"
#include "server.pb.h"

serverapp::serverapp()
{
}

serverapp::~serverapp()
{
}
void serverapp::on_newsession(uv_tcp_session* session)
{
	
}

void serverapp::on_removesession(uv_tcp_session* session)
{

}

void serverapp::on_tcpreceive(uv_tcp_session* session, char* data, size_t length)
{
	packet pack(data, length);

	//·Ö·¢ÍøÂç°ü
	networkinterface::dispatch(this, pack.id(), &pack);
}

void serverapp::on_udpreceive(sockaddr_in* addr, char* data, size_t length)
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
	pb::ss_register_request req;
	req.mutable_info()->set_id(0);

	return true;
}
void serverapp::on_registerserver(uv_tcp_session* session, packet*data)
{
	
}