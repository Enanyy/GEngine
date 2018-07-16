#include "uv_packet.h"

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

	int uv_packet::receive(uv_session* session)
	{
		return receive_head(session);
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

		if (session->packet().length() == uv_packet::PACKET_HEAD_LENGTH)
		{
			size_t bodylength = session->packet().bodylength();
			if (bodylength > 0)
			{
				session->packet().body().base = (char*)realloc(session->packet().body().base, bodylength);
				session->packet().body().len = bodylength;

				*buf = session->packet().body();
			}
		}
	}

	void uv_packet::on_receive_head(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		ASSERT(handle->data != nullptr);

		uv_session* session = (uv_session*)handle->data;
		auto service = session->service();

		if (nread == session->packet().head().len)
		{
			session->packet().length(nread);
			receive_body(session);
		}
		else
		{
			session->packet().clear();

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

		uv_session* session = (uv_session*)handle->data;

		if (session == nullptr || session->service() == nullptr)
		{
			return;
		}
		auto service = session->service();

		if (nread == session->packet().bodylength())
		{
			session->packet().length(session->packet().length() + session->packet().bodylength());
			std::string packet;
			packet.append(session->packet().head().base, session->packet().head().len);
			packet.append(session->packet().body().base, session->packet().bodylength());

			service->on_tcpreceive(session, packet.c_str(), packet.size());

			session->packet().clear();

			receive_head(session);
		}

		else
		{
			session->packet().clear();

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