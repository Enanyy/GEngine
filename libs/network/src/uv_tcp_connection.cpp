#include "uv_tcp_connection.h"

namespace network
{
	uv_tcp_connection::uv_tcp_connection(uv_service* service):
		m_service(service),
		m_session(NULL),
		m_ipv6(false),
		m_connectcallback(NULL),
		m_status(-1)
	{
		m_session = new uv_tcp_session(m_service->generateid());
	}
	uv_tcp_connection:: ~uv_tcp_connection()
	{
		close();
	}

	void uv_tcp_connection::close()
	{
		m_status = -1;
		m_ipv6 = false;
		m_connectcallback = NULL;
		if (m_session)
		{
			if (uv_is_active((uv_handle_t*)m_session->tcp()))
			{
				uv_close((uv_handle_t*)m_session->tcp(), on_close);
			}

			m_session->close();
			delete m_session;
		}
	}


	bool uv_tcp_connection::connect(const std::string& ip, const int port, bool ipv6,connectcallback callback )
	{
		if (m_service == NULL || m_session == NULL)
		{
			if (callback != NULL)
			{
				callback(this, false);
			}
			return false;
		}
		if (is_connect())
		{
			if (callback != NULL)
			{
				callback(this, true);
			}
			return true;
		}
		
		m_connectcallback = callback;
		m_ipv6 = ipv6;

		int r = uv_tcp_init(m_service->loop(), m_session->tcp());
		ASSERT(r == 0);

		struct sockaddr* addr = NULL;
		if (m_ipv6)
		{
			struct sockaddr_in addr4;
			 r = uv_ip4_addr(ip.c_str(), port, &addr4);
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
			r = uv_ip6_addr(ip.c_str(), port, &addr6);
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

	void uv_tcp_connection::disconnect()
	{
		if (uv_is_active((uv_handle_t*)m_session->tcp()))
		{
			uv_close((uv_handle_t*)m_session->tcp(), on_close);
		}
		m_status = false;
	}


	bool uv_tcp_connection::no_delay(bool enable)
	{
		int r = uv_tcp_nodelay(m_session->tcp(), enable ? 1 : 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	bool uv_tcp_connection::keep_alive(int enable, unsigned int delay)
	{
		int r = uv_tcp_keepalive(m_session->tcp(), enable, delay);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	void uv_tcp_connection::send(const char* data, const size_t length)
	{
		if (is_connect()==false)
		{
			return;
		}

		if (m_service == NULL || m_session == NULL)
		{
			return;
		}

		if (m_session->writebuf().len < length)
		{
			//重新分配内存
			m_session->writebuf().base = (char*)realloc(m_session->writebuf().base, length);
			m_session->writebuf().len = length;
		}

		memset(m_session->writebuf().base, 0, m_session->writebuf().len);

		memcpy(m_session->writebuf().base, data, length);
		m_session->writebuf().len = length;

		int  r = uv_write(&m_session->write(), (uv_stream_t*)m_session->tcp(), &m_session->writebuf(), 1, on_send);

		if (r != 0)
		{
			ASSERT(r == 0);
		}
	}

	
	void uv_tcp_connection::on_connect(uv_connect_t* req, int status)
	{
		if (req->data == NULL)
		{
			return;
		}
		uv_tcp_connection* connection = (uv_tcp_connection*)req->data;
		connection->m_status = status;

		if (status == 0)
		{
			int r = uv_read_start((uv_stream_t*)connection->session()->tcp(), on_alloc_buffer, on_receive);
			if (r != 0)
			{
				ASSERT(r == 0);
			}				
		}
		else
		{
			ASSERT(status == 0);
		}	

		if (connection->m_connectcallback != NULL)
		{
			connection->m_connectcallback(connection, status==0);
		}
	}

	void uv_tcp_connection::on_receive(uv_stream_t* req, ssize_t nread, const uv_buf_t* buf)
	{
		if (req->data == NULL)
		{
			return;
		}

		uv_tcp_session* sesseion = (uv_tcp_session*)req->data;

		if (nread > 0)
		{
			sesseion->service()->on_tcpreceive(sesseion, buf->base, nread);
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

	void uv_tcp_connection::on_send(uv_write_t* req, int status)
	{
		if (status < 0)
		{
			printf(uv_strerror(status));
		}

		ASSERT(status >= 0);
	}

	void uv_tcp_connection::on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{	
		assert(handle->data != nullptr);

		uv_tcp_session *session = (uv_tcp_session *)handle->data;

		*buf = session->readbuf();
	}

	void uv_tcp_connection::on_close(uv_handle_t* handle)
	{
		if (handle)
		{
			free(handle);
		}
		printf("tcp client close callback.\n");
	}
}