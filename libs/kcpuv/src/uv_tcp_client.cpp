#include "uv_tcp_client.h"

namespace uv
{
	uv_tcp_client::uv_tcp_client(uv_loop_t* loop /*= uv_default_loop()*/):
		m_loop(loop),
		m_init(false)
	{
		
	}
	uv_tcp_client:: ~uv_tcp_client()
	{
		close();
		printf("tcp client exit.\n");
	}

	bool uv_tcp_client::start(const char* ip, const unsigned port)
	{
		if (m_init)
		{
			return true;
		}
		close();

		if (init() == false)
		{
			printf("tcp client init fail.\n");
			return false;
		}

		if (connect(ip, port) == false)
		{
			printf("tcp client connect %s:%d fail.\n", ip, port);
			return false;
		}

		if (run() == false)
		{
			printf("tcp client run fail.\n");
		}
		

		return true;
	}

	void uv_tcp_client::close()
	{
		if (m_init)
		{
			uv_close((uv_handle_t*)&m_socket, on_close);
			uv_loop_close(m_loop);
		}
		m_init = false;

		free(m_write_buffer.base);
		free(m_read_buffer.base);

		m_write_buffer.base = nullptr;
		m_read_buffer.base = nullptr;

		m_write_buffer.len = 0;
		m_read_buffer.len = 0;
	}

	bool uv_tcp_client::init()
	{
		if (m_init)
		{
			return true;
		}

		if (!m_loop)
		{
			return false;
		}

		int r = uv_tcp_init(m_loop, &m_socket);
		if (r != 0)
		{
			error(r);
			return false;
		}

		m_socket.data = this;

		m_write_buffer = uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);
		m_read_buffer = uv_buf_init((char*)malloc(BUFFER_SIZE), BUFFER_SIZE);

		m_init = true;

		return true;
	}
	bool uv_tcp_client::connect(const char* ip, const unsigned port)
	{
		struct sockaddr_in addr;
		int r = uv_ip4_addr(ip, port, &addr);
		if (r != 0)
		{
			error(r);
			return false;
		}
		
		m_connect_req.data = this;

		r = uv_tcp_connect(&m_connect_req, &m_socket, (const struct sockaddr*)&addr, on_connect);
		if (r != 0)
		{
			error(r);
			return false;
		}
		return true;
	}

	bool uv_tcp_client::run()
	{
		int r = uv_run(m_loop, UV_RUN_DEFAULT);
		if (r != 0)
		{
			error(r);

			return false;
		}
		return true;
	}

	bool uv_tcp_client::set_no_delay(bool enable)
	{
		int r = uv_tcp_nodelay(&m_socket, enable ? 1 : 0);
		if (r != 0)
		{
			error(r);
			return false;
		}
		return true;
	}

	bool uv_tcp_client::set_keep_alive(int enable, unsigned int delay)
	{
		int r = uv_tcp_keepalive(&m_socket, enable, delay);
		if (r != 0)
		{
			error(r);
			return false;
		}
		return true;
	}

	void uv_tcp_client::send(const char* data, const size_t length)
	{
		if (m_write_buffer.len < length)
		{
			//重新分配内存
			m_write_buffer.base = (char*)realloc(m_write_buffer.base, length);
			m_write_buffer.len = length;
		}
		else
		{
			memset(m_write_buffer.base, 0, m_write_buffer.len);
		}

		memcpy(m_write_buffer.base, data, length);
		m_write_buffer.len = length;

		int  r = uv_write(&m_write_req, (uv_stream_t*)&m_socket, &m_write_buffer, 1, on_send);

		if (r != 0)
		{
			error(r);
		}
	}

	void uv_tcp_client::error(int status)
	{
		m_error = uv_strerror(status);
		fprintf(stderr, "%s\n", m_error.c_str());
	}

	void uv_tcp_client::on_connect(uv_connect_t* req, int status)
	{
		if (req->data == nullptr)
		{
			return;
		}
		uv_tcp_client* client = (uv_tcp_client*)req->data;
		if (status == 0)
		{
			int r = uv_read_start((uv_stream_t*)&client->m_socket, on_alloc_buffer, on_receive);
			if (r != 0)
			{
				client->error(r);
			}
		}
		else
		{
			client->error(status);
		}

		if (client->m_connect_callback != nullptr)
		{
			client->m_connect_callback(status);
		}
	}

	void uv_tcp_client::on_receive(uv_stream_t* req, ssize_t nread, const uv_buf_t* buf)
	{
		if (req->data == nullptr)
		{
			return;
		}

		uv_tcp_client* client = (uv_tcp_client*)req->data;

		if (nread > 0)
		{
			if (client->m_receive_callback!=nullptr)
			{
				client->m_receive_callback(buf->base, buf->len);
			}
		}
		else if (nread == 0)
		{
			/* Everything OK, but nothing read. */
		}
		else
		{
			if (nread == UV_EOF) {

				fprintf(stdout, "tcp client disconnected, close it.\n");
			}
			else if (nread == UV_ECONNRESET) {
				fprintf(stdout, "tcp client disconnected unusually, close it.\n");
			}
			else
			{
				client->error(nread);
			}
			
			client->close();
		}
	}

	void uv_tcp_client::on_send(uv_write_t* req, int status)
	{
		if (status < 0)
		{
			printf(uv_strerror(status));
		}

	}

	void uv_tcp_client::on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{	
		assert(handle->data != nullptr);

		uv_tcp_client *client = (uv_tcp_client *)handle->data;

		*buf = client->read_buffer();
	}

	void uv_tcp_client::on_close(uv_handle_t* handle)
	{
		if (handle)
		{
			free(handle);
		}
		printf("tcp client close callback.\n");
	}
}