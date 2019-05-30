#pragma once

#include "vector"
#include "cstring"

namespace SteelEngine {

    class Serialization
    {
    private:
        char*   m_DataStream;
        char*   m_Temp;

        template <typename A>
        void SerializeType(size_t& totalSize, const A& value, char* in)
        {
            A* t = (A*)in;

            *t = value;
            size_t s = sizeof(A);
            totalSize -= s;

            if(totalSize > 0)
            {
                t++;
            }

            m_Temp = (char*)t;
        }

        template <typename A, size_t N>
        void SerializeType(size_t& totalSize, const A(&arrayIn)[N], char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t s = sizeof(size_t);

            *sizePtr = N;
            sizePtr++;
            totalSize -= s;

            A* t = (A*)sizePtr;

            for(Type::uint32 i = 0; i < N; i++)
            {
                *t = arrayIn[i];
                
                size_t s = sizeof(A);
                totalSize -= s;

                if(totalSize > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        void SerializeType(size_t& totalSize, const std::vector<const char*>& value, char* in)
        {
            size_t s = sizeof(size_t);
            size_t* size = (size_t*)in;
            char* t = 0;

            *size = value.size();
            size++;
            totalSize -= s;

            for(Type::uint32 i = 0; i < value.size(); i++)
            {
                size_t s2 = strlen(value[i]);

                if(t)
                {
                    size = (size_t*)t;
                }

                *size = s2;
                size++;
                totalSize -= s;

                t = (char*)size;

                for(Type::uint32 j = 0; j < s2; j++)
                {
                    *t = value[i][j];
                
                    totalSize -= sizeof(value[i][j]);

                    if(totalSize > 0)
                    {
                        t++;
                    }
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void SerializeType(size_t& totalSize, const std::vector<A>& value, char* in)
        {
            size_t s = sizeof(size_t);
            size_t* size = (size_t*)in;

            *size = value.size();

            size++;

            totalSize -= s;

            A* t = (A*)size;

            for(Type::uint32 i = 0; i < value.size(); i++)
            {
                *t = value[i];
                
                s = sizeof(value[i]);
                totalSize -= s;

                if(totalSize > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(size_t& totalSize, A& value, char* in)
        {
            A* t = (A*)in;

            value = *t;
            size_t s = sizeof(A);
            totalSize -= s;

            if(totalSize > 0)
            {
                t++;
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(size_t& totalSize, A** value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t s = sizeof(size_t);
            char* str = new A[size];

            sizePtr++;

            totalSize -= s;

            A* t = (A*)sizePtr;

            for(Type::uint32 i = 0; i < size; i++)
            {
                str[i] = *t;
                s = sizeof(A);
                totalSize -= s;

                if(totalSize > 0)
                {
                    t++;
                }
            }

            *value = str;
            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(size_t& totalSize, A* value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t s = sizeof(size_t);

            sizePtr++;
            totalSize -= s;
            A* t = (A*)sizePtr;

            for(Type::uint32 i = 0; i < size; i++)
            {
                value[i] = *t;
                s = sizeof(A);
                totalSize -= s;

                if(totalSize > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A, size_t N>
        void DeserializeType(size_t& totalSize, A(&arrayIn)[N], char* in)
        {
            A* ptr = arrayIn;
            A* t = (A*)in;

            for(Type::uint32 i = 0; i < N; i++)
            {
                ptr[i] = *t;

                size_t s = sizeof(A);
                totalSize -= s;

                if(totalSize > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        template <typename A>
        void DeserializeType(size_t& totalSize, std::vector<A>& value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t s = sizeof(size_t);

            sizePtr++;

            A* t = (A*)sizePtr;

            totalSize -= s;

            for(Type::uint32 i = 0; i < size; i++)
            {
                value.push_back(*t);
                
                s = sizeof(*t);
                totalSize -= s;

                if(totalSize > 0)
                {
                    t++;
                }
            }

            m_Temp = (char*)t;
        }

        void DeserializeType(size_t& totalSize, std::vector<char*>& value, char* in)
        {
            size_t* sizePtr = (size_t*)in;
            size_t size = *sizePtr;
            size_t size2 = size;
            size_t s = sizeof(size_t);
            char* t = 0;

            size2 = *sizePtr;
            sizePtr++;
            totalSize -= s;

            for(Type::uint32 i = 0; i < size2; i++)
            {
                if(t)
                {
                    sizePtr = (size_t*)t;
                }

                size = *sizePtr;
                sizePtr++;
                totalSize -= s;

                t = (char*)sizePtr;

                char* arr = new char[size];

                for(Type::uint32 j = 0; j < size; j++)
                {
                    arr[j] = *t;
                    totalSize -= sizeof(*t);

                    if(totalSize > 0)
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
            m_DataStream = 0;
        }

        ~Serialization()
        {
            
        }

        template <typename A>
        void CalculateTotalSize(size_t& totalSize, const A& none)
        {
            totalSize += sizeof(none);
        }

        template <typename A, size_t N>
        void CalculateTotalSize(size_t& totalSize, const A(&arrayIn)[N])
        {
            totalSize += N;
            totalSize += sizeof(size_t);
        }

        void CalculateTotalSize(size_t& totalSize, const std::vector<const char*>& none)
        {
            for(Type::uint32 i = 0; i < none.size(); i++)
            {
                totalSize += sizeof(size_t);
                totalSize += strlen(none[i]);
            }

            totalSize += sizeof(size_t);
        }

        template <typename A>
        void CalculateTotalSize(size_t& totalSize, const std::vector<A>& none)
        {
            totalSize += sizeof(none.size() * sizeof(A));
            totalSize += sizeof(size_t);
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
            size_t totalSize = 0;

            int _[] = {0, (CalculateTotalSize(totalSize, value), 0)...};

            totalSize += sizeof(size_t);
            m_DataStream = new char[totalSize];

            m_Temp = m_DataStream;

            size_t* stringSizePtr = (size_t*)m_Temp;

            *stringSizePtr = totalSize;
            stringSizePtr++;
            totalSize -= sizeof(size_t);

            m_Temp = (char*)stringSizePtr;

            int _2[] = {0, (SerializeType(totalSize, value, m_Temp), 0)...};
        }

        template <typename... Types>
        void Deserialize(Types& ...value)
        {
            m_Temp = m_DataStream;

            size_t* stringSizePtr = (size_t*)m_Temp;

            size_t totalSize = *stringSizePtr;
            stringSizePtr++;
            totalSize -= sizeof(size_t);

            m_Temp = (char*)stringSizePtr;

            int _[] = {0, (DeserializeType(totalSize, value, m_Temp), 0)...};
        }

        inline char* GetDataStream()
        {
            return m_DataStream;
        }
    };

}