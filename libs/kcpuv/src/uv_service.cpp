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
		m_tcp(this),
		m_udp(this),
		m_error(),
		m_shutdown(false),
		m_init(false)
	{
	}

	uv_service::~uv_service()
	{
		uv_loop_close(m_loop);
		m_handler = NULL;
		m_shutdown = true;
		m_init = false;
	}

	bool uv_service::initialize(const char* ip, const int tcp_port, const int udp_port, bool ipv6 = false)
	{
		if (m_init)
		{
			return true;
		}
		if (m_tcp.initialize(ip, tcp_port, ipv6) == false)
		{
			return false;
		}
		if (m_udp.initialize(ip, udp_port, ipv6) == false)
		{
			return false;
		}

		m_init = true;

		return true;
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
			//��uv_sessnio::on_close��delete
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

	void uv_service::on_newsession(uv_session* session)
	{
		if (session == NULL)
		{
			return;
		}

		if (m_sessions.find(session->id()) == m_sessions.end())
		{
			m_sessions.insert(std::make_pair(session->id(), session));
		}
		else
		{
			
		}

		if (m_handler != NULL)
		{
			m_handler->on_newsession(session);
		}
	}

	void uv_service::on_receive(uv_session* session, char* data, size_t length)
	{
		if (m_handler)
		{
			m_handler->on_receive(session, data, length);
		}
	}

	void uv_service::on_receive(sockaddr_in* addr, char* data, size_t length)
	{
		if (m_handler)
		{
			m_handler->on_receive(addr, data, length);
		}
	}

	void uv_service::error(int status)
	{
		m_error = uv_strerror(status);

		printf("%s.\n", m_error.c_str());
	}

}