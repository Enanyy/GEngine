#include "serverapp.h"
#include "server.pb.h"

serverapp::serverapp(int id, serverapptype type):
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

	return serverappname[m_type];
}

void serverapp::on_newsession(uv_session* session)
{
	
}

void serverapp::on_closesession(uv_session* session)
{

}

void serverapp::on_tcpreceive(uv_session* session, const char* data, const size_t length)
{
	packet pack(data, length);

	if (pack.is_valid() == false)
	{
		pack.clear();
		return;
	}

	//·Ö·¢ÍøÂç°ü
	networkinterface::dispatch(session, pack.id(), &pack);
}

void serverapp::on_udpreceive(sockaddr_in* addr, const char* data, const size_t length)
{

}

void serverapp::on_newconnection(uv_session* connection)
{

	pb::ss_register_request cmd;
	cmd.mutable_info()->set_id(this->id());
	cmd.mutable_info()->set_type(this->type());
	cmd.mutable_info()->set_name(this->name());
	//cmd.mutable_info()->set_ip(ip.c_str());
	//cmd.mutable_info()->set_port();

	std::string proto;
	cmd.SerializeToString(&proto);

	packet buf(pb::SS_REGISTER_REQUEST, proto.c_str(), proto.size(), this->id());

	this->service()->tcp()->send(connection, buf.data(), buf.length());

}
void serverapp::on_closeconnection(uv_session* connection)
{

}


bool serverapp::initialize()
{
	networkinterface::listen(pb::SS_REGISTER_REQUEST, this, &serverapp::on_registerserver_request);
	networkinterface::listen(pb::SS_REGISTER_RETURN, this, &serverapp::on_registerserver_return);

	return true;
}
void serverapp::shutdown()
{
	networkinterface::unlisten(pb::SS_REGISTER_REQUEST, this, &serverapp::on_registerserver_request);
	networkinterface::unlisten(pb::SS_REGISTER_RETURN, this, &serverapp::on_registerserver_return);
}

void serverapp::update()
{
}

bool serverapp::connectserver(const std::string&  ip, const int port, const bool ipv6)
{
	uv_session* connection = this->service()->getconnection(ip, port);

	if (connection )
	{
		if (connection->is_active())
		{
			return true;
		}
		else
		{
			this->service()->closeconnection(connection->id());
		}
	}
	
	return this->service()->connect(ip, port, ipv6);

}
void serverapp::on_registerserver_request(const uv_session* session, const int id, const packet*data)
{
	if (session == nullptr || data == nullptr || id != pb::SS_REGISTER_REQUEST)
	{
		return;
	}
	

}
void serverapp::on_registerserver_return(const uv_session* session, const int id, const packet*data)
{
	if (session == nullptr || data == nullptr || id != pb::SS_REGISTER_RETURN)
	{
		return;
	}
}