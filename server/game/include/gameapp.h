#pragma once
#ifndef _GAME_APP_H_
#define _GAME_APP_H_
#include "serverapp.h"
#include "singleton.h"
#include "gmain.h"
class gameapp :public serverapp,
			   public singleton<gameapp>
{
public:
	gameapp(int id, serverapp_type type);
	virtual~gameapp();


private:

};


GENGINE_MAIN(gameapp)
#endif