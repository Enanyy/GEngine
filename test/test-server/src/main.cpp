#include "gmain.h"
#include "serverapp.h"
#include "singleton.h"
#include "pb.h"
#include "server.pb.h"
#include <time.h>

class testapp :public serverapp,
	public singleton<testapp>
{
public:
	testapp(int id, serverapptype type);
	virtual ~testapp();

	bool initialize() override;

	void on_message(const uv_session* u, const int id, const packet* data);

	void on_event(const void* object, const int id, const void* data);

private:

};

testapp::testapp(int id, serverapptype type):serverapp(id,type)
{
}

testapp::~testapp()
{
}
bool testapp::initialize()
{
	packet p;
	p.append<int>(10);
	p.append<float>(45.0);
	p.append<double>(98.000);

	char* buf = "oqwooqw";
	int len = strlen(buf);
	p.append(buf, len);
	

	printf("size=%d length = %d\n", p.size(), p.length());
	std::string str = "ewewef";
	p.append(str);
	printf("append size = %d  length = %d\n", str.size(), str.length());
	printf("size=%d length = %d\n", p.size(), p.length());

	p.append<unsigned int>(41);
	p.append<size_t>(412112212122);


	pb::ss_register_request req;

	req.mutable_info()->set_id(1);
	req.mutable_info()->set_type(3);
	req.mutable_info()->set_name("AAAA");
	req.mutable_info()->set_ip("172.0.0.1");
	req.mutable_info()->set_port(900);

	std::string proto;
	req.SerializeToString(&proto);
	p.append(proto);


	

	printf("%d\n", p.read<int>());
	printf("%f\n", p.read<float>());
	printf("%f\n", p.read<double>());

	char* buff = new char[len+1];
	int r = p.read(buff, len);
	buff[len] = '\0';

	printf("r = %d buf = %s\n", r,buff);
	
	SAFE_DELETE(buff);

	std::string strr;
	r = p.read(strr, str.size());
	printf("r = %d strr=%s\n", r,strr.c_str());

	printf("%lu\n", p.read<unsigned int>());
	printf("%llu\n", p.read<size_t>());


	std::string strrr;
	r = p.read(strrr, proto.size());

	
	printf("r = %d\n", r);
	pb::ss_register_request ret;
	ret.ParseFromString(strrr);

	printf("id= %d,type= %d,name=%s,ip=%s,port=%d\n", ret.info().id(), ret.info().type(), ret.info().name(), ret.info().ip(), ret.info().port());


	networkinterface::listen(1, this, &testapp::on_message);
	networkinterface::dispatch(nullptr, 1, &p);

	networkinterface::unlisten(1, this, &testapp::on_message);
	networkinterface::dispatch(nullptr, 1, &p);


	eventinterface::listen(1, this, &testapp::on_event);
	eventinterface::dispatch(this, 1, "aaaaa");
	eventinterface::unlisten(1, this, &testapp::on_event);
	eventinterface::dispatch(this, 1, "aaaaa");

	return true;
}

void testapp::on_message(const uv_session* u, const int id, const packet* data)
{
	
	printf("packet size =%d\n", data->size());
}

void testapp::on_event(const void* sender, const int id, const void* data)
{
	char* str = (char*)data;
	printf("%s\n", str);
}
GENGINE_MAIN(testapp)