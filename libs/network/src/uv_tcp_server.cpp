#include "uv_tcp_server.h"

namespace network {

	uv_tcp_server::uv_tcp_server(uv_service* service) :
		m_service(service),
		m_init(false),
		m_ipv6(false),
		m_handle()
	{

	}

	uv_tcp_server::~uv_tcp_server()
	{
		m_init = false;
		m_ipv6 = false;
		m_service = NULL;
		uv_close((uv_handle_t*)&m_handle, NULL);

	}

	bool uv_tcp_server::initialize(const char* ip, const int port, bool ipv6)
	{
		if (m_init)
		{
			return true;
		}
		m_ipv6 = ipv6;

		if (m_service == NULL)
		{
			return false;
		}

		int r = uv_tcp_init(m_service->loop(), &m_handle);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}

		r = m_ipv6 ? bind_ipv6(ip, port) : bind_ipv4(ip, port);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}

		r = listen();
		if (r != 0)
		{
			ASSERT(r != 0);
			return false;
		}

		m_handle.data = this;

		return true;
	}

	void uv_tcp_server::close()
	{
		if (m_init)
		{
			uv_close((uv_handle_t*)&m_handle, on_close);
		}
		m_init = false;

	}

	void uv_tcp_server::send(uv_session* session, const char* data, const size_t length)
	{
		if (session == NULL)
		{
			return;
		}

		auto buffer = session->tcp_writebuf();
		if (buffer.len < length)
		{
			buffer.base = (char*)realloc(buffer.base, length);
			buffer.len = length;
		}

		memset(buffer.base, 0, buffer.len);
		memcpy(buffer.base, data, length);
		buffer.len = length;

		int r = uv_write(&session->tcp_write(), (uv_stream_t*)session->tcp(), &buffer, 1, on_send);

		ASSERT(r == 0);
	}

	bool uv_tcp_server::no_delay(bool enable)
	{
		int r = uv_tcp_nodelay(&m_handle, enable ? 1 : 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	bool uv_tcp_server::keep_alive(int enable, unsigned int delay)
	{
		int r = uv_tcp_keepalive(&m_handle, enable, delay);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	int uv_tcp_server::bind_ipv4(const char* ip, const int port)
	{
		struct sockaddr_in addr;
		int r = uv_ip4_addr(ip, port, &addr);
		if (r != 0)
		{
			return r;
		}
		r = uv_tcp_bind(&m_handle, (const sockaddr*)&addr, 0);
		if (r != 0)
		{
			return r;
		}
		return r;
	}
	int uv_tcp_server::bind_ipv6(const char* ip, const int port)
	{
		struct sockaddr_in6 addr;
		int r = uv_ip6_addr(ip, port, &addr);
		if (r != 0)
		{
			return r;
		}
		r = uv_tcp_bind(&m_handle, (const sockaddr*)&addr, 0);
		if (r != 0)
		{
			return r;
		}

		return r;
	}

	int uv_tcp_server::listen(int backlog)
	{
		int r = uv_listen((uv_stream_t*)&m_handle, backlog, on_accept);
		return r;
	}


	void  uv_tcp_server::on_accept(uv_stream_t* handle, int status)
	{
		if (status != 0)
		{
			ASSERT(status == 0);
			return;
		}
		if (handle == nullptr)
		{
			return;
		}

		if (handle->data == nullptr)
		{
			return;
		}

		uv_tcp_server *server = (uv_tcp_server*)handle->data;

		if (server == nullptr)
		{
			return;
		}

		static int sessionId = 0;

		++sessionId;

		uv_session* session = new uv_session(sessionId);

		session->service(server->service());

		int r = uv_tcp_init(server->service()->loop(), session->tcp());
		if (r != 0)
		{
			delete session;
			ASSERT(r == 0);
			return;
		}

		r = uv_accept((uv_stream_t*)&server->m_handle, (uv_stream_t*)session->tcp());
		if (r != 0)
		{
			uv_close((uv_handle_t*)session->tcp(), NULL);

			delete session;

			ASSERT(r != 0);

			return;
		}

		server->service()->on_newsession(session);

		r = uv_read_start((uv_stream_t*)session->tcp(), on_alloc_buffer, on_receive);

		ASSERT(r == 0);

	}
	void  uv_tcp_server::on_receive(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		if (handle->data == NULL)
		{
			return;
		}

		uv_session* session = (uv_session*)handle->data;

		if (session == NULL || session->service() == NULL)
		{
			return;
		}
		auto service = session->service();

		if (nread > 0)
		{
			service->on_tcp_receive(session, buf->base, buf->len);
		}
		else if (nread == 0)
		{
			/* Everything OK, but nothing read. */
		}
		else
		{
			if (nread == UV_EOF) {

				fprintf(stdout, "client %d disconnected, close it.\n", session->id());
			}
			else if (nread == UV_ECONNRESET) {
				fprintf(stdout, "client %d disconnected unusually, close it.\n", session->id());
			}
			else
			{
				ASSERT(nread >= 0);
			}
			service->close(session->id());
		}

	}

	void  uv_tcp_server::on_send(uv_write_t* req, int status)
	{
		ASSERT(status != 0);
	}

	void  uv_tcp_server::on_alloc_buffer(uv_handle_t* hanle, size_t suggested_size, uv_buf_t* buf)
	{
		assert(hanle->data != NULL);

		uv_session* session = (uv_session*)hanle->data;

		*buf = session->tcp_readbuf();
	}

	void  uv_tcp_server::on_close(uv_handle_t* handle)
	{
		free(handle);

	}
}