#pragma once
#ifndef _UV_NET_H_
#define _UV_NET_H_

#include "uv.h"
#include <stdint.h>
#include <string>
#if defined(WIN32) || defined(_WIN32)|| defined(_WIN64) 
#ifdef _MSC_VER

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Userenv.lib")

#pragma comment(lib,"libuv.lib")

#endif // _MSC_VER
#else
#include <sys/types.h>
#include <netinet/in.h>
#endif
namespace network {

#define ADDRESS_BUFFER_SIZE 24
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete p;p = nullptr;}}			
#endif // !SAFE_DELETE
#ifndef SAFE_FREE
#define SAFE_FREE(p)   {if(p){free(p);p = nullptr;}}
#endif // !SAFE_FREE(p)



	
	/* Die with fatal error. */
#define FATAL(msg)                                        \
  do {                                                    \
    fprintf(stderr,                                       \
            "Fatal error in %s on line %d: %s\n",         \
            __FILE__,                                     \
            __LINE__,                                     \
            msg);                                         \
    fflush(stderr);                                       \
    abort();                                              \
  } while (0)

/* Have our own assert, so we are sure it does not get optimized away in
* a release build.
*/
#define ASSERT(expr)                                      \
 do {                                                     \
  if (!(expr)) {                                          \
    fprintf(stderr,                                       \
            "Assertion failed in %s on line %d: %s\n",    \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                       \
    abort();                                              \
  }                                                       \
 } while (0)

/*Log message*/
#define LOG(expr)                                         \
{														  \
    fprintf(stdout,                                       \
            "%s:%d-->>%s\n",							  \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                                                                 \
 } 		

	
	static unsigned long uv_tcp_get_ip(const uv_tcp_t* handle, const bool ipv6)
	{
		if (handle == nullptr)
		{
			return 0;
		}

		if (ipv6)
		{
			sockaddr_in6 addr6;
			int namelen = sizeof(sockaddr_in6);
			int r = uv_tcp_getsockname(handle, (sockaddr*)&addr6, &namelen);
			if (r == 0)
			{
				return *((unsigned long*)addr6.sin6_addr.u.Byte);
			}
		}
		else
		{
			sockaddr_in addr4;
			int namelen = sizeof(sockaddr_in);
			int r = uv_tcp_getsockname(handle, (sockaddr*)&addr4, &namelen);
			if (r == 0)
			{
				return addr4.sin_addr.S_un.S_addr;
			}
		}
		return 0;
	}
	static unsigned short uv_tcp_get_port(const uv_tcp_t* handle, const bool ipv6)
	{
		if (ipv6)
		{
			sockaddr_in6 addr6;
			int namelen = sizeof(sockaddr_in6);
			int r = uv_tcp_getsockname(handle, (sockaddr*)&addr6, &namelen);
			if (r == 0)
			{
				return addr6.sin6_port;
			}
		}
		else
		{
			sockaddr_in addr4;
			int namelen = sizeof(sockaddr_in);
			int r = uv_tcp_getsockname(handle, (sockaddr*)&addr4, &namelen);
			if (r == 0)
			{
				return addr4.sin_port;
			}
		}
		return 0;
	}

	static int uv_string2ip(std::string& ip, unsigned long& addr)
	{

	}

	static int uv_ip2string(unsigned long& addr ,std::string& ip)
	{

	}
}
#endif // !_UV_NET_H_
