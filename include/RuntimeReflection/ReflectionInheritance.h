#pragma once

#include "RuntimeReflection/IReflectionInheritance.h"

namespace SteelEngine {

    class ReflectionInheritance : public IReflectionInheritance
    {
    private:
        std::string m_Name;
        size_t m_TypeID;

    public:
        ReflectionInheritance(const std::string& name, size_t typeID) :
            m_Name(name),
            m_TypeID(typeID)
        {

        }

        ~ReflectionInheritance()
        {

        }

        const std::string& GetName() override { return m_Name; }
        size_t GetTypeID() override { return m_TypeID; }
    };

}