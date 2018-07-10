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

	void on_message(const void* u, const int id, const void* data);

	void on_event(const void* object, const int id, const void* data);

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
	networkinterface::listen<testapp>(1, this, &testapp::on_message);
	networkinterface::dispatch(NULL, 1, "aaa");

	networkinterface::unlisten<testapp>(1, this, &testapp::on_message);
	networkinterface::dispatch(NULL, 1, "aaa");
	printf("%zd \n", sizeof(time_t));
	printf("%zd \n", sizeof(size_t));

	

	return true;
}

void testapp::on_message(const void*u, const int id, const void*data)
{
	
}

void testapp::on_event(const void* object, const int id, const void* data)
{

}
GENGINE_MAIN(testapp)