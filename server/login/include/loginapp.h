#pragma once
#ifndef _LOGIN_APP_H_
#define _LOGIN_APP_H_
#include "serverapp.h"
#include "singleton.h"
#include "gmain.h"
class loginapp:public serverapp,
			   public singleton<loginapp>
{
public:
	loginapp(int id, serverapp_type type);
	virtual ~loginapp();
	

private:

};


GENGINE_MAIN(loginapp)
#endif