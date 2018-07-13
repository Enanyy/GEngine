#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "uv.h"

template <class T>
class singleton
{
    public: 
        static T* instance()
        {
           if(m_instance == nullptr)
           {
			   uv_mutex_init(&m_lock);
			   uv_mutex_lock(&m_lock);
               if(m_instance == nullptr)
               {
				   m_instance = new T;
               }
			   uv_mutex_unlock(&m_lock);
           }
           return m_instance;
        }

        static void destroy()
        {
            if(m_instance != nullptr)
            {
                delete m_instance;
				m_instance = nullptr;
            }
			uv_mutex_destroy(&m_lock);
        }
    protected:
		singleton() {  }
		virtual ~singleton() {  }

		singleton(const singleton&){}
		singleton operator=(const singleton&){}

    private:
        static T* m_instance;
		static uv_mutex_t m_lock;
};


#endif
