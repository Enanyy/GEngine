#pragma once
#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_
#include <typeinfo>
#include <map>
#include <list>
#include "nocopyable.h"
#include "platform.h"

class ireceiver
{
public:
	ireceiver() {}
	virtual ~ireceiver() {}

	virtual void invoke(const void*, const int id, const void*) const = 0;
	virtual const bool is_type(const std::type_info&)const = 0;
	virtual const bool equals(const ireceiver* receiver)const = 0;

};
class dispatcher :public nocopyable
{
public:
	bool listen(const int id, ireceiver* receiver )
	{
		if (receiver == NULL)
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
			if ((*iter)->equals(receiver))
			{
				exist = true;
				break;
			}
		}
		if (exist == false)
		{
			m_receivers[id].push_back(receiver);
		}

		return exist == false;
	}
	
	void unlisten(const int id, ireceiver* receiver)
	{
		if (receiver == NULL)
		{
			return;
		}

		auto it = m_receivers.find(id);
		if (it != m_receivers.end())
		{
			for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
			{
				auto r = (*iter);
				if (r->equals(receiver))
				{
					iter = it->second.erase(iter);
					delete r;
					r = NULL;
					break;;
				}
			}
		}
	}


	void dispatch(const void* object, const int id, const void* data)
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
				auto r = (*iter);
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



#endif // !_DISPATCHER_H_
