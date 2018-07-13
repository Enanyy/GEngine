#include "singleton.h"
//注意，static成员的初始化要在实现中进行，不能在头文件进行
template<class T>
T* singleton<T>::m_instance = nullptr;

template<class T> 
uv_mutex_t singleton<T>::m_lock;
