#include "singleton.h"
//ע�⣬static��Ա�ĳ�ʼ��Ҫ��ʵ���н��У�������ͷ�ļ�����
template<class T>
T* singleton<T>::m_instance = nullptr;

template<class T> 
uv_mutex_t singleton<T>::m_lock;
