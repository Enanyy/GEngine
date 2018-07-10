#pragma once
#ifndef _UV_TCP_SESSION_H_
#define _UV_TCP_SESSION_H_
#include "uv_service.h"
namespace network {

	class uv_service;
	class uv_tcp_session
	{
	public:
		uv_tcp_session(int id);
		virtual ~uv_tcp_session();


		const int						id() const { return m_id; }

		uv_service*						service()const { return m_service; }
		void							service(uv_service* service) { m_service = service; }

		/*
		 *The handle of tcp.
		*/
		uv_tcp_t*						tcp()const { return m_tcp; }
		uv_buf_t&						readbuf() { return m_readbuf; }
		uv_buf_t&						writebuf() { return m_writebuf; }
		uv_write_t&						write() { return m_write; }


		void close();

	private:
		static void on_close(uv_handle_t* handle);

	private:
		int					m_id;
		uv_service*			m_service;
		uv_tcp_t*			m_tcp;

		uv_buf_t			m_readbuf;
		uv_buf_t			m_writebuf;

		uv_write_t			m_write;
	};
}
#endif //_UV_TCP_SESSION_H_