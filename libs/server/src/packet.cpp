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
packet::packet(int id, char* body, size_t bodylength)
{
	m_length = (size_t)(PACKET_HEAD_LENGTH + bodylength);
	clear();

	resize(m_length);

	append<int>(id);
	append<int>(0);
	append<int>(bodylength);
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
	return read<unsigned int>(PACKET_TIMESTAMP_OFFSET);
}
