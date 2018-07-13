#pragma once
#ifndef _SERVERAPP_INFO_H_
#define _SERVERAPP_INFO_H_
#include "uv_session.h"

enum serverapptype
{
	APP_NONE,
	APP_LOGIN,
	APP_GATEWAY,
	APP_LOGIC,
	APP_GAME,
	APP_DB,

	APP_MAX
};

enum serverappstate
{
	// 初始状态
	APP_STATE_INIT = 0,

	// 进程正在运行中
	APP_STATE_RUN = 1,

	// 进程开始关闭
	APP_STATE_SHUTTINGDOWN_BEGIN = 2,

	// 进程正在关闭
	APP_STATE_SHUTTINGDOWN_RUNNING = 3,

	// 进程关闭完成了
	APP_STATE_FINISH = 4
};

#define SERVERAPP_NAME_LEGNTH 16

static char serverappname[APP_MAX][SERVERAPP_NAME_LEGNTH] = {
	"none	",
	"login	",
	"gateway",
	"logic	",
	"game	",
	"db		"
};

struct serverappinfo
{
	int					id;
	serverapptype		type;
	serverappstate		state;
	uv_session*			session;
	std::string			ip;
	int					port;

};

#endif // !_SERVERAPP_INFO_H_
