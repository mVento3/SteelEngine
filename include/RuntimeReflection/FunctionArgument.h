#pragma once

#include "string"

#include "RuntimeReflection/IFunctionArgument.h"

namespace SteelEngine {

    template <typename A>
    class FunctionArgument : public IFunctionArgument
    {
    private:
        size_t m_TypeID;
        std::string m_Name;

    public:
        FunctionArgument(const std::string& name) :
            m_Name(name),
            m_TypeID(typeid(A).hash_code())
        {

        }

        ~FunctionArgument()
        {

        }

        size_t GetTypeID() const override { return m_TypeID; }
        const std::string& GetName() const override { return m_Name; }
    };

}