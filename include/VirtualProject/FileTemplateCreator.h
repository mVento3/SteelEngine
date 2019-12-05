#pragma once

#include "string"
#include "map"
#include "vector"
#include "filesystem"

#include "Core/Type.h"

namespace SteelEngine {

    class FileTemplateCreator
    {
    public:
        typedef std::map<std::string, std::string> VariablesMap;

    private:
        std::vector<std::string> m_LoadedTemplate;

    public:
        FileTemplateCreator(const std::filesystem::path& templatePath);
        ~FileTemplateCreator();

        void Create(const std::string& outFile, const VariablesMap& variables);
        std::string Create(const VariablesMap& variables);
    };

}