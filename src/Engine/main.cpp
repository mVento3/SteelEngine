#include "Module/Module.h"
#include "Module/ModuleDetails.h"

#include "FileWatcher/FileWatcher.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "RuntimeReflection/Reflection.h"

#include "ModuleManager/ModuleManager.h"

#include "FileSystem/FileSystem.h"

#include "fstream"

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
        Options(char* argv[], Type::uint32 argc, std::vector<Descriptor>& desc) :
            m_Descriptor(&desc)
        {
            m_Arguments = "";

            for(Type::uint32 i = 0; i < argc; i++)
            {
                m_Arguments.append(argv[i]);

                if(i < argc - 1)
                {
                    m_Arguments.append(" ");
                }
            }

            std::vector<std::string> splitted = split(m_Arguments, '-');

            for(Type::uint32 i = 0; i < splitted.size(); i++)
            {
                std::string* sp = &splitted[i];

                if(sp->size() > 0)
                {
                    if(sp->at(sp->size() - 1) == ' ')
                    {
                        sp->erase(sp->end() - 1);
                    }

                    std::vector<std::string> splitted2 = split(*sp, ' ');

                    for(Type::uint32 j = 0; j < desc.size(); j++)
                    {
                        Descriptor* desc_ = &desc[j];

                        if(desc_->m_Option == splitted2[0])
                        {
                            desc_->m_IsSet = true;

                            if(splitted2.size() == 2)
                            {
                                desc_->m_Result = splitted2[1];
                            }

                            break;
                        }
                    }
                }
            }
        }

        ~Options()
        {

        }

        Descriptor operator[](Type::uint32 index)
        {
            return m_Descriptor->at(index);
        }
    };

}

enum Index
{
    SERVER,
    BIN_LOCATION,
    IP,
    ENGINE_DEV,
    GAME_DEV
};

std::vector<SteelEngine::Options::Descriptor> desc =
{
    { SERVER, "s" },
    { BIN_LOCATION, "p" },
    { IP, "ip" },
    { ENGINE_DEV, "ed" },
    { GAME_DEV, "gd" }
};

int main(int argc, char* argv[])
{
    // Skip program name
    argc -= (argc > 0);
    argv += (argc > 0);

    SteelEngine::Options parser(argv, argc, desc);

    // We need to prepare modules for engine
    SteelEngine::ModuleManager::SetGlobalBinaryLocation(parser[BIN_LOCATION].m_Result);
    SteelEngine::ModuleManager::LoadAll();

    SteelEngine::RuntimeDatabase* rd = (SteelEngine::RuntimeDatabase*)SteelEngine::ModuleManager::GetModule("RuntimeDatabase");

    rd->m_FileSystem = (SteelEngine::Interface::IFileSystem*)SteelEngine::Reflection::CreateInstance("FileSystem");

    // Then after that we need to prepare file system
    SteelEngine::FileSystem::SetBinaryLocation(parser[BIN_LOCATION].m_Result);
    SteelEngine::FileSystem::Map("bin", SteelEngine::FileSystem::GetBinaryLocation());

    SteelEngine::Interface::IRuntimeObject* core = SteelEngine::Reflection::CreateInstance("Core");

    if(parser[SERVER].m_IsSet)
    {
        SteelEngine::Reflection::GetType("Core")->SetMetaData(SteelEngine::Core::IS_SERVER, true);
    }
    else
    {
        SteelEngine::Reflection::GetType("Core")->SetMetaData(SteelEngine::Core::IS_SERVER, false);
        SteelEngine::Reflection::GetType("Core")->SetMetaData(SteelEngine::Core::SERVER_IP, parser[IP].m_Result);
    }

    if(!parser[ENGINE_DEV].m_IsSet && !parser[GAME_DEV].m_IsSet)
    {
        printf("Please specify path variant like -gd for game dev of -ed for engine dev!\n");

        return 1;
    }
    else if(parser[ENGINE_DEV].m_IsSet)
    {
        SteelEngine::Reflection::GetType("Core")->Invoke("SetPathVariant", core, SteelEngine::Core::EnginePathVariant::ENGINE_DEV);
    }
    else if(parser[GAME_DEV].m_IsSet)
    {
        SteelEngine::Reflection::GetType("Core")->Invoke("SetPathVariant", core, SteelEngine::Core::EnginePathVariant::GAME_DEV);
    }

    SteelEngine::Reflection::GetType("Core")->Invoke("Start", core);

    return EXIT_SUCCESS;
}