#pragma once
#ifndef _gateway_APP_H_
#define _LOGIC_APP_H_
#include "serverapp.h"
#include "singleton.h"
#include "gmain.h"
class gatewayapp :public serverapp,
			      public singleton<gatewayapp>
{
public:
	gatewayapp(int id, serverapp_type type);
	virtual~gatewayapp();


private:

};


GENGINE_MAIN(gatewayapp)
#endif
