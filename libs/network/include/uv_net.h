#pragma once
#ifndef _UV_NET_H_
#define _UV_NET_H_

#include <stdint.h>
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
#define BUFFER_SIZE (1024*1024)

	inline bool little_endian()
	{
		int i = 0x1;
		return *(char*)&i == 0x1;
	}
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

	/*
	return 0 when success
	return -1 when fail
	*/
	int string2ip(const std::string& ip, unsigned long & address)
	{
		unsigned long trial;
#ifdef _MSC_VER
		if ((trial = inet_addr(ip.c_str())) != INADDR_NONE)
#else
		if (inet_aton(ip.c_str(), (struct in_addr*)&trial) != 0)
#endif
		{
			address = trial;
			return 0;
		}

		struct hostent * hosts = gethostbyname(ip.c_str());
		if (hosts != NULL)
		{
			address = *(unsigned long*)(hosts->h_addr_list[0]);
			return 0;
		}
		return -1;
	}

	int ip2string(unsigned long& address, std::string&ip)
	{
		address = ntohl(address);

		int p1, p2, p3, p4;
		p1 = address >> 24;
		p2 = (address & 0xffffff) >> 16;
		p3 = (address & 0xffff) >> 8;
		p4 = (address & 0xff);

		ip.clear();
		ip.resize(24);

		char* c_str = const_cast<char*>(ip.c_str());

		return sprintf(c_str, "%d.%d.%d.%d", p1, p2, p3, p4);
		
	}
}
#endif // !_UV_NET_H_
