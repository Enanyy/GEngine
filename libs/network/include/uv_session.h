#pragma once
#ifndef _UV_TCP_SESSION_H_
#define _UV_TCP_SESSION_H_

#include "uv_service.h"
#include "uv_packet.h"

namespace network {

	class uv_service;
	class uv_session;
	class uv_packet;

	class uv_session
	{
		
	public:
	
		uv_session(uv_service* service, bool ipv6);
		virtual ~uv_session();


		const int						id() const { return m_id; }
		const unsigned long				ip()const;
		const unsigned short			port()const;

		uv_service*						service()const { return m_service; }
		
		/*
		 *The handle of tcp.
		*/
		uv_tcp_t*				tcp() { return &m_tcp; }
		uv_buf_t&				readbuf() { return m_readbuf; }
		uv_buf_t&				writebuf() { return m_writebuf; }
		uv_write_t&				write() { return m_write; }
		uv_packet&				packet() { return m_packet; }

		bool					setnodelay(bool enable);
		bool					setkeepalive(int enable, unsigned int delay);

		bool					is_active()const;
		bool					is_ipv6()const { return m_ipv6; }

		void					close();

		int						receive();


	private:
		static void on_close(uv_handle_t* handle);
	

	
	private:
		int					m_id;
		uv_service*			m_service;
		uv_tcp_t			m_tcp;

		uv_packet			m_packet;

		uv_buf_t			m_readbuf;
		uv_buf_t			m_writebuf;

		uv_write_t			m_write;

		bool				m_ipv6;

	};
}
#endif //_UV_TCP_SESSION_H_