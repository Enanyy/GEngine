#pragma once
#ifndef _SERVERAPP_H_
#define _SERVERAPP_H_
#include "uv_service.h"
#include "nocopyable.h"
#include "packet.h"
#include "networkinterface.h"
#include "eventinterface.h"
#include "serverappinfo.h"
#include "pb.h"

using namespace network;

class serverapp:public uv_service_handler,
				public nocopyable
{
public:
	serverapp(int id, serverapptype type);
	virtual ~serverapp();

	

	const int					id()const { return m_id; }
	const serverapptype			type()const { return m_type; }
	const char*					name();
	/*
	�������ӱ�ķ�����
	*/
	bool						connectserver(const std::string& ip, const int port, const bool ipv6);

protected:
	/*
	���µ����ӣ������ǿͻ��ˣ�Ҳ�п����Ǳ�ķ�����
	*/
	virtual void on_newsession(uv_session* session) ;
	virtual void on_closesession(uv_session* session) ;
	/*
	ͨ��tcp�յ��İ��������ͻ�����������ͷ�����֮�����Ϣ
	*/
	virtual void on_tcpreceive(uv_session* session, const char* data, const size_t length);
	/*
	ͨ��udp�յ��İ�
	*/
	virtual void on_udpreceive(sockaddr_in* addr, const char* data, const size_t length);

	/*
	�������ӱ�ķ������ɹ��Ļص�
	*/
	virtual void on_newconnection(uv_session* connection) ;
	/*
	��ĳ���������Ͽ�����
	*/
	virtual void on_closeconnection(uv_session* connection) ;

protected:
	/*
	��ķ���������ע�ᵽ��������
	*/
	virtual void on_registerserver_request(const uv_session* session, const int id, const packet*data);
	/*
	����ע�ᵽ��ķ������Ľ��
	*/
	virtual void on_registerserver_return(const uv_session* session, const int id, const packet*data);

	virtual bool initialize();
	virtual void update();
	virtual void shutdown();
private:
	int						m_id;
	serverapptype			m_type;
};


#endif //_SERVERAPP_H_
