#pragma once
#ifndef _UV_TCP_CLIENT_H_
#define _UV_TCP_CLIENT_H_

#include "uv.h"
#include <string>
#include <assert.h>
#include "uv_net.h"
#include "uv_service.h"
#include "uv_session.h"

namespace network
{
	class uv_service;
	class uv_session;
	class uv_tcp_connection
	{
	
	public:
		uv_tcp_connection(uv_service* service);
		virtual ~uv_tcp_connection();

		bool initialize();
		bool connect(const char* ip, const int port, bool ipv6 = false);
		void disconnect();

		void close();

		void send(const char* data, const size_t length);
	
		bool no_delay(bool enable);
		bool keep_alive(int enable, unsigned int delay);

		uv_service*				service() const { return m_service; }
		uv_session*				session() const  { return m_session; }

		const bool is_connect() const{ return m_connect; }
	
	protected:
		

		static void on_connect(uv_connect_t* req, int status);
		static void on_receive(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
		static void on_send(uv_write_t* req, int statu);
		static void on_alloc_buffer(uv_handle_t* hanle, size_t suggested_size, uv_buf_t* buf);
		static void on_close(uv_handle_t* handle);
		

	private:
		uv_service*			    m_service;
		uv_session*				m_session;
		uv_connect_t			m_tcp_connect;
	

		
		bool					m_init;
		bool					m_ipv6;
		bool					m_connect;
	};
}


#endif // !_UV_TCP_CLIENT_H_

