#pragma once
#ifndef _NETWORK_INTERFACE_H_
#define _NETWORK_INTERFACE_H_
#include "dispatcher.h"
#include "packet.h"
#include "nocopyable.h"
#include "uv_session.h"

using namespace network;
class networkinterface : public nocopyable
{
	template<class T>
	class receiver : public ireceiver
	{
	public:
		typedef void(T::*function)(const uv_session*, const int, const packet*);

		receiver( const int id, T* object, function func) : m_id(id), m_object(object),m_receiver(func) {}

		virtual ~receiver() {
			m_object = nullptr;
			m_receiver = nullptr;
		}


		void invoke(const void* object, const int id, const void* data) const
		{
			if (id == m_id && m_object)
			{
				(m_object->*m_receiver)((const uv_session*)object, id, (const packet*)data);
			}
		}

		const bool is_type(const std::type_info& type)const { return typeid(T) == type; }



		const bool equals(const ireceiver* other)const
		{
			if (other == nullptr || other->is_type(typeid(T)) == false)
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
	static bool listen(const int id, T* object, void (T::*func)(const uv_session*, const int, const packet*))
	{
		receiver<T>* rcv = new receiver<T>( id, object, func);
		bool r = m_dispatcher.listen(id, rcv);
		if (r == false)
		{
			SAFE_DELETE(rcv);
		}
		return r;
	}

	template<typename T>
	static void unlisten(const int id,  T* object,  void (T::*func)(const uv_session*, const int, const packet*))
	{ 
		receiver<T> other(id, object, func);
		m_dispatcher.unlisten(id, &other);
	}

	static void dispatch(const uv_session* object, const int id, const packet* data)
	{
		m_dispatcher.dispatch((const void*)object, id, (const void*)data);
	}
	
	static void clear()
	{
		m_dispatcher.clear();
	}

private:
	networkinterface() {}
	~networkinterface() {}

private:
	static dispatcher m_dispatcher;
};
 

#endif _NETWORK_INTERFACE_H_