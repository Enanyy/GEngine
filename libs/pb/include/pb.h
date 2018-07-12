#ifndef _PB_MESSAGE_H_
#define _PB_MESSAGE_H_

/*
所有网络消息ID定义在这里
*/

namespace pb
{
	enum
	{
		SS_REGISTER_REQUEST			= 1,	//向别的服务注册本服务器
		SS_REGISTER_RETURN			= 2,	//向别的服务注册本服务器结果

		CS_LOGIN_REQUEST			= 100,
		CS_LOGIN_RETURN				= 101
	};
}

#endif // !_PB_MESSAGE_H_
