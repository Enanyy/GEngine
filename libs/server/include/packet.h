#ifndef _PACKET_H_
#define _PACKET_H_

#include <time.h>
#include "memorystream.h"

class packet: public memorystream
{
    public:
        static const unsigned int PACKET_ID_OFFSET = 0;					//��IDƫ��
		static const unsigned int PACKET_VERSION_OFFSET = 4;			//�汾��ƫ��
        static const unsigned int PACKET_TIMESTAMP_OFFSET = 8;			//ʱ���ƫ��
        static const unsigned int PACKET_BODY_LENGTH_OFFSET = 16;		//���峤��ƫ��
		static const unsigned int PACKET_EXTRA_OFFSET = 20;				//��������ƫ��
        static const unsigned int PACKET_BODY_OFFSET = 24;				//����ƫ��

        static const unsigned int PACKET_HEAD_LENGTH = 24;				//��ͷ����

        static const unsigned int PACKET_SIZE = 64*1024;				//��Ĭ�ϳ�ʼ����С

		static const		  int PACKET_VERSION = 1;					//��ǰ�汾��
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
		* ������ַ�����������ĩβ���Ͻ�����'\0'
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
