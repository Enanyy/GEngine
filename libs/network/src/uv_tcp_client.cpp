#include "uv_tcp_client.h"

namespace network
{
	uv_tcp_client::uv_tcp_client(uv_service* service):
		m_service(service),
		m_session(NULL),
		m_init(false),
		m_ipv6(false),
		m_connect(false)
	{
		
	}
	uv_tcp_client:: ~uv_tcp_client()
	{
		close();
	}

	bool uv_tcp_client::initialize()
	{
		if (m_init)
		{
			return true;
		}
		if (m_service == NULL)
		{
			ASSERT(m_service != NULL);
			return false;
		}

		m_session = new uv_session(m_service->sessionid());

		int r = uv_tcp_init(m_service->loop(), m_session->tcp());
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}

		m_session->tcp()->data = this;

		
		m_init = true;

		return true;
	}

	void uv_tcp_client::close()
	{
		uv_close((uv_handle_t*)m_session->tcp(), on_close);
		m_connect = false;
		m_init = false;
		m_ipv6 = false;

		if (m_session)
		{
			m_session->close();
			delete m_session;
		}
	}


	bool uv_tcp_client::connect(const char* ip, const int port, bool ipv6 )
	{
		if (m_session == NULL)
		{
			return false;
		}

		if (m_init == false)
		{
			return false;
		}
		
		m_ipv6 = ipv6;

		struct sockaddr* addr = NULL;
		if (m_ipv6)
		{
			struct sockaddr_in addr4;
			int r = uv_ip4_addr(ip, port, &addr4);
			if (r != 0)
			{
				ASSERT(r == 0);
				return false;
			}
			addr = (struct sockaddr*)&addr4;
		}
		else
		{
			struct sockaddr_in6 addr6;
			int r = uv_ip6_addr(ip, port, &addr6);
			if (r != 0)
			{
				ASSERT(r == 0);
				return false;
			}
			addr = (struct sockaddr*)&addr6;
		}

		if (addr == NULL)
		{
			return false;
		}
		
		m_tcp_connect.data = this;

		int r = uv_tcp_connect(&m_tcp_connect, m_session->tcp(), (const struct sockaddr*)&addr, on_connect);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	void uv_tcp_client::disconnect()
	{
		uv_close((uv_handle_t*)m_session->tcp(), on_close);
		m_connect = false;
	}


	bool uv_tcp_client::no_delay(bool enable)
	{
		int r = uv_tcp_nodelay(m_session->tcp(), enable ? 1 : 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	bool uv_tcp_client::keep_alive(int enable, unsigned int delay)
	{
		int r = uv_tcp_keepalive(m_session->tcp(), enable, delay);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	void uv_tcp_client::send(const char* data, const size_t length)
	{
		if (m_init == false || m_connect == false)
		{
			return;
		}


		if (m_service == NULL || m_session == NULL)
		{
			return;
		}

		if (m_session->tcp_writebuf().len < length)
		{
			//重新分配内存
			m_session->tcp_writebuf().base = (char*)realloc(m_session->tcp_writebuf().base, length);
			m_session->tcp_writebuf().len = length;
		}

		memset(m_session->tcp_writebuf().base, 0, m_session->tcp_writebuf().len);

		memcpy(m_session->tcp_writebuf().base, data, length);
		m_session->tcp_writebuf().len = length;

		int  r = uv_write(&m_session->tcp_write(), (uv_stream_t*)m_session->tcp(), &m_session->tcp_writebuf(), 1, on_send);

		if (r != 0)
		{
			ASSERT(r == 0);
		}
	}

	
	void uv_tcp_client::on_connect(uv_connect_t* req, int status)
	{
		if (req->data == NULL)
		{
			return;
		}
		uv_tcp_client* client = (uv_tcp_client*)req->data;
		if (status == 0)
		{
			int r = uv_read_start((uv_stream_t*)client->session()->tcp(), on_alloc_buffer, on_receive);
			if (r != 0)
			{
				ASSERT(r == 0);
			}

			client->m_connect = true;
		}
		else
		{
			ASSERT(status == 0);
		}	
	}

	void uv_tcp_client::on_receive(uv_stream_t* req, ssize_t nread, const uv_buf_t* buf)
	{
		if (req->data == NULL)
		{
			return;
		}

		uv_session* sesseion = (uv_session*)req->data;

		if (nread > 0)
		{
			sesseion->service()->on_tcp_receive(sesseion, buf->base, nread);
		}
		else if (nread == 0)
		{
			/* Everything OK, but nothing read. */
		}
		else
		{
			if (nread == UV_EOF) {

				fprintf(stdout, "tcp client disconnected, close it.\n");
			}
			else if (nread == UV_ECONNRESET) {
				fprintf(stdout, "tcp client disconnected unusually, close it.\n");
			}
			else
			{
				ASSERT(nread >= 0);
			}
			
			sesseion->close();
		}
	}

	void uv_tcp_client::on_send(uv_write_t* req, int status)
	{
		if (status < 0)
		{
			printf(uv_strerror(status));
		}

		ASSERT(status >= 0);
	}

	void uv_tcp_client::on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{	
		assert(handle->data != nullptr);

		uv_tcp_client *client = (uv_tcp_client *)handle->data;

		*buf = client->session()->tcp_readbuf();
	}

	void uv_tcp_client::on_close(uv_handle_t* handle)
	{
		if (handle)
		{
			free(handle);
		}
		printf("tcp client close callback.\n");
	}
}