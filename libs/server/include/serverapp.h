#pragma once
#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_
#include "uv_service.h"
#include "nocopyable.h"
#include "packet.h"
#include "networkinterface.h"
#include "dispatcher.h"
using namespace network;

typedef unsigned int SERVERAPPID;
typedef unsigned int SERVERAPPTYPE;

class serverapp:public uv_service_handler,
				public nocopyable
{
public:
	serverapp();
	virtual ~serverapp();

	virtual bool initialize();
	virtual void shutdown();

	const SERVERAPPID			id()const { return m_id; }
	const SERVERAPPTYPE			type()const { return m_type; }

	bool						registerserver(const std::string& ip, const int port);

protected:
	void on_newsession(uv_session* session) override;
	void on_tcp_receive(uv_session* session, char* data, size_t length) override;
	void on_udp_receive(sockaddr_in* addr, char* data, size_t length) override;

	void on_registerserver(uv_session* session,char* data, size_t length);

	virtual void update();
private:
	SERVERAPPID			m_id;
	SERVERAPPTYPE		m_type;
};


#endif //_SERVERAPP_H_
