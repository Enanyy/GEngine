#pragma once
#ifndef _UV_TCP_SERVER_H_
#define _UV_TCP_SERVER_H_

#include <map>
#include <string>
#include <memory>
#include <assert.h>
#include "uv.h"
#include "uv_net.h"
#include "uv_service.h"
namespace network {

	class uv_service;
	class uv_session;
	class uv_tcp_server
	{

	public:

		uv_tcp_server(uv_service* service);
		virtual ~uv_tcp_server();

		bool					initialize(const std::string&  ip, const int port, bool ipv6 = false);
		void					close();

		virtual void			send(uv_session* session, const char* data, const size_t length);
		bool					no_delay(bool enable);
		bool					keep_alive(int enable, unsigned int delay);

		uv_service*				service() const { return m_service; }

		const bool				is_active();
		const bool				is_ipv6()const { return m_ipv6; }

	protected:

		static void on_accept(uv_stream_t* server, int status);
		static void on_receive(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
		static void on_send(uv_write_t* req, int status);
		static void on_alloc_buffer(uv_handle_t* hanle, size_t suggested_size, uv_buf_t* buf);
		static void on_close(uv_handle_t* handle);

	private:
		int bind_ipv4(const std::string& ip, const int port);
		int bind_ipv6(const std::string& ip, const int port);
		int listen(int backlog = 1024);



	private:
		uv_service*						m_service;
		uv_tcp_t						m_handle;

		bool							m_init;
		bool							m_ipv6;
	};

}

#endif // !_UV_UDP_SERVER_H_