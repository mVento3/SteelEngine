#include "RuntimeCompiler/RuntimeCompiler.h"

#include "Windows.h"

namespace SteelEngine {

    const std::string RuntimeCompiler::mcs_CompletionToken = "_COMPLETION_TOKEN_";

    RuntimeCompiler::RuntimeCompiler()
    {
        m_Process = 0;
    }

    RuntimeCompiler::RuntimeCompiler(ISubprocess* process) :
        m_Process(process)
    {

    }

    RuntimeCompiler::~RuntimeCompiler()
    {

    }

    void RuntimeCompiler::Setup()
    {
        // If no process is set, create new

        // if(m_Process)
        // {
        //     m_Process->Setup();
        // }
    }

    void RuntimeCompiler::Compile(
        const std::string& source,
        const std::vector<std::string>& flags,
        const std::vector<std::string>& defines,
        const std::vector<std::string>& includes,
        const std::vector<std::string>& libPaths,
        const std::vector<std::string>& libs)
    {
        std::string flagsStr = "";
        std::string definesStr = "";
        std::string includesStr = "";
        std::string libPathsStr = "";
        std::string libsStr = "";

        for(Type::uint32 i = 0; i < flags.size(); i++)
        {
            flagsStr += flags[i];

            if(i < flags.size() - 1)
            {
                flagsStr += " ";
            }
        }

        for(Type::uint32 i = 0; i < defines.size(); i++)
        {
            definesStr += "/D" + defines[i];

            if(i < defines.size() - 1)
            {
                definesStr += " ";
            }
        }

        for(Type::uint32 i = 0; i < includes.size(); i++)
        {
            includesStr += "/I\"" + includes[i] + "\"";

            if(i < includes.size() - 1)
            {
                includesStr += " ";
            }
        }

        for(Type::uint32 i = 0; i < libPaths.size(); i++)
        {
            libPathsStr += "/LIBPATH:\"" + libPaths[i] + "\"";

            if(i < libPaths.size() - 1)
            {
                libPathsStr += " ";
            }
        }

        for(Type::uint32 i = 0; i < libs.size(); i++)
        {
            libsStr += libs[i];

            if(i < libs.size() - 1)
            {
                libsStr += " ";
            }
        }

        std::string totalCommand =
            "cl " +
            flagsStr + " " +
            definesStr + " " +
            includesStr + " " +
            source + " /link " +
            libPathsStr + " " +
            libsStr;

        totalCommand += "\n" + mcs_CompletionToken + "\n";

		m_Process->WriteInput(totalCommand);
    }

    void RuntimeCompiler::Compile(
        const std::string& source,
        const Utils::json& flags,
        const Utils::json& definitions,
        const Utils::json& includes,
        const Utils::json& libPaths,
        const Utils::json& libs)
    {
        std::vector<std::string> flagVector = flags;
        std::vector<std::string> definitionVector = definitions;
        std::vector<std::string> includeVector = includes;
        std::vector<std::string> libPathVector = libPaths;
        std::vector<std::string> libVector = libs;

        Compile(
            source,
            flagVector,
            definitionVector,
            includeVector,
            libPathVector,
            libVector
        );
    }

    void RuntimeCompiler::Compile(
        const std::string& source,
        const Utils::json& options)
    {
        Compile(
            source,
            options["flags"],
            options["definitions"],
            options["includes"],
            options["lib_paths"],
            options["libs"]
        );
    }

    void RuntimeCompiler::WaitUntilComplete() const
    {
        while(!IsCompileComplete())
        {
            Sleep(10);
        }
    }

}