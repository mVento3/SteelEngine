#include "ConsoleSystem/ConsoleSystem.h"

#include "RuntimeReflection/Reflection.h"

namespace SteelEngine {

    ConsoleSystem::ConsoleSystem()
    {

    }

    ConsoleSystem::~ConsoleSystem()
    {

    }

    bool ConsoleSystem::Init()
    {
        IReflectionData const* const* types = Reflection::GetTypes();

        for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
        {
            const IReflectionData* type = types[i];
            Variant* consoleCommandVariant = type->GetMetaData(Reflection::ReflectionAttribute::CONSOLE_COMMAND);

            if(consoleCommandVariant->IsValid() && consoleCommandVariant->Convert<bool>())
            {
                RegisterCommand((ICommand*)type->Create());
            }
        }

        return true;
    }

    void ConsoleSystem::RegisterCommand(ICommand* command)
    {
        m_Commands.push_back(command);
    }

    std::vector<ICommand*> ConsoleSystem::GetCommands(const char* name)
    {
        std::vector<ICommand*> res;
        
        for(Type::uint32 i = 0; i < m_Commands.size(); i++)
        {
            ICommand* command = m_Commands[i];

            if(command->m_Command.rfind(name, 0) == 0)
            {
                res.push_back(command);
            }
        }

        return res;
    }

    ICommand* ConsoleSystem::GetCommand(const char* name)
    {
        for(Type::uint32 i = 0; i < m_Commands.size(); i++)
        {
            ICommand* command = m_Commands[i];

            if(strcmp(command->m_Command.c_str(), name) == 0)
            {
                return command;
            }
        }

        return 0;
    }

}