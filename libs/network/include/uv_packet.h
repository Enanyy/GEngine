#pragma once
#ifndef _UV_PACKET_H_
#define _UV_PACKET_H_

#include "uv_net.h"

namespace network {

#define PACKET_BUFFER_SIZE (1024)

	class uv_service;
	class uv_session;
	class uv_packet
	{

	public:

		static const unsigned int PACKET_HEAD_LENGTH = 24;			//包头长度
		static const unsigned int PACKET_BODY_LENGTH_OFFSET = 16;	//包体长度在包头中的偏移	

		uv_packet();
		

		virtual ~uv_packet();
	

		void  clear();
		

		/*
		recv data length
		*/
		const size_t length()const { return m_length; }
		void		 length(const size_t len) { m_length = len; }

		const size_t bodylength()const { return *(unsigned int*)(m_head.base + PACKET_HEAD_LENGTH); }


		uv_buf_t& head() { return m_head; }
		uv_buf_t& body() { return m_body; }


		int receive(uv_session* session)
		{
			return receive_head(session);
		}

	private:


		static int receive_head(uv_session* session);
		static int receive_body(uv_session* session);
		static void on_alloc_head(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void on_alloc_body(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void on_receive_head(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
		static void on_receive_body(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);


	private:
		uv_buf_t m_head;
		uv_buf_t m_body;

		size_t	 m_length;			//recv bodylength

	};


}
#endif // !_UV_PACKET_H_
