#pragma once
#ifndef _LOGIC_APP_H_
#define _LOGIC_APP_H_
#include "serverapp.h"
#include "singleton.h"
#include "gmain.h"
class logicapp :public serverapp,
				public singleton<logicapp>
{
public:
	logicapp(int id, serverapptype type);
	virtual ~logicapp();


private:

};


GENGINE_MAIN(logicapp)
#endif