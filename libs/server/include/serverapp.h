#pragma once
#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_
#include "uv_service.h"
#include "nocopyable.h"
#include "packet.h"
#include "networkinterface.h"
#include "event.h"
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

	bool						registerserver(const std::string& ip, const int port);

protected:
	virtual void on_newsession(uv_tcp_session* session) ;
	virtual void on_closesession(uv_tcp_session* session) ;
	virtual void on_tcpreceive(uv_tcp_session* session, char* data, size_t length);
	virtual void on_udpreceive(sockaddr_in* addr, char* data, size_t length);

	virtual void on_registerserver(uv_tcp_session* session,packet*data);

	virtual bool initialize();
	virtual void update();
	virtual void shutdown();
private:
	int						m_id;
	serverapp_type			m_type;
};


#endif //_SERVERAPP_H_
