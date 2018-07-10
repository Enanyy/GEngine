#pragma once
#ifndef _UV_SERVICE_H_
#define _UV_SERVICE_H_
#include <map>
#include <string>

#include "uv.h"
#include "ikcp.h"
#include "uv_tcp_session.h"
#include "uv_tcp_server.h"
#include "uv_udp_server.h"
#include "uv_tcp_connection.h"
#include "uv_service_handler.h"

namespace network {

	class uv_service_handler;
	class uv_tcp_session;
	class uv_tcp_server;
	class uv_udp_server;
	class uv_tcp_connection;

	class uv_service
	{
		friend class uv_tcp_server;
		friend class uv_udp_server;
		friend class uv_tcp_connection;

	public:
		uv_service(uv_service_handler* handler);
		~uv_service();

		bool initialize(const std::string& ip, const int tcp_port, const int udp_port, bool ipv6 = false);

		bool run() const;

		void shutdown();

		uv_tcp_session*			get_session(int id);
		void					close_session(int id);
		int						gen_sessionid() { static int id = 0; return ++id; }

	
		void					error(int status);
		const std::string&		error() const { return m_error; }

		uv_loop_t*				loop() const { return m_loop; }
		uv_service_handler*		handler() const { return m_handler; }

		uv_tcp_server*			tcp()	{ return m_tcp; }
		uv_udp_server*			udp()	{ return m_udp; }
		
		bool					add_connection(uv_tcp_connection* client);
		uv_tcp_connection*		get_connection(int id);
		uv_tcp_connection*      get_connection(const std::string& ip, const int port);

	private:
		void on_newsession(uv_tcp_session* session);
		void on_tcpreceive(uv_tcp_session* session, char* data, size_t length);
		void on_udpreceive(sockaddr_in* addr, char* data, size_t length);

	private:
		uv_service_handler*					m_handler;
		uv_loop_t*							m_loop;
		uv_tcp_server*						m_tcp;
		uv_udp_server*						m_udp;

		//本服务器与别的服务器的连接
		std::map<int,uv_tcp_connection*>	m_connections;

		//客户端（或别的服务器）与本服务器的连接
		std::map<int, uv_tcp_session*>		m_sessions;
		std::string							m_error;
		bool								m_shutdown;
		bool								m_init;
	};
}
#endif //_UV_SERVICE_H_