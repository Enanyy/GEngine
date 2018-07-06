#ifndef _UV_UDP_SERVER_H_
#define _UV_UDP_SERVER_H_
#include <string>
#include <assert.h>
#include "uv_service.h"
#include "uv_session.h"
#include "uv_net.h"
namespace network {

	class uv_service;
	class uv_udp_server
	{

	public:
		uv_udp_server(uv_service* service);
		virtual ~uv_udp_server();

		bool initialize(const char* ip, const int port, bool ipv6 = false);

		void close();

		void send(const char* ip, const int port, const char* data, const size_t length);

		void send(const sockaddr* addr, const char* data, const size_t length);

		uv_buf_t& readbuf() { return m_readbuf; }
		uv_buf_t& writebuf() { return m_writebuf; }

		uv_service*	service() const { return m_service; }

		uv_udp_send_t& sendreq() { return m_sendreq; }

		bool set_broadcast(bool enable);

	protected:

		int bind_ipv4(const char* ip, const int port);
		int bind_ipv6(const char* ip, const int port);
		int listen();



	private:
		static void on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void on_send(uv_udp_send_t* req, int status);
		static void on_receive(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);
		static void on_close(uv_handle_t* handle);

	private:

		uv_service*			m_service;
		uv_udp_t			m_handle;

		uv_udp_send_t		m_sendreq;
		uv_buf_t			m_writebuf;
		uv_buf_t			m_readbuf;

		bool				m_init;
		bool				m_ipv6;
	};
}

#endif // !_UV_UDP_SERVER_H_
