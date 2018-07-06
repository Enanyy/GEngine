#include "uv_session.h"
#include "uv_net.h"
namespace uv
{
	uv_session::uv_session(int id) :
		m_id(id)
	{
		m_tcp = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		m_tcp->data = this;

		m_tcp_readbuf	= uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
		m_tcp_writebuf	= uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
	
	}

	uv_session:: ~uv_session()
	{
		close();

		free(m_tcp_readbuf.base);
		free(m_tcp_writebuf.base);
	
		free(m_tcp);

		m_tcp_readbuf.base = NULL;
		m_tcp_writebuf.base = NULL;
	
		m_tcp = NULL;
	}

	void uv_session::close()
	{
		if (uv_is_active((uv_handle_t*)m_tcp))
		{
			uv_read_stop((uv_stream_t*)m_tcp);
		}
	
		uv_close((uv_handle_t*)m_tcp, on_close);
		
	}

	void uv_session::on_close(uv_handle_t* handle)
	{
		if (handle != NULL) {
		
			uv_session* session = (uv_session*)handle->data;
			if (session != NULL) {
				delete session;
			}
		}
	}

}