#include "uv_udp_server.h"

namespace network {

	uv_udp_server::uv_udp_server(uv_service* service) :
		m_service(service),
		m_handle(),
		m_sendreq(),
		m_init(false),
		m_ipv6(false)
	{
		m_handle.data = this;
		m_writebuf = uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
		m_readbuf = uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
	}
	uv_udp_server:: ~uv_udp_server()
	{
		m_init = false;
		m_ipv6 = false;

		free(m_writebuf.base);
		free(m_readbuf.base);

		m_writebuf.base = nullptr;
		m_readbuf.base = nullptr;

		m_writebuf.len = 0;
		m_readbuf.len = 0;
	}

	bool uv_udp_server::initialize(const std::string& ip, const int port, bool ipv6)
	{
		if (m_init)
		{
			return true;
		}

		m_ipv6 = ipv6;

		if (m_service == NULL)
		{
			ASSERT(m_service != NULL);
			return false;
		}

		int r = uv_udp_init(m_service->loop(), &m_handle);
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
			ASSERT(r == 0);
			return false;
		}


		m_init = true;

		return true;
	}

	bool uv_udp_server::set_broadcast(bool enable)
	{
		int r = uv_udp_set_broadcast(&m_handle, enable ? 1 : 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	void uv_udp_server::close()
	{
		uv_close((uv_handle_t*)&m_handle, on_close);
		m_init = false;
	}

	void uv_udp_server::send(const std::string& ip, const int port, const char* data, const size_t length)
	{
		if (m_ipv6)
		{
			struct sockaddr_in6 addr;
			int r = uv_ip6_addr(ip.c_str(), port, &addr);
			if (r != 0)
			{
				ASSERT(r == 0);
				return;
			}
			send((const sockaddr*)&addr, data, length);
		}
		else
		{
			struct sockaddr_in addr;
			int r = uv_ip4_addr(ip.c_str(), port, &addr);
			if (r != 0)
			{
				ASSERT(r == 0);
				return;
			}

			send((const sockaddr*)&addr, data, length);
		}
	}

	void uv_udp_server::send(const sockaddr* addr, const char* data, const size_t length)
	{
		if (m_writebuf.len < length)
		{
			//重新分配内存
			m_writebuf.base = (char*)realloc(m_writebuf.base, length);
			m_writebuf.len = length;
		}
		else
		{
			memset(m_writebuf.base, 0, m_writebuf.len);
		}

		memcpy(m_writebuf.base, data, length);
		m_writebuf.len = length;

		int r = uv_udp_send(&m_sendreq, &m_handle, &m_writebuf, 1, addr, on_send);
		if (r != 0)
		{
			ASSERT(r == 0);
		}
	}

	int uv_udp_server::bind_ipv4(const std::string& ip, const int port)
	{
		struct sockaddr_in addr;
		int r = uv_ip4_addr(ip.c_str(), port, &addr);
		if (r != 0)
		{
			ASSERT(r == 0);
			return r;
		}

		r = uv_udp_bind(&m_handle, (const sockaddr*)&addr, 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return r;
		}

		return r;
	}
	int uv_udp_server::bind_ipv6(const std::string& ip, const int port)
	{
		struct sockaddr_in6 addr;
		int r = uv_ip6_addr(ip.c_str(), port, &addr);
		if (r != 0)
		{
			ASSERT(r == 0);
			return r;
		}

		r = uv_udp_bind(&m_handle, (const sockaddr*)&addr, 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return r;
		}

		return r;
	}

	int uv_udp_server::listen()
	{
		int r = uv_udp_recv_start(&m_handle, on_alloc_buffer, on_receive);
		if (r != 0)
		{
			ASSERT(r == 0);

			return r;
		}
		return r;
	}

	void uv_udp_server::on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		assert(handle->data != NULL);
		uv_udp_server* client = (uv_udp_server*)handle->data;
		*buf = client->readbuf();
	}
	void uv_udp_server::on_send(uv_udp_send_t* req, int status)
	{
		if (status != 0)
		{
			ASSERT(status == 0);
		}
	}
	void uv_udp_server::on_receive(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)
	{
		if (handle->data == NULL)
		{
			return;
		}

		uv_udp_server* server = (uv_udp_server*)handle->data;

		if (nread > 0)
		{
			server->service()->on_udpreceive((struct sockaddr_in*)addr, buf->base, nread);
		}
		else if (nread == 0)
		{

		}
		else
		{
			if (nread == UV_EOF) {

				fprintf(stdout, "udp client disconnected, close it.\n");
			}
			else if (nread == UV_ECONNRESET) {
				fprintf(stdout, "udp client disconnected unusually, close it.\n");
			}
			else
			{
				server->service()->error(nread);
			}
			server->close();
		}
	}
	void uv_udp_server::on_close(uv_handle_t* handle)
	{
		if (handle)
		{
			free(handle);
		}
	}
}
