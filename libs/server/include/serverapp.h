#pragma once
#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_
#include "uv_service.h"
#include "nocopyable.h"
#include "packet.h"
#include "networkinterface.h"
#include "eventinterface.h"
#include "pb.h"

using namespace network;

enum serverapp_type
{
	APP_NONE,
	APP_LOGIN,
	APP_GATEWAY,
	APP_LOGIC,
	APP_GAME,
	APP_DB,

	APP_MAX
};
static char serverapp_name [APP_MAX][32] = {
	"none",
	"login",
	"gateway",
	"logic",
	"game",
	"db"
};

class serverapp:public uv_service_handler,
				public nocopyable
{
public:
	serverapp(int id, serverapp_type type);
	virtual ~serverapp();

	

	const int					id()const { return m_id; }
	const serverapp_type		type()const { return m_type; }
	const char*					name();
	/*
	请求连接别的服务器
	*/
	bool						registerserver(const std::string& ip, const int port, const bool ipv6 =false, uv_tcp_connection::connectcallback callback = NULL);

protected:
	/*
	有新的连接，可能是客户端，也有可能是别的服务器
	*/
	virtual void on_newsession(uv_tcp_session* session) ;
	virtual void on_closesession(uv_tcp_session* session) ;
	/*
	通过tcp收到的包，包括客户端与服务器和服务器之间的消息
	*/
	virtual void on_tcpreceive(uv_tcp_session* session, char* data, size_t length);
	/*
	通过udp收到的包
	*/
	virtual void on_udpreceive(sockaddr_in* addr, char* data, size_t length);

protected:
	/*
	别的服务器请求注册到本服务器
	*/
	virtual void on_registerserver_request(const uv_tcp_session* session, const int id, const packet*data);
	/*
	请求注册到别的服务器的结果
	*/
	virtual void on_registerserver_return(const uv_tcp_session* session, const int id, const packet*data);

	virtual bool initialize();
	virtual void update();
	virtual void shutdown();
private:
	int						m_id;
	serverapp_type			m_type;
};


#endif //_SERVERAPP_H_
