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
		m_service = nullptr;

		if (is_active())
		{
			uv_close((uv_handle_t*)&m_handle, on_close);
		}

	}

	bool uv_tcp_server::initialize(const std::string& ip, const int port, bool ipv6)
	{
		if (m_init)
		{
			return true;
		}
		m_ipv6 = ipv6;

		if (m_service == nullptr)
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
		if (is_active())
		{	
			uv_close((uv_handle_t*)&m_handle, on_close);
		}
		m_init = false;

	}

	void uv_tcp_server::send(uv_session* session, const char* data, const size_t length)
	{
		if (session == nullptr)
		{
			return;
		}

		auto buffer = session->writebuf();
		if (buffer.len < length)
		{
			buffer.base = (char*)realloc(buffer.base, length);
			buffer.len = length;
		}

		memset(buffer.base, 0, buffer.len);
		memcpy(buffer.base, data, length);
		buffer.len = length;

		int r = uv_write(&session->write(), (uv_stream_t*)session->tcp(), &buffer, 1, on_send);

		ASSERT(r == 0);
	}
	
    const bool uv_tcp_server::is_active()
	{
		return uv_is_active((const uv_handle_t*)&m_handle) != 0;
	}
	const unsigned long	uv_tcp_server::ip() const
	{
		return uv_tcp_get_ip(&m_handle, m_ipv6);
	}
	const unsigned short uv_tcp_server::port()const
	{
		return uv_tcp_get_port(&m_handle, m_ipv6);

	}

	bool uv_tcp_server::setnodelay(bool enable)
	{
		int r = uv_tcp_nodelay(&m_handle, enable ? 1 : 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	bool uv_tcp_server::setkeepalive(int enable, unsigned int delay)
	{
		int r = uv_tcp_keepalive(&m_handle, enable, delay);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	int uv_tcp_server::bind_ipv4(const std::string& ip, const int port)
	{
		struct sockaddr_in addr;
		int r = uv_ip4_addr(ip.c_str(), port, &addr);
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
	int uv_tcp_server::bind_ipv6(const std::string& ip, const int port)
	{
		struct sockaddr_in6 addr;
		int r = uv_ip6_addr(ip.c_str(), port, &addr);
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

		uv_session* session = new uv_session(server->service(), server->is_ipv6());


		int r = uv_tcp_init(server->service()->loop(), session->tcp());
		if (r != 0)
		{
			SAFE_DELETE(session);
			ASSERT(r == 0);
			return;
		}

		r = uv_accept((uv_stream_t*)&server->m_handle, (uv_stream_t*)session->tcp());
		if (r != 0)
		{
			uv_close((uv_handle_t*)session->tcp(), nullptr);

			SAFE_DELETE(session);

			ASSERT(r != 0);

			return;
		}

		server->service()->on_newsession(session);

		r = session->receive();

		ASSERT(r == 0);

	}

	

	void  uv_tcp_server::on_send(uv_write_t* req, int status)
	{
		ASSERT(status != 0);
	}

	
	void  uv_tcp_server::on_close(uv_handle_t* handle)
	{
		SAFE_FREE(handle);
	}
}