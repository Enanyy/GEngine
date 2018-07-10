#include "uv_tcp_session.h"
#include "uv_net.h"
namespace network {

	uv_tcp_session::uv_tcp_session(int id) :
		m_id(id)
	{
		m_tcp = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		m_tcp->data = this;

		m_readbuf	= uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
		m_writebuf	= uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
	
	}

	uv_tcp_session:: ~uv_tcp_session()
	{
		if (uv_is_active((uv_handle_t*)m_tcp))
		{
			close();
		}

		free(m_readbuf.base);
		free(m_writebuf.base);
	
		free(m_tcp);

		m_readbuf.base = NULL;
		m_writebuf.base = NULL;
	
		m_tcp = NULL;
	}

	void uv_tcp_session::close()
	{
		if (uv_is_active((uv_handle_t*)m_tcp))
		{
			uv_read_stop((uv_stream_t*)m_tcp);
			uv_close((uv_handle_t*)m_tcp, on_close);
		}
	}

	void uv_tcp_session::on_close(uv_handle_t* handle)
	{
		if (handle != NULL) {
		
			free(handle);
		}
	}

}