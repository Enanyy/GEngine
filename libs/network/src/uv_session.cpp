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
		return uv_is_active((const uv_handle_t*)&m_tcp);
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