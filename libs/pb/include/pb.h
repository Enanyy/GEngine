#ifndef _PB_MESSAGE_H_
#define _PB_MESSAGE_H_

/*
����������ϢID����������
*/

namespace pb
{
	enum
	{
		SS_REGISTER_REQUEST			= 1,	//���ķ���ע�᱾������
		SS_REGISTER_RETURN			= 2,	//���ķ���ע�᱾���������

		CS_LOGIN_REQUEST			= 100,
		CS_LOGIN_RETURN				= 101
	};
}

#endif // !_PB_MESSAGE_H_
