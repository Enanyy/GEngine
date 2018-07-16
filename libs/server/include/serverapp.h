#pragma once
#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_
#include "uv_service.h"
#include "nocopyable.h"
#include "packet.h"
#include "networkinterface.h"
#include "eventinterface.h"
#include "serverappinfo.h"
#include "pb.h"

using namespace network;

class serverapp:public uv_service_handler,
				public nocopyable
{
public:
	serverapp(int id, serverapptype type);
	virtual ~serverapp();

	

	const int					id()const { return m_id; }
	const serverapptype			type()const { return m_type; }
	const char*					name();
	/*
	请求连接别的服务器
	*/
	bool						connectserver(const std::string& ip, const int port, const bool ipv6);

protected:
	/*
	有新的连接，可能是客户端，也有可能是别的服务器
	*/
	virtual void on_newsession(uv_session* session) ;
	virtual void on_closesession(uv_session* session) ;
	/*
	通过tcp收到的包，包括客户端与服务器和服务器之间的消息
	*/
	virtual void on_tcpreceive(uv_session* session, const char* data, const size_t length);
	/*
	通过udp收到的包
	*/
	virtual void on_udpreceive(sockaddr_in* addr, const char* data, const size_t length);

	/*
	请求连接别的服务器成功的回调
	*/
	virtual void on_newconnection(uv_session* connection) ;
	/*
	与某个服务器断开连接
	*/
	virtual void on_closeconnection(uv_session* connection) ;

protected:
	/*
	别的服务器请求注册到本服务器
	*/
	virtual void on_registerserver_request(const uv_session* session, const int id, const packet*data);
	/*
	请求注册到别的服务器的结果
	*/
	virtual void on_registerserver_return(const uv_session* session, const int id, const packet*data);

	virtual bool initialize();
	virtual void update();
	virtual void shutdown();
private:
	int						m_id;
	serverapptype			m_type;
};


#endif //_SERVERAPP_H_
