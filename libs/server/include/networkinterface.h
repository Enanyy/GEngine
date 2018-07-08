#pragma once
#ifndef _MESSAGE_DISPATCH_H_
#define _MESSAGE_DISPATCH_H_

#include <functional>
#include <map>
#include <list>
#include <typeinfo>
#include "singleton.h"
#include "packet.h"


class ireceiver
{
public:
	ireceiver() {}
	virtual ~ireceiver() {}

	virtual void invoke(void*, const int id, const char*, const size_t) const = 0;
	virtual bool is_type(const std::type_info&)const = 0;

};

template<class T>
class receiver : public ireceiver
{
public:
	typedef void(T::*function)(void*, const int, const char*, const size_t);


	receiver(T* object, int id, function& func) :m_object(object), m_id(id), m_receiver(func) {}

	virtual ~receiver() {
		m_object = NULL;
		m_receiver = NULL;
	}


	void invoke(void* u, const int id, const char* data, const size_t length) const
	{
		if (id == m_id && m_object)
		{
			(m_object->*m_receiver)(u, id, data, length);
		}
	}

	bool is_type(const std::type_info& type)const { return typeid(receiver<T>) == type; }



	bool equals(const std::type_info& type, const void* object, const int id, function func)const
	{
		if (object == NULL || is_type(type) == false)
		{
			return false;
		}

		return m_object == object && id == m_id && func == m_receiver;
	}

private:
	T* m_object;

	int m_id;

	function& m_receiver;

};
class networkinterface : public singleton<networkinterface>
{
	
public:
	
	template<typename T>
	bool listen(int id, T* object, void (T::*func)(void*, const int, const char*, const size_t))
	{
		if (object == NULL)
		{
			return false;
		}
		auto it = m_receivers.find(id);
		if (it == m_receivers.end())
		{
			m_receivers.insert(std::make_pair(id, std::list<ireceiver*>()));
		}
		bool exist = false;
		auto iter = m_receivers[id].begin();
		for (; iter != m_receivers[id].end(); ++iter)
		{
			if ((*iter)->is_type(typeid(receiver<T>)))
			{
				auto r = static_cast<receiver<T>*>((*iter));
				if (r != NULL && r->equals(typeid(T),object,id,func))
				{
					exist = true; break;
				}
			}
		}
		if (exist == false)
		{
			m_receivers[id].push_back(new receiver<T>(object, id, func));
		}

		return true;
	}
	template<typename T>
	void unlisten(int id, T* object, void (T::* func)(void*, const int, const char*, const size_t))
	{
		if (object == NULL)
		{
			return;
		}

		auto it = m_receivers.find(id);
		if (it != m_receivers.end())
		{
			for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
			{
				if ((*iter)->is_type(typeid(receiver<T>)))
				{
					auto r = static_cast<receiver<T>*>(*iter);
					if (r != NULL && r->equals(typeid(T), object, id, func))
					{
						iter = it->second.erase(iter);
						delete r;
						r = NULL;
						break;;
					}
				}
			}
		}
	}
	
	
	void dispatch(void*u, const int id, const char* data, const size_t length)
	{
		auto it = m_receivers.find(id);
		if (it != m_receivers.end())
		{
			for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
			{
				(*iter)->invoke(u, id, data, length);
			}
		}
	}

	void clear()
	{
		auto it = m_receivers.begin();
		for (; it != m_receivers.end(); ++it)
		{
			auto iter = it->second.begin();
			for (; iter != it->second.end(); ++iter)
			{
				auto* r = (*iter);
				delete r;
				r = NULL;
			}
			it->second.clear();
		}

		m_receivers.clear();
	}

private:
	std::map<int, std::list<ireceiver*> > m_receivers;
};

#endif _MESSAGE_DISPATCH_H_