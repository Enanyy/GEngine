#include "packet.h"
packet::packet():memorystream(PACKET_HEAD_LENGTH)
{

}
packet::packet(size_t size):memorystream(size)
{

}
packet::packet(char* data, size_t length):memorystream(length)
{
	clear();
	resize(length);
	append(data, length);
}
packet::packet(int id, char* body, size_t bodylength,int extra)
{
	m_length = (size_t)(PACKET_HEAD_LENGTH + bodylength);
	clear();

	resize(m_length);
	time_t t = ::time(NULL);

	append<int>(id);				//id
	append<int>(PACKET_VERSION);	//version
	append<time_t>(t);			    //time
	append<int>(bodylength);		//bodylength
	append<int>(extra);				//extra
	append(body, bodylength);
}

packet:: ~packet()
{
	clear();
}


int packet::id()
{
	if (m_length <= PACKET_ID_OFFSET)
	{
		return 0;
	}
	return read<int>(PACKET_ID_OFFSET);
}
char* packet::body()
{
	if (m_length <= PACKET_BODY_OFFSET)
	{
		return NULL;
	}
	return read(PACKET_BODY_OFFSET);
}
size_t  packet::bodylength()
{
	if (m_length <= PACKET_BODY_LENGTH_OFFSET)
	{
		return 0;
	}
	return read<unsigned int>(PACKET_BODY_LENGTH_OFFSET);
}
size_t  packet::time()
{
	if (m_length <= PACKET_TIMESTAMP_OFFSET)
	{
		return 0;
	}
	return read<size_t>(PACKET_TIMESTAMP_OFFSET);
}

size_t	packet::version()
{
	if (m_length <= PACKET_VERSION_OFFSET)
	{
		return 0;
	}
	return read<unsigned int>(PACKET_VERSION_OFFSET);
}
size_t	packet::extra()
{

	if (m_length <= PACKET_EXTRA_OFFSET)
	{
		return 0;
	}
	return read<unsigned int>(PACKET_EXTRA_OFFSET);
}
