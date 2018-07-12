#include "serverapp.h"
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
	networkinterface::dispatch(session, pack.id(), &pack);
}

void serverapp::on_udpreceive(sockaddr_in* addr, char* data, size_t length)
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

bool serverapp::registerserver(const std::string&  ip, const int port,const bool ipv6, uv_tcp_connection::connectcallback callback)
{
	uv_tcp_connection* connection = this->service()->getconnection(ip, port);

	if (connection )
	{
		if (connection->is_connect())
		{
			if (callback != NULL)
			{
				callback(connection, connection->is_connect());
			}
			return true;
		}
		else
		{
			return connection->connect(ip, port, ipv6, callback);
		}
	}
	else
	{
		connection = new uv_tcp_connection(this->service());

		this->service()->addconnection(connection);

	    return 	connection->connect(ip, port, ipv6,callback);
		
	}
	
}
void serverapp::on_registerserver_request(const uv_tcp_session* session, const int id, const packet*data)
{
	
}
void serverapp::on_registerserver_return(const uv_tcp_session* session, const int id, const packet*data)
{

}