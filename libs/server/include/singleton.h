#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "uv.h"

template <class T>
class singleton
{
    public: 
        static T* instance()
        {
           if(m_instance == NULL)
           {
               if(m_instance == NULL)
               {
				   m_instance = new T;
               }
           }
           return m_instance;
        }

        static void destroy()
        {
            if(m_instance !=NULL)
            {
                delete m_instance;
				m_instance = NULL;
            }
			
        }
    protected:
		singleton(){  }
        virtual ~singleton(){ }

		singleton(const singleton&){}
		singleton operator=(const singleton&){}

    private:
        static T* m_instance;
		static uv_mutex_t m_lock;
};

template<class T>
T* singleton<T>::m_instance = NULL;
#endif
