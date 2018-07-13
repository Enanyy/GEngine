#pragma once
#ifndef _UV_NET_H_
#define _UV_NET_H_

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

#define PACKET_BUFFER_SIZE (1024*1024)
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

	
}
#endif // !_UV_NET_H_
