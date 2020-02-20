#pragma once

#include "RuntimeReflection/Macro.h"
#include "RuntimeReflection/Reflection.h"

#include "RuntimeReflection/Parser.h"

#include "RuntimeReflection/IReflectionGenerator.h"

namespace SteelEngine {

    struct IReflectionModule;

    SE_CLASS(
        Reflection::NO_SERIALIZE
    )
    class ReflectionGenerator : public IReflectionGenerator
    {
    public:
        

    private:
        std::vector<std::string> m_LoadedLines;
        Parser m_Parser;

        const std::vector<Parser::ScopeInfo*>* m_ParsedStructure;

        std::filesystem::path m_Filename;
        bool m_GenerateReflection;

        bool m_BeginRecordingOnce;
        bool m_EndRecordingOnce;

        // std::vector<std::string> m_GeneratedSourceLinesByModules;
        std::vector<std::string> m_GeneratedHeaderLinesByModules;

        std::string WriteArguments(const std::vector<Parser::Argument>& args, Parser::ScopeInfo* info);
        std::string WriteMetaData(const Parser::ScopeInfo* info);

        void Process(Parser::ScopeInfo* info, std::ofstream& file);

    public:
        ReflectionGenerator();
        ~ReflectionGenerator();

        Result Load(const std::filesystem::path& filename) override;
        Result Parse() override;
        Result Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath) override;
        void Clear() override;
    };

}