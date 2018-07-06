#ifndef _UV_SERVICE_H_
#define _UV_SERVICE_H_
#include <map>
#include <string>

#include "uv.h"
//#include "ikcp.h"
#include "uv_session.h"
#include "uv_tcp_server.h"
#include "uv_udp_server.h"
#include "uv_service_handler.h"

namespace uv
{
	class uv_service
	{
	public:
		uv_service(uv_service_handler* handler);
		~uv_service();

		bool initialize(const char* ip, const int tcp_port, const int udp_port, bool ipv6 = false);

		bool run() const;

		void shutdown();

		uv_session* session(int id);
		void close(int id);

		void on_newsession(uv_session* session);
		void on_tcp_receive(uv_session* session, char* data, size_t length);
		void on_udp_receive(sockaddr_in* addr, char* data, size_t length);

		void error(int status);
		const std::string& error() const { return m_error; }

		uv_loop_t* loop() const { return m_loop; }
		uv_service_handler* handler() const { return m_handler; }

		uv_tcp_server* tcp() { return &m_tcp; }
		uv_udp_server* udp() { return &m_udp; }
	
	private:

	private:
		uv_service_handler*				m_handler;
		uv_loop_t*						m_loop;
		uv_tcp_server					m_tcp;
		uv_udp_server					m_udp;
		std::map<int, uv_session*>		m_sessions;
		std::string						m_error;
		bool							m_shutdown;
		bool							m_init;
	};
}
#endif //_UV_SERVICE_H_