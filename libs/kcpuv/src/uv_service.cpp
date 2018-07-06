#include "uv_service.h"
namespace uv
{
	uv_service_handler::uv_service_handler()
	{
	}

	uv_service_handler::~uv_service_handler()
	{
	}


	uv_service::uv_service(uv_service_handler* handler) :
		m_handler(handler),
		m_loop(uv_default_loop()),
		m_sessions(),
		m_error(),
		m_shutdown(false)
	{
	}

	uv_service::~uv_service()
	{
		uv_loop_close(m_loop);
		m_handler = NULL;
		
	}

	void uv_service::shutdown()
	{
		m_shutdown = true;
	}

	bool uv_service::run() const
	{
		if (m_loop == NULL)
		{
			return false;
		}
		int r = 0;
		while (m_shutdown == false)
		{
			r = uv_run(m_loop, UV_RUN_NOWAIT);
		}

		return true;
	}

	void uv_service::close(int id)
	{
		auto s = session(id);
		if (s)
		{
			s->close();
			m_sessions.erase(id);
			//ÔÚuv_sessnio::on_closeÔÙdelete
			//delete s;
		}
	}

	uv_session* uv_service::session(int id)
	{
		auto it = m_sessions.begin();
		for (; it != m_sessions.end(); ++it)
		{
			if (it->second->id() == id)
			{
				return it->second;
			}
		}
		return NULL;
	}

	void uv_service::on_connect(uv_session* session)
	{
		if (session == NULL)
		{
			return;
		}

		m_sessions.insert(std::make_pair(session->id(), session));

		if (m_handler != NULL)
		{
			m_handler->on_connect(session);
		}
	}

	void uv_service::error(int status)
	{
		m_error = uv_strerror(status);

		printf("%s.\n", m_error.c_str());
	}

}