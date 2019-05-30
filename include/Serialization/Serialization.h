#pragma once

#include "vector"
#include "cstring"

namespace SteelEngine {

    class Serialization
    {
    private:
        char*   m_DataStream;
        char*   m_Temp;
        size_t  m_Size;

        template <typename A>
        void CalculateTotalSize(const A& none)
        {
            m_Size += sizeof(none);
        }

        template <typename A, size_t N>
        void CalculateTotalSize(const A(&arrayIn)[N])
        {
            m_Size += N;
            m_Size += sizeof(size_t);
        }

        void CalculateTotalSize(const std::vector<const char*>& none)
        {
            for(Type::uint32 i = 0; i < none.size(); i++)
            {
                m_Size += sizeof(size_t);
                m_Size += strlen(none[i]);
            }

            m_Size += sizeof(size_t);
        }

        template <typename A>
        void CalculateTotalSize(const std::vector<A>& none)
        {
            m_Size += sizeof(none.size() * sizeof(A));
            m_Size += sizeof(size_t);
        }

        template <typename A>
        void SerializeType(const A& value, char* in)
        {
            A* t = (A*)in;

            *t = value;
            size_t s = sizeof(A);
            m_Size -= s;

            if(m_Size > 0)
            {
                t++;
            }

            m_Temp = (char*)t;
        }

        template <typename A, size_t N>
        void SerializeType(const A(&arrayIn)[N], char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t s = sizeof(size_t);

            *sizePtr = N;
            sizePtr++;
            m_Size -= s;

            A* t = (A*)sizePtr;

            for(Type::uint32 i = 0; i < N; i++)
            {
                *t = arrayIn[i];
                
                size_t s = sizeof(A);
                m_Size -= s;

                if(m_Size > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        void SerializeType(const std::vector<const char*>& value, char* in)
        {
            size_t s = sizeof(size_t);
            size_t* size = (size_t*)in;
            char* t = 0;

            *size = value.size();
            size++;
            m_Size -= s;

            for(Type::uint32 i = 0; i < value.size(); i++)
            {
                size_t s2 = strlen(value[i]);

                if(t)
                {
                    size = (size_t*)t;
                }

                *size = s2;
                size++;
                m_Size -= s;

                t = (char*)size;

                for(Type::uint32 j = 0; j < s2; j++)
                {
                    *t = value[i][j];
                
                    m_Size -= sizeof(value[i][j]);

                    if(m_Size > 0)
                    {
                        t++;
                    }
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void SerializeType(const std::vector<A>& value, char* in)
        {
            size_t s = sizeof(size_t);
            size_t* size = (size_t*)in;

            *size = value.size();

            size++;

            m_Size -= s;

            A* t = (A*)size;

            for(Type::uint32 i = 0; i < value.size(); i++)
            {
                *t = value[i];
                
                s = sizeof(value[i]);
                m_Size -= s;

                if(m_Size > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(A& value, char* in)
        {
            A* t = (A*)in;

            value = *t;
            size_t s = sizeof(A);
            m_Size -= s;

            if(m_Size > 0)
            {
                t++;
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(A** value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t s = sizeof(size_t);
            char* str = new A[size];

            sizePtr++;

            m_Size -= s;

            A* t = (A*)sizePtr;

            for(Type::uint32 i = 0; i < size; i++)
            {
                str[i] = *t;
                s = sizeof(A);
                m_Size -= s;

                if(m_Size > 0)
                {
                    t++;
                }
            }

            *value = str;
            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(A* value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t s = sizeof(size_t);

            sizePtr++;

            m_Size -= s;

            A* t = (A*)sizePtr;

            for(Type::uint32 i = 0; i < size; i++)
            {
                value[i] = *t;
                s = sizeof(A);
                m_Size -= s;

                if(m_Size > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A, size_t N>
        void DeserializeType(A(&arrayIn)[N], char* in)
        {
            A* ptr = arrayIn;
            A* t = (A*)in;

            for(Type::uint32 i = 0; i < N; i++)
            {
                ptr[i] = *t;

                size_t s = sizeof(A);
                m_Size -= s;

                if(m_Size > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(std::vector<A>& value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t s = sizeof(size_t);

            sizePtr++;

            A* t = (A*)sizePtr;

            m_Size -= s;

            for(Type::uint32 i = 0; i < size; i++)
            {
                value.push_back(*t);
                
                s = sizeof(*t);
                m_Size -= s;

                if(m_Size > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        void DeserializeType(std::vector<char*>& value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t size2 = size;
            size_t s = sizeof(size_t);
            char* t = 0;

            size2 = *sizePtr;
            sizePtr++;
            m_Size -= s;

            for(Type::uint32 i = 0; i < size2; i++)
            {
                if(t)
                {
                    sizePtr = (size_t*)t;
                }

                size = *sizePtr;
                sizePtr++;
                m_Size -= s;

                t = (char*)sizePtr;

                char* arr = new char[size];

                for(Type::uint32 j = 0; j < size; j++)
                {
                    arr[j] = *t;
                    m_Size -= sizeof(*t);

                    if(m_Size > 0)
                    {
                        t++;
                    }
                }

                value.push_back(arr);
            }

            m_Temp = (char*)t;
        }

    public:
        Serialization()
        {
            m_Size = 0;
        }

        ~Serialization()
        {
            
        }

        void AllocateCustomSize(size_t size)
        {
            m_DataStream = new char[size];
        }

        template <typename Type>
        void SerializeData(const Type& data)
        {
            m_DataStream = new char[sizeof(Type)];

            memcpy(m_DataStream, &data, sizeof(Type));
        }

        template <typename Type>
        void DeserializeData(Type& data)
        {
            memcpy(&data, m_DataStream, sizeof(Type));
        }

        template <typename... Types>
        void Serialize(const Types& ...value)
        {
            if(!m_DataStream)
            {
                int _[] = {0, (CalculateTotalSize(value), 0)...};

                m_DataStream = new char[m_Size];
            }

            m_Temp = m_DataStream;

            size_t size = m_Size;

            int _2[] = {0, (SerializeType(value, m_Temp), 0)...};

            m_Size = size;
        }

        template <typename... Types>
        void Deserialize(Types& ...value)
        {
            size_t size = m_Size;
            m_Temp = m_DataStream;

            int _[] = {0, (DeserializeType(value, m_Temp), 0)...};

            m_Size = size;
        }

        inline char* GetDataStream()
        {
            return m_DataStream;
        }
    };

}