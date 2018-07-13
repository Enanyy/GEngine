#include "uv_service.h"

namespace network {

	uv_service::uv_service(uv_service_handler* handler) :
		m_handler(handler),
		m_loop(uv_default_loop()),
		m_tcp(NULL),
		m_udp(NULL),
		m_connections(),
		m_sessions(),
		m_error(),
		m_shutdown(false),
		m_init(false)
	{
		
		m_handler->m_service = this;
	}

	uv_service::~uv_service()
	{
		uv_loop_close(m_loop);
		m_handler = NULL;
		m_shutdown = true;
		m_init = false;

		m_tcp->close();
		m_udp->close();
	
		SAFE_DELETE(m_tcp);
		SAFE_DELETE(m_udp);

	

		for (auto it = m_sessions.begin(); it != m_sessions.end(); ++it)
		{
			auto session = it->second;
			session->close();
			SAFE_DELETE(session);
		}

		for (auto it = m_connections.begin(); it != m_connections.end(); ++it)
		{
			auto con = it->second; 
			con->close();
			SAFE_DELETE(con);
		}

		m_connections.clear();
	}

	bool uv_service::initialize(const std::string&  ip, const int tcp_port, const int udp_port, bool ipv6)
	{
		if (m_init)
		{
			return true;
		}
		if (m_handler == NULL)
		{
			return false;
		}

		ASSERT(m_handler->initialize());

		m_tcp = new uv_tcp_server(this);
		m_udp = new uv_udp_server(this);

		ASSERT(m_tcp->initialize(ip, tcp_port, ipv6));
		ASSERT(m_udp->initialize(ip, udp_port, ipv6));
				
		m_init = true;

		return true;
	}

	void uv_service::shutdown()
	{
		if (m_handler)
		{
			m_handler->shutdown();
		}

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

			if (m_handler)
			{
				m_handler->update();
			}
		}

		return true;
	}

	void uv_service::closesession(int id)
	{
		auto session = getsession(id);
		if (session)
		{
			session->close();

			m_sessions.erase(id);
			
			if (m_handler)
			{
				m_handler->on_closesession(session);
			}

			SAFE_DELETE(session);
		}
		
	}


	uv_session* uv_service::getsession(int id)
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
		printf("newsession = %d\n", session->id());
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

	void uv_service::on_tcpreceive(uv_session* session, char* data, size_t length)
	{
		if (m_handler)
		{
			m_handler->on_tcpreceive(session, data, length);
		}
	}

	void uv_service::on_udpreceive(sockaddr_in* addr, char* data, size_t length)
	{
		if (m_handler)
		{
			m_handler->on_udpreceive(addr, data, length);
		}
	}

	void uv_service::error(int status)
	{
		m_error = uv_strerror(status);

		printf("%s.\n", m_error.c_str());
	}

	bool uv_service::connect(const std::string& ip, const int port, bool ipv6 )
	{
		
		uv_session*connection = new uv_session(this,ipv6);

		int r = uv_tcp_init(m_loop, connection->tcp());
		ASSERT(r == 0);

		struct sockaddr* addr = NULL;
		if (ipv6)
		{
			struct sockaddr_in addr4;
			r = uv_ip4_addr(ip.c_str(), port, &addr4);
			if (r != 0)
			{
				connection->close();
				SAFE_DELETE(connection);

				ASSERT(r == 0);
				return false;
			}
			addr = (struct sockaddr*)&addr4;
		}
		else
		{
			struct sockaddr_in6 addr6;
			r = uv_ip6_addr(ip.c_str(), port, &addr6);
			if (r != 0)
			{
				connection->close();
				SAFE_DELETE(connection);
			

				ASSERT(r == 0);
				return false;
			}
			addr = (struct sockaddr*)&addr6;
		}

		m_connect.data = connection;


		r = uv_tcp_connect(&m_connect, connection->tcp(), (const struct sockaddr*)&addr, on_connect);
		if (r != 0)
		{
			connection->close();
			SAFE_DELETE(connection);


			ASSERT(r == 0);
			return false;
		}

		return true;
	}

	void uv_service::on_connect(uv_connect_t* req, int status)
	{
		if (req->data == NULL)
		{
			return;
		}
		uv_session* connection = (uv_session*)req->data;
		auto service = connection->service();


		if (status == 0)
		{
			//连接成功
			service->on_newconnection(connection);

			int r = uv_read_start((uv_stream_t*)connection->tcp(),

				[](uv_handle_t* hanle, size_t suggested_size, uv_buf_t* buf) {

					assert(hanle->data != NULL);

					uv_session* connection = (uv_session*)hanle->data;

					*buf = connection->readbuf();
				},

				[](uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {

					if (handle->data == NULL)
					{
						return;
					}

					uv_session* connection = (uv_session*)handle->data;

					if (connection == NULL || connection->service() == NULL)
					{
						return;
					}
					auto service = connection->service();

					if (nread > 0)
					{
						service->on_tcpreceive(connection, buf->base, nread);
					}
					else if (nread == 0)
					{
						/* Everything OK, but nothing read. */
					}
					else
					{
						service->closeconnection(connection->id());

						if (nread == UV_EOF) {

							fprintf(stdout, "client %d disconnected, close it.\n", connection->id());
						}
						else if (nread == UV_ECONNRESET) {
							fprintf(stdout, "client %d disconnected unusually, close it.\n", connection->id());
						}
						else
						{
							ASSERT(nread >= 0);
						}
					}
				});


			if (r != 0)
			{
				service->closeconnection(connection->id());

				ASSERT(r == 0);
			}
		}
		else
		{
			connection->close();
			SAFE_DELETE(connection);

			
			ASSERT(status == 0);
		}

	}

	uv_session* uv_service::getconnection(int id)
	{
		auto it = m_connections.find(id);

		if (it != m_connections.end())
		{
			return it->second;
		}
		return NULL;
	}

	uv_session* uv_service::getconnection(const std::string&  ip, const int port)
	{
		struct sockaddr_in6 addr6;
		int r = uv_ip6_addr(ip.c_str(), port, &addr6);
		if (r != 0)
		{
			return NULL;
		}
		
		struct sockaddr_in addr4;
		r = uv_ip4_addr(ip.c_str(), port, &addr4);
		if (r != 0)
		{
			return NULL;
		}

		auto it = m_connections.begin();
		for (; it != m_connections.end(); ++it)
		{
			auto connection = it->second;
			if (connection->is_ipv6())
			{
				struct sockaddr_in6 addr;
				int length = sizeof(sockaddr_in6);
				int r = uv_tcp_getsockname(connection->tcp(), (sockaddr*)&addr, &length);
				if (r == 0)
				{
					if ( (*(unsigned long*)addr6.sin6_addr.u.Byte) == (*(unsigned long*)addr.sin6_addr.u.Byte) 
						 && addr6.sin6_port == addr.sin6_port)
					{
						return connection;
					}
				}			
			}
			else
			{
				struct sockaddr_in addr;
				int length = sizeof(sockaddr_in);
				int r = uv_tcp_getsockname(connection->tcp(), (sockaddr*)&addr, &length);
				if (r == 0)
				{
					if (addr4.sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr 
						&& addr4.sin_port == addr.sin_port)
					{
						return connection;
					}
				}			
			}
		}

		return NULL;
	}

	void uv_service::closeconnection(int id)
	{
		auto connection = getconnection(id);
		if (connection)
		{
			connection->close();
			m_connections.erase(id);

			if (m_handler)
			{
				m_handler->on_closeconnection(connection);
			}

			SAFE_DELETE(connection);

		}
	}

	void uv_service::on_newconnection(uv_session* connection)
	{
		if (connection == NULL)
		{
			return;
		}

		if (m_connections.find(connection->id()) == m_connections.end())
		{
			m_connections.insert(std::make_pair(connection->id(), connection));
		}

		if (m_handler)
		{
			m_handler->on_newconnection(connection);
		}
	}
}