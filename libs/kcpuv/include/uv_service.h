#pragma once
#ifndef _UV_SERVICE_H_

#include <map>
#include <string>

#include "uv.h"
#include "ikcp.h"
#include "uv_session.h"
#include "uv_tcp_server.h"

namespace uv
{
	class uv_service_handler
	{
	
	public:
		uv_service_handler();
		virtual ~uv_service_handler();

		virtual void on_connect(uv_session* session) = 0;
		virtual void on_receive(uv_session* session,char* data, unsigned int length) = 0;


	};

	
	class uv_service
	{
	public:
		uv_service(uv_service_handler* handler);
		~uv_service();

		uv_loop_t* loop() const { return m_loop; }
		uv_service_handler* handler() const { return m_handler; }

		bool run() const;

		void shutdown();

		uv_session* session(int id);
		void close(int id);

		void on_connect(uv_session* session);

		void error(int status);
		const std::string& error() const { return m_error; }
	
	private:

	private:
		uv_service_handler*				m_handler;
		uv_loop_t*						m_loop;
		uv_tcp_server					m_tcp;
		std::map<int, uv_session*>		m_sessions;
		std::string						m_error;
		bool							m_shutdown;


	};
}
#endif //_UV_SERVICE_H_