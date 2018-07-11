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
		/*
		 *容器大小，默认初始化为DEFAULT_SIZE
		 */
        size_t              size()      const   { return m_data.size(); }
		/*
		 *数据大小，实际填充的数据大小
		 */
        size_t              length()    const   { return m_length; }
        char*               data()              { return &m_data[0];}

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

		/*
		*不包含结束符'\0'
		*/
        void append(const std::string& str)
        {
			append((const void*)str.c_str(), str.size() + 1);
        }

		/*
		/* 如果data是字符串， size = strlen(data)，即不包含结束符'\0'
		*/
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

		/*
		* return 0 when read success. 
		* return 1 when offset out of rang.
		* return 2 when len out of rang.
		* 如果是字符串，不会再末尾加上结束符'\0'
		*/
        int read(char*dest, size_t offset, size_t len)
        {
            if(offset >= m_length)
            {
                return 1;
            }
			if (len > m_length)
			{
				return 2;
			}
			memcpy(dest, &m_data[offset], len);

            return 0;
        }

		/*
		* len = string.size() + 1
		* return 0 when read success.
		* return 1 when offset out of rang.
		* return 2 when len out of rang.
		*/
		int read(std::string& dest, size_t offset, size_t len)
		{
			if (offset > m_length)
			{
				return 1;
			}
			if (len > m_length)
			{
				return 2;
			}
			dest.assign((char*)(data() + offset), len);

			return 0;
		}

	private:
        std::vector<char>		m_data;
        size_t					m_length;

};


#endif
