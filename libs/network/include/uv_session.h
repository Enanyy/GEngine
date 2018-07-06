#ifndef _UV_SESSION_H_
#define _UV_SESSION_H_
#include "uv_service.h"
namespace uv
{
	class uv_session
	{
	public:
		uv_session(int id);
		virtual ~uv_session();

		
		const int						id() const						{ return m_id; }

		uv_service*						service()const					{ return m_service; }
		void							service(uv_service* service)	{ m_service = service; }

		/*
		 *The handle of tcp.
		*/
		uv_tcp_t*						tcp()const						{ return m_tcp; }	
		uv_buf_t&						tcp_readbuf()					{ return m_tcp_readbuf; }
		uv_buf_t&						tcp_writebuf()					{ return m_tcp_writebuf; }
		uv_write_t&						tcp_write()						{ return m_tcp_write; }
		
	
		void close();

	private:
		static void on_close(uv_handle_t* handle);

	private:
		int					m_id;
		uv_service*			m_service;
		uv_tcp_t*			m_tcp;

		uv_buf_t			m_tcp_readbuf;
		uv_buf_t			m_tcp_writebuf;
	
		uv_write_t			m_tcp_write;
	};
}
#endif //_UV_SESSION_H_