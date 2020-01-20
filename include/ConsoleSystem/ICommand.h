#pragma once

#include "string"
#include "vector"

#include "HotReloader/IRuntimeObject.h"

namespace SteelEngine {

    struct ICommand : public HotReloader::IRuntimeObject
    {
        struct Options
        {
            std::string m_Option;
            std::string m_Value;
        };

        ICommand(const char* command) :
            m_Command(command)
        {

        }

        std::string m_Command;
        virtual void Function(const std::vector<Options>& options) = 0;
    };

}