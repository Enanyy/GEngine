#include "gmain.h"
#include "serverapp.h"
#include "singleton.h"
#include "pbmessage.h"
#include <time.h>

class testapp :public serverapp,
	public singleton<testapp>
{
public:
	testapp();
	~testapp();

	bool initialize() override;

	void on_message(void*u, const int id, const char*data, const size_t length);

private:

};

testapp::testapp()
{
}

testapp::~testapp()
{
}
bool testapp::initialize()
{
	networkinterface::instance()->listen<testapp>(1, this, &testapp::on_message);
	networkinterface::instance()->dispatch(NULL, 1, "aaa", 3);

	networkinterface::instance()->unlisten<testapp>(1, this, &testapp::on_message);
	networkinterface::instance()->dispatch(NULL, 1, "aaa", 3);
	printf("%d \n", sizeof(time_t));
	printf("%d \n", sizeof(size_t));

	return true;
}

void testapp::on_message(void*u, const int id, const char*data, const size_t length)
{
	
}
GENGINE_MAIN(testapp)