#include "eventinterface.h"
//注意，static成员的初始化要在实现中进行，不能在头文件进行
dispatcher eventinterface::m_dispatcher;