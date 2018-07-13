#ifndef _PACKET_H_
#define _PACKET_H_

#include <time.h>
#include "memorystream.h"

class packet: public memorystream
{
    public:
        static const unsigned int PACKET_ID_OFFSET = 0;					//包ID偏移
		static const unsigned int PACKET_VERSION_OFFSET = 4;			//版本号偏移
        static const unsigned int PACKET_TIMESTAMP_OFFSET = 8;			//时间戳偏移
        static const unsigned int PACKET_BODY_LENGTH_OFFSET = 16;		//包体长度偏移
		static const unsigned int PACKET_EXTRA_OFFSET = 20;				//额外数据偏移
        static const unsigned int PACKET_BODY_OFFSET = 24;				//包体偏移

        static const unsigned int PACKET_HEAD_LENGTH = 24;				//包头长度

        static const unsigned int PACKET_SIZE = 64*1024;				//包默认初始化大小

		static const		  int PACKET_VERSION = 1;					//当前版本号
    public:
		packet();
		packet(size_t size);
		packet(const char* data, size_t length);
		packet(std::string& data);
		packet(int id, const char* body, size_t bodylength, int extra);
        virtual ~packet();

		const bool          is_valid()const {return length() >= PACKET_HEAD_LENGTH;}
        int                 id();
		
		/*
		 return bodylength
		*/
        size_t              body(std::string &data);
		size_t				bodylength();
		time_t				time();
		int					version();
		int					extra();

	public:
		template<typename T> T read() 
		{
			T t = memorystream::read<T>(m_index);

			m_index += sizeof(T);

			return t;
		}

		/*
		* return 0 when read success.
		* return 1 when offset out of rang.
		* return 2 when len out of rang.
		* 如果是字符串，不会再末尾加上结束符'\0'
		*/
		int read(char* dest, size_t len);
		
		/*
		* len = string.size()
		* return 0 when read success.
		* return 1 when offset out of rang.
		* return 2 when len out of rang.
		*/
		int read(std::string& dest, size_t len);
		

private:
	size_t m_index;

};

#endif
