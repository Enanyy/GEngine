#include "gmain.h"
#include "serverapp.h"
#include "singleton.h"
class testapp :public serverapp,
				public singleton<testapp>
{
public:
	testapp();
	~testapp();

private:

};

testapp::testapp()
{
}

testapp::~testapp()
{
}

GENGINE_MAIN(testapp)