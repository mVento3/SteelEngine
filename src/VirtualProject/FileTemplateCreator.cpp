#include "VirtualProject/FileTemplateCreator.h"

#include "fstream"

#include "Utils/Utils.h"

namespace SteelEngine {

    FileTemplateCreator::FileTemplateCreator(const std::filesystem::path& templatePath)
    {
        std::ifstream file(templatePath);
        std::string line;

        while(std::getline(file, line))
        {
            m_LoadedTemplate.push_back(line);
        }

        file.close();
    }

    FileTemplateCreator::~FileTemplateCreator()
    {

    }

    void FileTemplateCreator::Create(const std::string& outFile, const VariablesMap& variables)
    {
        std::ofstream file(outFile);

        // for(Type::uint32 i = 0; i < m_LoadedTemplate.size(); i++)
        // {
        //     std::string line = m_LoadedTemplate[i];

        //     for(VariablesMap::const_iterator it = variables.begin(); it != variables.end(); ++it)
        //     {
        //         replaceAll(line, "%" + it->first + "%", it->second);
        //     }

        //     file << line << "\n";
        // }

        file << Create(variables);

        file.close();
    }

    std::string FileTemplateCreator::Create(const VariablesMap& variables)
    {
        std::string res = "";

        for(Type::uint32 i = 0; i < m_LoadedTemplate.size(); i++)
        {
            std::string line = m_LoadedTemplate[i];

            for(VariablesMap::const_iterator it = variables.begin(); it != variables.end(); ++it)
            {
                replaceAll(line, "%" + it->first + "%", it->second);
            }

            res.append(line).append("\n");
        }

        return res;
    }

}