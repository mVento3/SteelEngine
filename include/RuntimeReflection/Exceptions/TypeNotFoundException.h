#pragma once

#include "string"
#include "exception"

namespace SteelEngine {

    struct TypeNotFoundException : public std::exception
    {
        std::string m_TypeName;

        TypeNotFoundException()
        {

        }

        TypeNotFoundException(const std::string& typeName) :
            m_TypeName(typeName)
        {

        }

        const char* what() const throw() override
        {
            if(m_TypeName != "")
            {
                return ("Type " + m_TypeName + " not found!").c_str();
            }
            else
            {
                return "Type not found!";
            }
        }
    };

}