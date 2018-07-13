#pragma once
#ifndef _UV_SERVICE_H_
#define _UV_SERVICE_H_
#include <map>
#include <string>

#include "uv.h"
#include "ikcp.h"
#include "uv_session.h"
#include "uv_tcp_server.h"
#include "uv_udp_server.h"
#include "uv_service_handler.h"

namespace network {

	class uv_service_handler;
	class uv_session;
	class uv_tcp_server;
	class uv_udp_server;
	class uv_tcp_connection;

	class uv_service
	{
		friend class uv_tcp_server;
		friend class uv_udp_server;
		friend class uv_session;
		friend class uv_tcp_connection;

	public:
		uv_service(uv_service_handler* handler);
		~uv_service();

		bool initialize(const std::string& ip, const int tcp_port, const int udp_port, bool ipv6 = false);

		bool run() const;

		void shutdown();

		uv_session*				getsession(int id);
		void					closesession(int id);
		int						generateid() { static int id = 0; return ++id; }

		void					error(int status);
		const std::string&		error() const { return m_error; }

		uv_loop_t*				loop() const { return m_loop; }
		uv_service_handler*		handler() const { return m_handler; }

		uv_tcp_server*			tcp()	{ return m_tcp; }
		uv_udp_server*			udp()	{ return m_udp; }
		
		bool					connect(const std::string& ip, const int port, bool ipv6 = false);
		uv_session*				getconnection(int id);
		uv_session*				getconnection(const std::string& ip, const int port);
		void					closeconnection(int id);

	private:
		void on_newsession(uv_session* session);
		void on_tcpreceive(uv_session* session, char* data, size_t length);
		void on_udpreceive(sockaddr_in* addr, char* data, size_t length);

		void on_newconnection(uv_session* session);

	private:
		static void on_connect(uv_connect_t* req, int status);

	private:
		uv_service_handler*					m_handler;
		uv_loop_t*							m_loop;
		uv_tcp_server*						m_tcp;
		uv_udp_server*						m_udp;

		//�����������ķ�����������
		std::map<int,uv_session*>			m_connections;
		uv_connect_t						m_connect;


		//�ͻ��ˣ����ķ��������뱾������������
		std::map<int, uv_session*>			m_sessions;
		std::string							m_error;
		bool								m_shutdown;
		bool								m_init;
	};
}
#endif //_UV_SERVICE_H_