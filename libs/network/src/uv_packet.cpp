#include "uv_packet.h"
#include "uv_service.h"
#include "uv_session.h"

namespace network
{
	uv_packet::uv_packet()
	{
		m_head = uv_buf_init((char*)malloc(PACKET_HEAD_LENGTH), PACKET_HEAD_LENGTH);
		m_head = uv_buf_init((char*)malloc(PACKET_BUFFER_SIZE), PACKET_BUFFER_SIZE);
	}

	uv_packet::~uv_packet()
	{
		SAFE_FREE(m_head.base);
		SAFE_FREE(m_body.base);

		m_head.len = 0;
		m_body.len = 0;
		m_length = 0;
	}

	void uv_packet::clear()
	{
		if (m_head.base != nullptr) {
			memset(m_head.base, 0, m_head.len);
		}
		if (m_body.base != nullptr) {
			memset(m_body.base, 0, m_body.len);
		}
	}

	int uv_packet::receive_head(uv_session* session)
	{
		session->packet().clear();

		int r = uv_read_start((uv_stream_t*)session->tcp(), on_alloc_head, on_receive_head);

		return r;
	}
	int uv_packet::receive_body(uv_session* session)
	{
		int r = uv_read_start((uv_stream_t*)session->tcp(), on_alloc_body, on_receive_body);

		return r;
	}

	void uv_packet::on_alloc_head(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		ASSERT(handle->data != nullptr);

		uv_session* session = (uv_session*)handle->data;

		*buf = session->packet().head();
	}

	void uv_packet::on_alloc_body(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		ASSERT(handle->data != nullptr);

		uv_session* session = (uv_session*)handle->data;

		uv_buf_t&  bodybuf = session->packet().body();

		SAFE_FREE(bodybuf.base);
		bodybuf.len = 0;

		if (session->packet().length() == uv_packet::PACKET_HEAD_LENGTH)
		{
			size_t bodylength = session->packet().bodylength();
			if (bodylength > 0)
			{
				bodybuf.base = (char*)malloc(bodylength);
				bodybuf.len = bodylength;

				*buf = bodybuf;
			}
		}
	}

	void uv_packet::on_receive_head(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		ASSERT(handle->data != nullptr);

		uv_session* session		= (uv_session*)handle->data;
		uv_service* service		= session->service();
		uv_packet& packet		= session->packet();

		if (nread == packet.head().len)
		{
			packet.length(nread);

			int bodylength = packet.bodylength();
			if (bodylength > 0)
			{
				receive_body(session);
			}
			else
			{
				std::string data;

				data.append(packet.head().base, packet.head().len);

				service->on_tcpreceive(session, data.c_str(), data.size());

				packet.clear();

				receive_head(session);
			}
		}
		else
		{
			packet.clear();

			if (nread < 0)
			{
				int id = session->id();

				service->closesession(id);

				if (nread == UV_EOF) {

					fprintf(stdout, "client %d disconnected, close it.\n", id);
				}
				else if (nread == UV_ECONNRESET) {
					fprintf(stdout, "client %d disconnected unusually, close it.\n", id);
				}
				else
				{
					ASSERT(nread >= 0);
				}
			}
		}
	}
	void uv_packet::on_receive_body(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		if (handle->data == nullptr)
		{
			return;
		}

		uv_session* session		= (uv_session*)handle->data;
		uv_service* service		= session->service();
		uv_packet& packet		= session->packet();

		if (nread > 0 && nread == session->packet().bodylength())
		{

			packet.length(packet.length() + packet.bodylength());
			
			std::string data;

			data.append(packet.head().base, packet.head().len);

			data.append(packet.body().base, packet.bodylength());

			service->on_tcpreceive(session, data.c_str(), data.size());

			packet.clear();

			receive_head(session);
		}

		else
		{
			packet.clear();

			if (nread < 0)
			{
				int id = session->id();

				service->closesession(id);

				if (nread == UV_EOF) {

					fprintf(stdout, "client %d disconnected, close it.\n", id);
				}
				else if (nread == UV_ECONNRESET) {
					fprintf(stdout, "client %d disconnected unusually, close it.\n", id);
				}
				else
				{
					ASSERT(nread >= 0);
				}
			}
		}
	}
}