#include "uv_session.h"
#include "uv_net.h"
namespace network {

	uv_session::uv_session(uv_service* service, bool ipv6) :
		m_service(service),
		m_tcp(),
		m_id(service->generateid()),
		m_ipv6(ipv6)
	{
		
		m_tcp.data = this;

		m_readbuf	= uv_buf_init((char*)malloc(PACKET_BUFFER_SIZE), PACKET_BUFFER_SIZE);
		m_writebuf	= uv_buf_init((char*)malloc(PACKET_BUFFER_SIZE), PACKET_BUFFER_SIZE);
	
	}

	uv_session:: ~uv_session()
	{
		if (is_active())
		{
			close();
		}

		SAFE_FREE(m_readbuf.base);
		SAFE_FREE(m_writebuf.base);

	
	}

	const unsigned long	uv_session::ip() const
	{
		if (m_ipv6)
		{
			sockaddr_in6 addr6;
			int namelen = sizeof(sockaddr_in6);
			int r = uv_tcp_getsockname(&m_tcp, (sockaddr*)&addr6, &namelen);
			if (r==0)
			{
				return *((unsigned long*)addr6.sin6_addr.u.Byte);
			}
		}
		else
		{
			sockaddr_in addr4;
			int namelen = sizeof(sockaddr_in);
			int r = uv_tcp_getsockname(&m_tcp, (sockaddr*)&addr4, &namelen);
			if (r == 0)
			{
				return addr4.sin_addr.S_un.S_addr;
			}
		}
		return 0;
	}
	const unsigned short uv_session::port()const
	{
		if (m_ipv6)
		{
			sockaddr_in6 addr6;
			int namelen = sizeof(sockaddr_in6);
			int r = uv_tcp_getsockname(&m_tcp, (sockaddr*)&addr6, &namelen);
			if (r == 0)
			{
				return addr6.sin6_port;
			}
		}
		else
		{
			sockaddr_in addr4;
			int namelen = sizeof(sockaddr_in);
			int r = uv_tcp_getsockname(&m_tcp, (sockaddr*)&addr4, &namelen);
			if (r == 0)
			{
				return addr4.sin_port;
			}
		}
		return 0;
	}

	bool uv_session::no_delay(bool enable)
	{
		int r = uv_tcp_nodelay(&m_tcp, enable ? 1 : 0);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}

	bool uv_session::keep_alive(int enable, unsigned int delay)
	{
		int r = uv_tcp_keepalive(&m_tcp, enable, delay);
		if (r != 0)
		{
			ASSERT(r == 0);
			return false;
		}
		return true;
	}
	bool uv_session::is_active()const
	{
		return uv_is_active((const uv_handle_t*)&m_tcp) != 0;
	}

	void uv_session::close()
	{
		if (is_active())
		{
			uv_read_stop((uv_stream_t*)&m_tcp);
			uv_close((uv_handle_t*)&m_tcp, on_close);
		}
	}

	void uv_session::on_close(uv_handle_t* handle)
	{
		SAFE_FREE(handle);
		
	}
}