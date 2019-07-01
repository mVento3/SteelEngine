#pragma once

#include "Core/Type.h"

#include "string"
#include "vector"

#include "Utils/Utils.h"

namespace SteelEngine {

    class Options
    {
    public:
        struct Descriptor
        {
            Type::uint32 m_Index;
            std::string m_Option;
            std::string m_Result = "";
            bool m_IsSet = false;
        };

    private:
        std::string m_Arguments;
        std::vector<Descriptor>* m_Descriptor;

    public:
        Options(char* argv[], Type::uint32 argc, std::vector<Descriptor>& desc);
        ~Options();

        Descriptor operator[](Type::uint32 index);
    };

}