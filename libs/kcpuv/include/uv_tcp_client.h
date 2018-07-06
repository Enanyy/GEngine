#pragma once
#ifndef UV_TCP_CLIENT_H_
#define UV_TCP_CLIENT_H_

#include "uv.h"
#include <string>
#include <assert.h>
#include "uv_net.h"

namespace uv
{
	

	class uv_tcp_client
	{
		typedef void(*connect_callback)(int status);
		typedef void(*receive_callback)(char* data, size_t length);
	public:
		uv_tcp_client(uv_loop_t* loop = uv_default_loop());
		virtual ~uv_tcp_client();

		bool start(const char* ip, const unsigned port);
		void close();

		void send(const char* data, const size_t length);
		void set_connect_callback(connect_callback callback) { m_connect_callback = callback; }
		void set_receive_callback(receive_callback callback) { m_receive_callback = callback; }
		bool set_no_delay(bool enable);
		bool set_keep_alive(int enable, unsigned int delay);

		uv_buf_t& read_buffer() { return m_read_buffer; }
		uv_buf_t& write_buffer() { return m_write_buffer; }

	protected:
		bool init();
		bool connect(const char* ip, const unsigned port);
		bool run();
		void error(int status);

		static void on_connect(uv_connect_t* req, int status);
		static void on_receive(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);
		static void on_send(uv_write_t* req, int statu);
		static void on_alloc_buffer(uv_handle_t* hanle, size_t suggested_size, uv_buf_t* buf);
		static void on_close(uv_handle_t* handle);
		

	private:
		uv_loop_t*				m_loop;
		uv_tcp_t				m_socket;
		uv_connect_t			m_connect_req;
		uv_write_t				m_write_req;
		uv_buf_t				m_read_buffer;
		uv_buf_t				m_write_buffer;

		std::string				m_error;
		connect_callback		m_connect_callback;
		receive_callback		m_receive_callback;

		bool					m_init;

	};

	
}


#endif // !UV_TCP_CLIENT_H_

