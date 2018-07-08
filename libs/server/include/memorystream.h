#ifndef _MEMORY_STREAM_H_
#define _MEMORY_STREAM_H_


#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

class memorystream
{
    const static size_t DEFAULT_SIZE = 256;
    public:
        
		memorystream():m_length(0)
        {
            m_data.reserve(DEFAULT_SIZE);
        }
        
		memorystream(size_t size):m_length(0)
        {
            if(size)
            {
                m_data.reserve(size);
            }
        }

        virtual ~memorystream()
        {
            clear();
        }

        virtual void clear()
        {
            m_data.clear();
            m_length = 0;
        }

        bool                empty()     const   { return m_data.empty();}
        int                 size()      const   { return m_data.size(); }
        size_t              length()    const   { return m_length; }

        char*               data()              {return &m_data[0];}

        void resize(const size_t n)
        {
            if(size() < n){
                
                m_data.resize(n);
            }
        }


        void reserve(const size_t size)
        {
            if(size > m_data.size())
            {
                m_data.reserve(size);
            }
        }
        
        template <typename T> void append(T value)
        {
            append((const void*)&value, sizeof(T));
        }

        template<typename T> void append(const T* t, size_t size)
        {
            append((const void*)t, size*sizeof(T));
        }

        void append(const std::string& str)
        {
            append((const void*)str.c_str(), (size_t)(str.size() + 1));
        }

        void append(const char* data, size_t size)
        {
            append((const void*)data, size);
        }

        void append(const void* data, size_t size)
        {
            if(data == NULL)
            {
                return;
            }

            if(m_data.size() < m_length + size)
            {
                m_data.resize(m_length + size);
            }

            memcpy(&m_data[m_length], data, size);
            m_length += size;
        }


        template<typename T> T read(size_t offset) const
        {
            assert(sizeof(T) <= (m_length - offset));

            T t = *((T const *)&m_data[offset]);
            return t;
        }

        char* read(size_t offset)
        {
            if(offset >= m_length)
            {
                return NULL;
            }

            return (char*)&m_data[offset];
        }

    protected:
        std::vector<char> m_data;
        int m_length;

};


#endif
