#pragma once
#ifndef _EVENT_DISPATCH_H_
#define _EVENT_DISPATCH_H_
#include "singleton.h"
#include <typeinfo>
#include <map>
#include <list>

class eventdispatch:public singleton<eventdispatch>
{
	class ireceiver
	{
	public:
		ireceiver() {}
		virtual ~ireceiver() {}

		virtual void invoke(void*, const int id, const void*) const = 0;
		virtual bool is_type(const std::type_info&)const = 0;

	};

	template<class T>
	class receiver : public ireceiver
	{
	public:
		typedef void(T::*function)(void*, const int, const void*);


		receiver(T* object, int id, function& func) :m_object(object), m_id(id), m_receiver(func) {}

		virtual ~receiver() {
			m_object = NULL;
			m_receiver = NULL;
		}


		void invoke(void* object, const int id, const void* data) const
		{
			if (id == m_id && m_object)
			{
				(m_object->*m_receiver)(object, id, data);
			}
		}

		bool is_type(const std::type_info& type)const { return typeid(T) == type; }



		bool equals(const std::type_info& type, const void* object, const int id, function& func)const
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

public:

	template<typename T>
	bool listen(int id, T* object, void (T::*func)(void*, const int, const void*))
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
			if ((*iter)->is_type(typeid(T)))
			{
				auto r = static_cast<receiver<T>*>((*iter));
				if (r != NULL && r->equals(typeid(T), object, id, func))
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
	void unlisten(int id, T* object, void (T::* func)(void*, const int, const void*))
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
				if ((*iter)->is_type(typeid(T)))
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


	void dispatch(void* object, const int id, const void* data)
	{
		auto it = m_receivers.find(id);
		if (it != m_receivers.end())
		{
			for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
			{
				(*iter)->invoke(object, id, data);
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



#endif // !_EVENT_DISPATCH_H_
