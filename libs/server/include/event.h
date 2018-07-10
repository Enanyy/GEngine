#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_
#include "dispatcher.h"
#include "packet.h"
#include "nocopyable.h"

class event : public nocopyable
{

public:
	template<typename T>
	static bool listen(const int id, T* object, void (T::*func)(const void*, const int, const void*))
	{
		return m_dispatcher.listen<T>(id, object, func);
	}

	template<typename T>
	static void unlisten(const int id, T* object, void (T::*func)(const void*, const int, const void*))
	{
		m_dispatcher.unlisten<T>(id, object, func);
	}

	static void dispatch(const void* object, const int id, const void* data)
	{
		m_dispatcher.dispatch(object, id, data);
	}

	static void clear()
	{
		m_dispatcher.clear();
	}

private:
	event() {}
	~event() {}

private:
	static dispatcher m_dispatcher;
};


#endif _EVENT_H_