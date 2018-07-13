#include "packet.h"
packet::packet():memorystream(PACKET_HEAD_LENGTH),m_index(0)
{

}
packet::packet(size_t size):memorystream(size),m_index(0)
{

}
packet::packet(std::string& data)
{
	clear();
	append(data);
}
packet::packet(const char* data, size_t length):memorystream(length), m_index(0)
{
	clear();
	append(data, length);
}
packet::packet(int id,const char* body, size_t bodylength,int extra):memorystream(PACKET_HEAD_LENGTH + bodylength), m_index(0)
{
	clear();

	time_t t = ::time(NULL);

	append<int>(id);				//id
	append<int>(PACKET_VERSION);	//version
	append<time_t>(t);			    //time
	append<int>(bodylength);		//bodylength
	append<int>(extra);				//extra
	append(body, bodylength);		//body
}

packet:: ~packet()
{
	clear();
	m_index = 0;
}


int packet::id()
{
	if (length() <= PACKET_ID_OFFSET)
	{
		return 0;
	}
	return memorystream::read<int>(PACKET_ID_OFFSET);
}

size_t  packet::body(std::string &data)
{
	if (length() <= PACKET_BODY_OFFSET)
	{
		return 0;
	}
	size_t len = bodylength();

	if (length() < len)
	{
		return 0;
	}

	int r = memorystream::read(data, PACKET_BODY_OFFSET, len);
	if (r != 0)
	{
		data.clear();
		len = 0;
	}
	return len;
}
size_t  packet::bodylength()
{
	if (length() <= PACKET_BODY_LENGTH_OFFSET)
	{
		return 0;
	}
	return memorystream::read<unsigned int>(PACKET_BODY_LENGTH_OFFSET);
}

size_t  packet::time()
{
	if (length() <= PACKET_TIMESTAMP_OFFSET)
	{
		return 0;
	}
	return memorystream::read<time_t>(PACKET_TIMESTAMP_OFFSET);
}

size_t	packet::version()
{
	if (length() <= PACKET_VERSION_OFFSET)
	{
		return 0;
	}
	return memorystream::read<unsigned int>(PACKET_VERSION_OFFSET);
}
size_t	packet::extra()
{

	if (length() <= PACKET_EXTRA_OFFSET)
	{
		return 0;
	}
	return memorystream::read<int>(PACKET_EXTRA_OFFSET);
}


int packet::read(char* dest, size_t len)
{
	int r = memorystream::read(dest, m_index, len);
	if (r == 0)
	{
		m_index += len;
	}
	return r;
}

int  packet::read(std::string& dest, size_t len)
{
	int r = memorystream::read(dest, m_index, len);
	if (r == 0)
	{
		m_index += len +1;
	}
	else
	{
		dest.clear();
	}

	return r;
}
