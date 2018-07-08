#ifndef _PACKET_H_
#define _PACKET_H_

#include "memorystream.h"

class packet: public memorystream
{
    public:
        static const size_t PACKET_ID_OFFSET = 0;
		static const size_t PACKET_VERSION_OFFSET = 4;
        static const size_t PACKET_TIMESTAMP_OFFSET = 8;
        static const size_t PACKET_BODY_LENGTH_OFFSET = 12;
		static const size_t PACKET_EXTRA_OFFSET = 16;
        static const size_t PACKET_BODY_OFFSET = 20;
        static const size_t PACKET_HEAD_LENGTH = 20;

        static const size_t PACKET_SIZE = 64*1024;
    public:
		packet();
		packet(size_t size);
		packet(char* data, size_t length);
		packet(int id, char* body, size_t bodylength);
        virtual ~packet();


        int                 id();
        char*               body();
		size_t				bodylength();
		size_t				time();
		size_t				version();
		size_t				extra();

};

#endif
