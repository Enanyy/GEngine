#pragma once
#ifndef _DB_APP_H_
#define _DB_APP_H_
#include "serverapp.h"
#include "singleton.h"
#include "gmain.h"
class dbapp :public serverapp,
	public singleton<dbapp>
{
public:
	dbapp();
	~dbapp();


private:

};


GENGINE_MAIN(dbapp)
#endif