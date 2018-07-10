#pragma once
#ifndef _UV_SERVICE_HANDLER_H_
#define _UV_SERVICE_HANDLER_H_
#include "uv_service.h"
#include "uv_session.h"

namespace network {

	class uv_service;
	class uv_session;
	class uv_service_handler
	{
		friend class uv_service;
	public:
		uv_service_handler():m_service(NULL) {}
		virtual ~uv_service_handler() { m_service = NULL; }

		uv_service* service() const { return m_service; }

	protected:
		virtual void on_newsession(uv_session* session) = 0;
		virtual void on_tcp_receive(uv_session* session, char* data, size_t length) = 0;
		virtual void on_udp_receive(sockaddr_in* addr, char* data, size_t length) = 0;

		virtual void update() = 0;

	private:
		uv_service* m_service;
	};






}
#endif //_UV_SERVICE_HANDLER_H_