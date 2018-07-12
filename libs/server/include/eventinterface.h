#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_
#include "dispatcher.h"
#include "packet.h"
#include "nocopyable.h"

class eventinterface : public nocopyable
{
	template<class T>
	class receiver : public ireceiver
	{
	public:
		typedef void(T::*function)(const void*, const int, const void*);


		receiver( const int id, T* object, function func) : m_id(id), m_object(object), m_receiver(func) {}

		virtual ~receiver() {
			m_object = NULL;
			m_receiver = NULL;
		}


		void invoke(const void* object, const int id, const void* data) const
		{
			if (id == m_id && m_object)
			{
				(m_object->*m_receiver)(object, id, data);
			}
		}

		const bool is_type(const std::type_info& type)const { return typeid(T) == type; }

		const bool equals(const ireceiver* other)const
		{
			if (other == NULL || other->is_type(typeid(T)) ==false)
			{
				return false;
			}
			auto r = static_cast<const receiver<T>*>(other);

			return m_object == r->m_object &&m_id == r->m_id  && m_receiver == r->m_receiver;
		}

	private:
		T* m_object;

		int m_id;

		function m_receiver;

	};

public:
	template<typename T>
	static bool listen(const int id, T* object, void (T::*func)(const void*, const int, const void*))
	{
		receiver<T>* rcv = new receiver<T>(id, object, func);
		bool r = m_dispatcher.listen(id, rcv);
		if (r == false)
		{
			delete rcv;
			rcv = NULL;
		}
		return r;
	}

	template<typename T>
	static void unlisten(const int id, T* object, void (T::*func)(const void*, const int, const void*))
	{
		receiver<T> other(id, object, func);
		m_dispatcher.unlisten(id, &other);
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
	eventinterface() {}
	~eventinterface() {}

private:
	static dispatcher m_dispatcher;
};


#endif _EVENT_H_