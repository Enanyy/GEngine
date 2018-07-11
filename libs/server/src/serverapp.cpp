#include "serverapp.h"
#include "pbmessage.h"
#include "server.pb.h"

serverapp::serverapp(int id, serverapp_type type):
		   m_id(id),m_type(type <0 || type >APP_MAX ? APP_NONE : type)
		  
{
}

serverapp::~serverapp()
{
}
const char* serverapp::name()
{
	if (m_type <0 || m_type > APP_MAX)
	{
		m_type = APP_NONE;
	}

	return serverapp_name[m_type];
}

void serverapp::on_newsession(uv_tcp_session* session)
{
	
}

void serverapp::on_closesession(uv_tcp_session* session)
{

}

void serverapp::on_tcpreceive(uv_tcp_session* session, char* data, size_t length)
{
	packet pack(data, length);

	if (pack.is_valid() == false)
	{
		pack.clear();
		return;
	}

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
	uv_tcp_connection* connection = this->service()->get_connection(ip, port);

	if (connection)
	{
		return true;
	}

	pb::ss_register_request req;

	req.mutable_info()->set_id(m_id);
	req.mutable_info()->set_type(m_type);
	req.mutable_info()->set_name(name());
	req.mutable_info()->set_ip(ip);
	req.mutable_info()->set_port(port);

	return true;
}
void serverapp::on_registerserver(uv_tcp_session* session, packet*data)
{
	
}