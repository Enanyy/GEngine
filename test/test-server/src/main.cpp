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
	networkinterface::listen(1, this, &testapp::on_message);
	networkinterface::dispatch(NULL, 1, "aaa");

	networkinterface::unlisten(1, this, &testapp::on_message);
	networkinterface::dispatch(NULL, 1, "aaa");
	printf("%zd \n", sizeof(time_t));
	printf("%zd \n", sizeof(size_t));

	event::listen(1, this, &testapp::on_event);
	event::dispatch(this, 1, "aaaaa");
	event::unlisten(1, this, &testapp::on_event);
	event::dispatch(this, 1, "aaaaa");

	return true;
}

void testapp::on_message(const void*u, const int id, const void*data)
{
	char* str = (char*)data;
	printf("%s\n", str);
}

void testapp::on_event(const void* object, const int id, const void* data)
{
	char* str = (char*)data;
	printf("%s\n", str);
}
GENGINE_MAIN(testapp)