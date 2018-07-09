#ifndef _PACKET_H_
#define _PACKET_H_

#include <time.h>
#include "memorystream.h"

class packet: public memorystream
{
    public:
        static const unsigned int PACKET_ID_OFFSET = 0;
		static const unsigned int PACKET_VERSION_OFFSET = 4;
        static const unsigned int PACKET_TIMESTAMP_OFFSET = 8;
        static const unsigned int PACKET_BODY_LENGTH_OFFSET = 16;
		static const unsigned int PACKET_EXTRA_OFFSET = 20;
        static const unsigned int PACKET_BODY_OFFSET = 24;
        static const unsigned int PACKET_HEAD_LENGTH = 24;

        static const unsigned int PACKET_SIZE = 64*1024;

		static const unsigned int PACKET_VERSION = 1;
    public:
		packet();
		packet(size_t size);
		packet(char* data, size_t length);
		packet(int id, char* body, size_t bodylength, int extra);
        virtual ~packet();


        int                 id();
        char*               body();
		size_t				bodylength();
		size_t				time();
		size_t				version();
		size_t				extra();

};

#endif
