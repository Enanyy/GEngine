#pragma once
#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_
#include "uv_service.h"
#include "nocopyable.h"
#include "packet.h"
#include "networkinterface.h"
#include "eventdispatch.h"
using namespace network;
class serverapp:public uv_service_handler,
				public nocopyable
{
public:
	serverapp();
	virtual ~serverapp();

	virtual bool initialize();
	virtual void shutdown();

protected:
	void on_newsession(uv_session* session) override;
	void on_tcp_receive(uv_session* session, char* data, size_t length) override;
	void on_udp_receive(sockaddr_in* addr, char* data, size_t length) override;


private:

};


#endif //_SERVERAPP_H_
